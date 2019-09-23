# Ubuntu 14.04 - VERSION 2.8.12
cmake_minimum_required (VERSION 3.1)
enable_testing()

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake;${CMAKE_MODULE_PATH}")
set(PFS_TARGET_SHARED_LIB_NAME pfs)
set(PFS_TARGET_STATIC_LIB_NAME pfs-static)

include(solution)

include_directories(${CMAKE_CURRENT_LIST_DIR}/include ${CMAKE_SOURCE_DIR} ${CMAKE_BINARY_DIR})

add_subdirectory(src/pfs)
add_subdirectory(tests)
add_subdirectory(src/demo-logger)
add_subdirectory(src/demo-sfinae)
#add_subdirectory(src/demo-socket_manager)
add_subdirectory(src/demo-epoll)
