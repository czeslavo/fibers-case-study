#!/bin/bash

BOOST_DIR="${TRAVIS_BUILD_DIR}/boost"
BOOST_CMAKE_FLAGS="-DBOOST_ROOT=${BOOST_DIR}" 

cd code && mkdir -p build && cd build
cmake .. -DUSE_ALL=1 -DWITH_TESTS=1 "${BOOST_CMAKE_FLAGS}"
make -j
