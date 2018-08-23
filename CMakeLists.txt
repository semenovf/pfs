# Ubuntu 14.04 - VERSION 2.8.12
cmake_minimum_required (VERSION 3.1)

set(PFS_TARGET_SHARED_LIB_NAME pfs)
set(PFS_TARGET_STATIC_LIB_NAME pfs-static)

include_directories(${CMAKE_CURRENT_LIST_DIR}/include ${CMAKE_SOURCE_DIR} ${CMAKE_BINARY_DIR})

add_subdirectory(src/pfs)
add_subdirectory(tests)
add_subdirectory(src/demo-logger)
