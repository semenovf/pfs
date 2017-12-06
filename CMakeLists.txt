# Ubuntu 14.04 - VERSION 2.8.12
cmake_minimum_required (VERSION 2.8)

enable_testing()

add_subdirectory(src/pfs-test)
add_subdirectory(src/pfs)
add_subdirectory(tests)
