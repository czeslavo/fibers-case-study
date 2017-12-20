#!/bin/bash

cd code && mkdir -p build && cd build
cmake .. -DUSE_ALL=1 -DWITH_TESTS=1
make -j $(nproc)
ctest --vebose --color 
