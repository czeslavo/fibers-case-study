#pragma once

#include <complex>
#include <optional>
#include <boost/spirit/home/x3.hpp>

template <typename Iterator>
bool parse_numbers(Iterator first, Iterator last)
{
    using boost::spirit::x3::double_;
    using boost::spirit::x3::phrase_parse;
    using boost::spirit::x3::ascii::space;
    using boost::spirit::x3::_attr;

    bool r = phrase_parse(
        first, last, 
        double_ >> *(',' >> double_),
        space
    );

    if (first != last)
        return false;

    return r;
}

template <typename Iterator>
bool parse_complex(Iterator first, Iterator last, std::complex<double>& c)
{
    using boost::spirit::x3::double_;
    using boost::spirit::x3::phrase_parse;
    using boost::spirit::x3::ascii::space;
    using boost::spirit::x3::_attr;

    double rN = 0.0;
    double iN = 0.0;
    auto fr = [&](auto& ctx) { rN = _attr(ctx); };
    auto fi = [&](auto& ctx) { iN = _attr(ctx); };

    auto grammar = '(' >> double_[fr] 
                       >> -(',' >> double_[fi]) >> ')'
                    |  double_[fr];
                       

    bool r = phrase_parse(first, last, grammar, space);

    if (first != last)
        return false;

    c = std::complex<double>(rN, iN);
    return r;
}

template <typename Iterator>
bool parse_doubles(Iterator first, Iterator last, std::vector<double>& v)
{
    using boost::spirit::x3::double_;
    using boost::spirit::x3::phrase_parse;
    using boost::spirit::x3::ascii::space;
    using boost::spirit::x3::_attr;
    
    auto grammar = double_ % ',';
    
    bool r = phrase_parse(first, last, grammar, space, v);

    if (first != last)
        return false;
    
    return r;
}

