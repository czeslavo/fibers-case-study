#include <iostream>
#include <boost/asio.hpp>
#include <vector>
#include <array>
#include <string_view>
#include "roman_numerals_parser.hpp"

using boost::asio::ip::tcp;
using namespace std::string_literals;

class Session : public std::enable_shared_from_this<Session>
{
public:
    Session(tcp::socket socket)
        : socket(std::move(socket)) 
    {
    }

    void run() 
    {
        auto self = shared_from_this();
        socket.async_read_some(boost::asio::buffer(data_buffer), [this, self](auto& err, size_t bytes) {
            if (not err)
            {
                const auto read_ = std::string_view(data_buffer.data(), bytes);
                onSuccessRead(read_);
            }
            else
            {
                fail("reading failed");
            }
        }); 
    }

    void onSuccessRead(const std::string_view& literal)
    {
        const auto numeral = parse_roman_numeral(literal.begin(), literal.end());
       
        if (numeral) 
        {
            const std::string numeralStr = std::to_string(numeral.value());
            std::copy(numeralStr.begin(), numeralStr.end(), data_buffer.begin());
            auto self = shared_from_this();
            boost::asio::async_write(socket, boost::asio::buffer(data_buffer), 
            [this, self, numeralStr](auto& err, size_t bytes) {
                std::cout << "Answered " << numeralStr << '\n';
                run();
            });
        }
        else
        {
            auto self = shared_from_this();
            boost::asio::async_write(socket, boost::asio::buffer("couldn't parse literal"), [this, self](auto& err, size_t bytes) {
                std::cout << "Answered, finish\n";
                run();
            });
        }
    }

    void fail(const std::string& reason) const 
    {
        std::cout << "session id " << id << "failed: " << reason << '\n';
    }

private:
    static size_t ids_counter;
    const size_t id = ids_counter++;
    
    std::array<char, 1024> data_buffer;

    tcp::socket socket;
};

size_t Session::ids_counter = 0;

class Server
{
public:
    Server(boost::asio::io_service& io)
        : acceptor(io, tcp::endpoint(tcp::v4(), 4050)),
          socket(io)
    {
        startAccepting();
        io.run();
    }
    
    void startAccepting() 
    {
        acceptor.async_accept(socket, [this](auto& err) {

            if (not err)
            {
                std::make_shared<Session>(std::move(socket))->run();
            }

            startAccepting();
        });
    }

private:
    tcp::acceptor acceptor;
    tcp::socket socket;
};

int main()
{
    boost::asio::io_service io;
    auto server = Server(io);
}
