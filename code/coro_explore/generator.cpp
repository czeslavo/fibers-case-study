#include <boost/coroutine2/all.hpp>
#include <iostream>

template <typename T>
using coro = boost::coroutines2::coroutine<T>;

int main() {
    // infinite integers generator
    coro<int>::pull_type generator([](auto& sink) {
        int current = 0;
        for (;;) sink(current++);
    });

    // use std::copy_n to take first 10 integers from infinite generator
    std::vector<int> from_0_to_9;
    std::copy_n(begin(generator), 10, std::back_inserter(from_0_to_9));

    // print them
    for (const auto& v : from_0_to_9) {
        std::cout << v << ' ';
    }

    // invert the control, printer will consume values
    coro<int>::push_type printer([](auto& source) {
        for (const auto& n : source)  // wait for next push from outside
            std::cout << n << ' ';
    });

    for (const auto& n : from_0_to_9)
        printer(n);  // push all values to printer coroutine
}
