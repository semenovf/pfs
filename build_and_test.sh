#!/bin/sh
#ulimit -c unlimited
cmake -DCMAKE_CXX_STANDARD=${CXX_STANDARD} -DCMAKE_BUILD_TYPE=Release .
cmake --build . && ctest --output-on-failure
#cmake --build . && tests/test-io-file
#cmake --build . && tests/test-static_cast

#gdb tests/test-regex core -ex "bt"