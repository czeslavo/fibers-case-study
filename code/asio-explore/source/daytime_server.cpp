#include <iostream>
#include <array>
#include <ctime>
#include <string>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

std::string make_daytime_string()
{
    using namespace std;
    time_t now = time(0);
    return ctime(&now);
}

int main(int argc, char* argv[])
{
    boost::asio::io_service io;
    
    tcp::acceptor acceptor(io, tcp::endpoint(tcp::v4(), 13));

    for (;;)
    {
        tcp::socket socket(io);
        acceptor.accept(socket);
        
        const auto daytime_string = make_daytime_string();
        boost::system::error_code ignored_error;
        boost::asio::write(socket, boost::asio::buffer(daytime_string), ignored_error);
    } 
}
