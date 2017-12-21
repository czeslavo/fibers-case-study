#!/bin/bash

mkdir tmp 
pushd tmp

wget https://dl.bintray.com/boostorg/release/1.65.1/source/boost_1_65_1.tar.gz
tar -xf boost_1_65_1.tar.gz 
pushd boost_1_65_1
echo "- Installing boost..."
./bootstrap.sh --with-libraries=context,coroutine,fiber,program_options,system --prefix=${TRAVIS_BUILD_DIR}/boost &> /dev/null
./b2 install &> /dev/null
echo "- Boost installed"
