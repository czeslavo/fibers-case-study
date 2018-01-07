#include <boost/fiber/all.hpp>
#include <iostream>

namespace fibers = boost::fibers;

fibers::buffered_channel<int> ch{8};
fibers::barrier b{3};  // barrier for producer, consument and main

// pushes 5 integers to channel
void producer() {
    for (auto i = 0u; i < 5; ++i) {
        std::cout << "Push " << i << '\n';
        ch.push(i);
        std::cout << "Pushed\n";
    }

    b.wait();  // wait until all reach the barrier.wait()
}

// consumes 5 integers from channel
void consument() {
    for (auto i = 0u; i < 5; ++i) {
        std::cout << "Start popping\n";
        const auto v = ch.value_pop();
        std::cout << "Pop " << v << '\n';
    }

    b.wait();  // wait until all reach the barrier.wait()
}

int main() {
    fibers::fiber prod(producer);
    fibers::fiber cons(consument);
    prod.detach();
    cons.detach();

    b.wait();  // wait until all reach the barrier.wait()
}
