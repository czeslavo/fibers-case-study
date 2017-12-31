#include <boost/coroutine2/all.hpp>
#include <iostream>

template <typename T>
using coro = boost::coroutines2::coroutine<T>;

int main() {
    coro<int>::pull_type generator([](auto& sink) {
        int current = 0;
        for(;;) sink(current++);
    });

    std::vector<int> from_0_to_9;
    std::copy_n(begin(generator), 10, std::back_inserter(from_0_to_9));

    for (const auto& v : from_0_to_9) {
        std::cout << v << ' ';
    }

    coro<int>::push_type printer([](auto& source) {
        for (const auto& n : source) 
            std::cout << n << ' ';
    });

    for (const auto& n : from_0_to_9)
        printer(n);
}
