#pragma once

#include <boost/spirit/home/x3.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <optional>

struct Config 
{
    int numOfThreads;
    std::string instanceName;
};

BOOST_FUSION_ADAPT_STRUCT(
    Config,
    (int, numOfThreads)
    (std::string, instanceName)
);

namespace parser
{
    using namespace boost::spirit::x3;

    const rule<class config, Config> config = "config";

    auto quoted_string = lexeme['"' >> +(char_ - '"') >> '"'];

    auto const config_def = 
        lit("config") >> 
        '{' >>
        int_ >> ',' >>
        quoted_string >>
        '}';

    BOOST_SPIRIT_DEFINE(config);
}

template <typename Iterator>
std::optional<Config> parse_config(Iterator first, Iterator last) 
{
    using namespace boost::spirit::x3;

    Config val;
    bool r = phrase_parse(first, last, ::parser::config, space, val);

    if (first != last or not r)
        return {};

    return val;
}
