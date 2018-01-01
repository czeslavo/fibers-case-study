#pragma once
#include <iostream>
#include <boost/lexical_cast.hpp>

template <typename SizeType>
struct Parameters {
    Parameters(int argc, char* argv[]) {
        if (argc < 2) {
            std::cerr << "usage: " << argv[0] << " size\n";
            std::exit(1);
        }
        size = boost::lexical_cast<SizeType>(argv[1]);
    }
    
    SizeType size;
};
