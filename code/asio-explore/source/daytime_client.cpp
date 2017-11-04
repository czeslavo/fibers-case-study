#include <iostream>
#include <array>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        std::cerr << "usage: daytime-client <host>\n";
        return 1;
    }

    boost::asio::io_service io;
    tcp::resolver resolver(io);
    tcp::resolver::query query(argv[1], "daytime");
    auto endpoint_it = resolver.resolve(query);
    tcp::socket socket(io);
    boost::asio::connect(socket, endpoint_it);


    for (;;)
    {
        std::array<char, 128> buffer;
        boost::system::error_code error;
        
        std::size_t len = socket.read_some(boost::asio::buffer(buffer), error);
        
        if (error == boost::asio::error::eof)
            break;
        else if (error)
            throw boost::system::system_error(error);

        std::cout.write(buffer.data(), len);
    } 
}
