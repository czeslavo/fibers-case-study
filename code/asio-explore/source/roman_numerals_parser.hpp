#pragma once

#include <boost/spirit/home/x3.hpp>

namespace symbols_
{
    using namespace boost::spirit;

    struct hundreds_ : x3::symbols<int>
    {
        hundreds_() 
        {
            add("C",    100)
               ("CC",   200)
               ("CCC",  300)
               ("CD",   400)
               ("D",    500)
               ("DC",   600)
               ("DCC",  700)
               ("DCCC", 800)
               ("CM",   900);
        }
    };

    struct tens_ : x3::symbols<int>
    {
        tens_()
        {
            add("X",    10)
               ("XX",   20)
               ("XXX",  30)
               ("XL",   40)
               ("L",    50)
               ("LX",   60)
               ("LXX",  70)
               ("LXXX", 80)
               ("XC",   90);
        }
    };

    struct ones_ : x3::symbols<int>
    {
        ones_()
        {
            add("I",    1)
               ("II",   2)
               ("III",  3)
               ("IV",   4)
               ("V",    5)
               ("VI",   6)
               ("VII",  7)
               ("VIII", 8)
               ("IX",   9);
        }
    };
}

namespace parser
{
    using namespace boost::spirit::x3;
    using ascii::space;

    auto set_zero = [&](auto& ctx) { _val(ctx) = 0; };
    auto add1000  = [&](auto& ctx) { _val(ctx) += 1000; };
    auto add      = [&](auto& ctx) { _val(ctx) += _attr(ctx); };

    const rule<class roman, int> roman = "roman";
    symbols_::hundreds_ hundreds;
    symbols_::tens_ tens;
    symbols_::ones_ ones;

    auto const roman_def = 
        eps                   [set_zero] >> 
        (
            -(+lit('M')       [add1000]) >>
            -hundreds         [add]      >>
            -tens             [add]      >>
            -ones             [add]
        );

    BOOST_SPIRIT_DEFINE(roman);
}

template <typename Iterator>
std::optional<int> parse_roman_numeral(Iterator first, Iterator last)
{
    using namespace boost::spirit::x3;
                                      
    int val{};
    bool r = phrase_parse(first, last, ::parser::roman, space, val);
        
    if (first != last or not r)
    {
        return {};
    }

    return val;
}
