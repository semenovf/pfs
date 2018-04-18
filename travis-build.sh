#!/bin/sh
cmake -DCMAKE_CXX_STANDARD=${CXX_STANDARD} -DCMAKE_BUILD_TYPE=Release .
#cmake --build . && ctest

#
# Verbose output
#
cmake --build . -- -v && ctest
