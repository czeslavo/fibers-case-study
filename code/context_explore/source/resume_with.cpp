#include <boost/context/all.hpp>
#include <iostream>

namespace ctx = boost::context;

void print_sentence(const std::string& sentence) {
    std::cout << "Sentence is: " << sentence << '\n';
}

int main() {
    std::string sentence{};

    ctx::continuation sentence_builder = ctx::callcc([&](auto&& main_c) {
        sentence += "My";
        main_c = main_c.resume();

        sentence += " dog";
        main_c = main_c.resume();

        sentence += " is";
        main_c = main_c.resume();

        sentence += " tiny.";
        return std::move(main_c);
    });

    for (int i = 0; i < 3; ++i) {
        sentence_builder = sentence_builder.resume_with([&sentence](auto&& c) {
            print_sentence(sentence);
            return std::move(c);
        });
    }
    print_sentence(sentence);
}
