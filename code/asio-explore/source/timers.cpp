#include <iostream>
#include <boost/asio.hpp>
#include <boost/asio/high_resolution_timer.hpp>
#include <chrono>

class SecondsCounter 
{
public:
    SecondsCounter(const unsigned to_count, boost::asio::io_service& io) 
        : to_count(to_count), t(io, std::chrono::seconds(1))
    {
        t.async_wait([this](const auto&) { count_next(); });
    }

private:
    void count_next()
    {
        std::cout << ++counter << '\n';

        if (counter >= to_count) 
            return;

        t.expires_at(t.expires_at() + std::chrono::seconds(1));
        t.async_wait([this](const auto&) { count_next(); });
    }

    boost::asio::high_resolution_timer t;
    int counter{0};
    int to_count;
};

int main()
{
    boost::asio::io_service io;
    SecondsCounter secsCounter(5, io);

    io.run();
}
