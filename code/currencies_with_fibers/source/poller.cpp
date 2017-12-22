#include <array>
#include <iostream>
#include <string>

#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/fiber/all.hpp>
#include <boost/lexical_cast.hpp>
#include <json.hpp>

namespace http = boost::beast::http;
namespace ip = boost::asio::ip;
namespace fibers = boost::fibers;
namespace ssl = boost::asio::ssl;
using tcp = boost::asio::ip::tcp;
using channel_t = boost::fibers::buffered_channel<std::string>;
using json = nlohmann::json;

boost::asio::io_context io_context{};
ip::tcp::resolver resolver{io_context};

class FiberTimer {
   public:
    FiberTimer(const std::chrono::milliseconds period)
        : period(period), t(io_context, period) {
        t.async_wait([this](const auto&) { tick(); });
    }

   private:
    void tick() {
        t.expires_at(t.expires_at() + period);
        boost::this_fiber::yield();
        t.async_wait([this](const auto&) { tick(); });
    }

    boost::asio::high_resolution_timer t;
    std::chrono::milliseconds period;
};

void log(const std::string& s) {
    std::cout << "[" << boost::this_fiber::get_id() << "] " << s << '\n';
}

class CurrencyPoller {
   public:
    CurrencyPoller(const std::string& currency,
                   channel_t& channel, const std::chrono::seconds period)
        : ctx{ssl::context::sslv23_client},
          stream{io_context, ctx},
          currency{currency},
          channel{channel},
          period{period} {}

    void run() {
        connect();
        for (;;) poll(currency);
    }

   private:
    void poll(const std::string& currency) {
        http::request<http::string_body> req{
            http::verb::get, "/v1/ticker/" + currency + "/", 11};
        req.set(http::field::host, host);
        req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);

        http::write(stream, req);

        boost::beast::flat_buffer buffer;
        http::response<http::string_body> res;

        http::read(stream, buffer, res);
        channel.push(res.body());

        boost::this_fiber::sleep_for(period);
    }

    void connect() {
        const auto endpoints = resolver.resolve(host, "https");
        boost::asio::connect(stream.next_layer(), endpoints.begin(),
                             endpoints.end());
        stream.handshake(ssl::stream_base::client);
    }

    ssl::context ctx;
    ssl::stream<tcp::socket> stream;
    const std::string currency;
    const std::chrono::seconds period;
    channel_t& channel;

    static constexpr auto host = "api.coinmarketcap.com";
};

void start_polling(const std::string& c, channel_t& ch,
                   const std::chrono::seconds per = std::chrono::seconds(30)) {
    CurrencyPoller p{c, ch, per};
    p.run();
}

void currencies_printer(channel_t& channel) {
    std::map<std::string, double> prices;
    for (;;) {
        for (const auto& c : channel) {
            const auto j = json::parse(c);
            const auto o = j.at(0);
            const std::string name = o["name"];
            const auto price =
                boost::lexical_cast<double>(o["price_usd"].get<std::string>());

            if (prices.find(name) != prices.end()) {
                const auto& last = prices[name];
                if (price > last)
                    log(name + " +" + std::to_string(price - last) + "USD");
                else if (price < last)
                    log(name + " -" + std::to_string(last - price) + "USD");
            }

            prices[name] = price;
            boost::this_fiber::yield();
        }
    }
}

int main() {
    channel_t ch{2};

    boost::fibers::fiber btc{[&ch]() { start_polling("bitcoin", ch); }};
    boost::fibers::fiber ltc{[&ch]() { start_polling("litecoin", ch); }};
    boost::fibers::fiber printer{[&ch]() { currencies_printer(ch); }};

    btc.join();
    ltc.join();
    printer.join();
}
