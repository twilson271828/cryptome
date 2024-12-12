#!/usr/bin/bash 

build_type=Debug
mkdir build && cd build
cmake ../ -GNinja -DCMAKE_BUILD_TYPE=$build_type