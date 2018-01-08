#include <algorithm>
#include <cassert>
#include <chrono>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <numeric>
#include <vector>

#include <boost/predef.h>
#include <boost/fiber/all.hpp>
#include "parameters.hpp"

using allocator_type = boost::fibers::fixedsize_stack;
using channel_type = boost::fibers::buffered_channel<std::uint64_t>;
using clock_type = std::chrono::high_resolution_clock;
using duration_type = clock_type::duration;
using time_point_type = clock_type::time_point;

// Funkcja benchmarku, przyjmująca:
// salloc - alokator stosu
// c - kanał rodzica
// num - numer porządkowy
// size - ilość agentów do utworzenia
// div - dzielnik określający ile dzieci utworzy każdy agent
void skynet(allocator_type& salloc, channel_type& c, std::size_t num,
            std::size_t size, std::size_t div) {
    if (1 == size) {  // warunek końcowy
        c.push(num);
    } else {
        channel_type rc{16};

        // utworzenie div dzieci
        for (std::size_t i = 0; i < div; ++i) {
            auto sub_num = num + i * size / div;
            boost::fibers::fiber{boost::fibers::launch::dispatch,
                                 std::allocator_arg,
                                 salloc,
                                 skynet,
                                 std::ref(salloc),
                                 std::ref(rc),
                                 sub_num,
                                 size / div,
                                 div}
                .detach();
        }

        // zsumowanie wartości otrzymanych od dzieci
        std::uint64_t sum{0};
        for (std::size_t i = 0; i < div; ++i) {
            sum += rc.value_pop();
        }

        // przesłanie wyniku sumowania do rodzica
        c.push(sum);
    }
}

int main(int argc, char* argv[]) {
    // klasa pomoczniczna Parameters, odczytująca pierwszy argument
    // podany z linii komend jako rozmiar problemu
    const auto size = Parameters<std::size_t>(argc, argv).size;
    constexpr std::size_t div{10};

    // alokator stosu 16kB
    constexpr auto stack_size = 16 * 1024;
    allocator_type salloc{stack_size};

    channel_type rc{2};  // kanał do komunikacji z pierwszym poziomem benchmarku
    const time_point_type start{
        clock_type::now()};              // początkowy punkt odmierzania czasu
    skynet(salloc, rc, 0, size, div);    // uruchomienie benchmarku
    const auto result = rc.value_pop();  // odczytanie końcowego wyniku
    const auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(
        clock_type::now() - start);  // zakończenie odmierzania czasu
    std::cout << duration.count() << std::endl;  // wypisanie czasu w ms
}
