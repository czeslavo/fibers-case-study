#include <iostream>
#include <array>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        std::cerr << "usage: roman-numerals-client <numeral>\n";
        return 1;
    }

    boost::asio::io_service io;
    tcp::resolver resolver(io);
    tcp::resolver::query query("localhost", "4050");
    auto endpoint_it = resolver.resolve(query);
    tcp::socket socket(io);
    boost::asio::connect(socket, endpoint_it);
    
    std::string input;
    while (std::getline(std::cin, input)) 
    { 
        std::cout << "Read " << input << '\n';
        boost::system::error_code error;
        boost::asio::write(socket, boost::asio::buffer(input), error);

        std::array<char, 128> buffer;
        std::size_t len = socket.read_some(boost::asio::buffer(buffer), error);
        std::cout.write(buffer.data(), len - 1);
        std::cout << std::endl;
    }
}
