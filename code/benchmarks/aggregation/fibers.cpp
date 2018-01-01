#include <boost/fiber/all.hpp>
#include <boost/lexical_cast.hpp>
#include <chrono>
#include <iostream>

namespace fibers = boost::fibers;

void worker(const unsigned int n, fibers::unbuffered_channel<int>& ch,
            fibers::barrier& b, const unsigned int workers) {
    // send its value
    ch.push(n);

    // spawn next if n < 1000
    if (n < workers) {
        fibers::fiber next_worker([&]() { worker(n + 1, ch, b, workers); });
        next_worker.detach();
    }

    b.wait();
}

int aggregator(fibers::unbuffered_channel<int>& ch,
               const unsigned int workers) {
    int sum = 0;
    unsigned int n = 1;
    for (const auto v : ch) {
        sum += v;
        if (n++ == workers) break;
    }

    return sum;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "usage: " << argv[0] << " number_of_agents\n";
        exit(1);
    }
    const auto workers = boost::lexical_cast<unsigned int>(argv[1]);

    fibers::unbuffered_channel<int> channel{};
    fibers::barrier b{workers + 1};

    fibers::fiber first_worker([&]() { worker(1, channel, b, workers); });
    first_worker.detach();

    auto sum =
        fibers::async([&]() { return aggregator(channel, workers); });

    auto val = sum.get();
    std::cout << "Sum = " << val << '\n';
    b.wait();
}
