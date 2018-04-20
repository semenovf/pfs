#!/bin/sh
ulimit -c unlimited
cmake -DCMAKE_CXX_STANDARD=${CXX_STANDARD} -DCMAKE_BUILD_TYPE=Release .
#cmake --build . && ctest
cmake --build . && tests/test-regex

gdb tests/test-regex core -ex "bt"