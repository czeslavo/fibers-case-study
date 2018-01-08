#!/bin/bash

cd code && mkdir -p build && cd build
cmake .. -DUSE_ALL=1
make -j
ctest -V
