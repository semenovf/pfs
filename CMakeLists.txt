# Ubuntu 14.04 - VERSION 2.8.12
cmake_minimum_required (VERSION 3.1)

enable_testing()

#set(CMAKE_CXX_STANDARD 14)

set(PFS_TARGET_SHARED_LIB_NAME pfs)
set(PFS_TARGET_STATIC_LIB_NAME pfs-static)

add_subdirectory(src/pfs)
add_subdirectory(tests)
add_subdirectory(src/demo-logger)
