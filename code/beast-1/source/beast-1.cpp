#include <iostream>
#include <string>
#include <array>

#include <boost/fiber/all.hpp>
#include <boost/asio.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>

namespace http = boost::beast::http;
namespace ip = boost::asio::ip;

boost::asio::io_service io_service{};
ip::tcp::resolver resolver{io_service};

std::string read_site(const std::string& address)
{
    ip::tcp::socket socket{io_service};

    const auto endpoint = resolver.resolve({address, "80"});
    socket.connect(*endpoint);
    
    http::request<http::string_body> req{http::verb::get, "/", 11};
    req.set(http::field::host, address);
    req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
    http::write(socket, req);

    boost::beast::flat_buffer buffer;
    http::response<http::dynamic_body> res;
    
    http::read(socket, buffer, res);
    
    std::stringstream out;
    out << res;
    return out.str();
}

int main()
{
    std::cout << read_site("www.example.com") << std::endl;
}
