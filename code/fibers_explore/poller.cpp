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

#include "round_robin.hpp"
#include "yield.hpp"

namespace http = boost::beast::http;
namespace ip = boost::asio::ip;
namespace fibers = boost::fibers;
namespace ssl = boost::asio::ssl;
using tcp = boost::asio::ip::tcp;
using channel_t = boost::fibers::unbuffered_channel<std::string>;
using json = nlohmann::json;

void log(const std::string& s) {
    std::cout << "[" << boost::this_fiber::get_id() << "] " << s << '\n';
}

class CurrencyPoller {
   public:
    CurrencyPoller(std::shared_ptr<boost::asio::io_context> io,
                   const std::string& currency, channel_t& channel,
                   const std::chrono::seconds period)
        : io{io},
          resolver{*io},
          ctx{ssl::context::sslv23_client},
          stream{*io, ctx},
          currency{currency},
          channel{channel},
          period{period} {}

    void run() {
        connect();
        for (;;) poll(currency);
    }

   private:
    void poll(const std::string& currency) {
        boost::system::error_code ec;
        http::request<http::string_body> req{
            http::verb::get, "/v1/ticker/" + currency + "/", 11};
        req.set(http::field::host, host);
        req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);

        http::async_write(stream, req, boost::fibers::asio::yield[ec]);

        boost::beast::flat_buffer buffer;
        http::response<http::string_body> res;

        http::async_read(stream, buffer, res, boost::fibers::asio::yield[ec]);
        channel.push(res.body());

        boost::this_fiber::sleep_for(period);
    }

    void connect() {
        const auto endpoints = resolver.resolve(host, "https");
        boost::asio::async_connect(stream.next_layer(), endpoints.begin(),
                                   endpoints.end(),
                                   boost::fibers::asio::yield[ec]);
        stream.handshake(ssl::stream_base::client);
    }

    std::shared_ptr<boost::asio::io_context> io;
    ip::tcp::resolver resolver;
    ssl::context ctx;
    ssl::stream<tcp::socket> stream;
    const std::string currency;
    const std::chrono::seconds period;
    channel_t& channel;

    boost::system::error_code ec;
    static constexpr auto host = "api.coinmarketcap.com";
};

void start_polling(std::shared_ptr<boost::asio::io_context> io,
                   const std::string& c, channel_t& ch,
                   const std::chrono::seconds per = std::chrono::seconds(5)) {
    try {
        CurrencyPoller p{io, c, ch, per};
        p.run();
    } catch (...) {
        log("Caught unexpected exception, stop polling");
    }
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
        }
    }
}

int main() {
    auto io = std::make_shared<boost::asio::io_context>();
    boost::fibers::use_scheduling_algorithm<boost::fibers::asio::round_robin>(
        io);

    channel_t ch;

    boost::fibers::fiber{[io, &ch]() {
        start_polling(io, "bitcoin", ch);
    }}.detach();
    boost::fibers::fiber{[io, &ch]() {
        start_polling(io, "litecoin", ch);
    }}.detach();
    std::thread{[io, &ch]() { currencies_printer(ch); }}.detach();

    io->run();
}
