#include <boost/fiber/all.hpp>
#include <iostream>

namespace fibers = boost::fibers;

fibers::buffered_channel<int> ch{8};
fibers::barrier b{3};

void producer() {
    for (auto i = 0u; i < 5; ++i) {
        std::cout << "Push " << i << '\n';
        ch.push(i);
        std::cout << "Pushed\n";
    }

    b.wait();
}

void consument() {
    for (auto i = 0u; i < 5; ++i) {
        std::cout << "Start popping\n";
        const auto v = ch.value_pop();
        std::cout << "Pop " << v << '\n';
    }

    b.wait();
}

int main() {  
    fibers::fiber prod(producer);
    fibers::fiber cons(consument);
    prod.detach();
    cons.detach();

    b.wait();
}
