#!/bin/bash

mkdir tmp 
pushd tmp

wget https://dl.bintray.com/boostorg/release/1.66.0/source/boost_1_66_0.tar.gz
tar -xvf boost_1_66_0.tar.gz 
pushd boost_1_66_0
./bootstrap.sh --with-libraries=context,coroutine,fiber,program_options,system --prefix=${TRAVIS_BUILD_DIR}/boost
./b2 install


