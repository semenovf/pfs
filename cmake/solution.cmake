include(CheckIncludeFileCXX) # For use of CHECK_INCLUDE_FILE_CXX()
include(CheckFunctionExists)
# include(CheckCXXCompilerFlag)
include(WriteCompilerDetectionHeader)
# include(compiler)

#
# Check C++ features
#
write_compiler_detection_header(
  FILE "${CMAKE_BINARY_DIR}/cxx_features.h"
  PREFIX PFS
  COMPILERS GNU Clang MSVC
  FEATURES
    cxx_generalized_initializers
    cxx_variadic_templates
)

# pthread
find_package(Threads)

if(CMAKE_USE_PTHREADS_INIT)
    set(HAVE_PTHREAD 1)
endif()

# [FindBoost](https://cmake.org/cmake/help/git-master/module/FindBoost.html)
find_package(Boost COMPONENTS 
    system
    filesystem
    regex
    chrono
    atomic
    thread
)

set(HAVE_BOOST_SYSTEM_ERROR ${Boost_SYSTEM_FOUND})
set(HAVE_BOOST_FILESYSTEM   ${Boost_FILESYSTEM_FOUND})
set(HAVE_BOOST_REGEX        ${Boost_REGEX_FOUND})
set(HAVE_BOOST_CHRONO       ${Boost_CHRONO_FOUND})
set(HAVE_BOOST_THREAD       ${Boost_THREAD_FOUND})
set(HAVE_BOOST_MUTEX        ${Boost_THREAD_FOUND})
set(HAVE_BOOST_ATOMIC       ${Boost_ATOMIC_FOUND})
set(HAVE_BOOST_BIND         ${Boost_SYSTEM_FOUND})
set(HAVE_BOOST_SMART_PTR    ${Boost_SYSTEM_FOUND})
set(HAVE_BOOST_RATIO        ${Boost_SYSTEM_FOUND})
CHECK_INCLUDE_FILE_CXX(boost/move/unique_ptr.hpp HAVE_BOOST_UNIQUE_PTR)

CHECK_INCLUDE_FILE_CXX(system_error HAVE_STDCXX_SYSTEM_ERROR)
CHECK_INCLUDE_FILE_CXX(filesystem HAVE_STDCXX_FILESYSTEM)
CHECK_INCLUDE_FILE_CXX(chrono HAVE_STDCXX_CHRONO)
CHECK_INCLUDE_FILE_CXX(regex HAVE_STDCXX_REGEX)
CHECK_INCLUDE_FILE_CXX(thread HAVE_STDCXX_THREAD)

#
# Platform-specific features
#
if(UNIX)
    set(CMAKE_REQUIRED_INCLUDES net/if.h)
    CHECK_FUNCTION_EXISTS(if_nameindex HAVE_IF_NAMEINDEX)

    set(CMAKE_REQUIRED_INCLUDES ifaddrs.h)
    CHECK_FUNCTION_EXISTS(getifaddrs HAVE_GETIFADDRS)

    set(CMAKE_REQUIRED_INCLUDES netdb.h)
    CHECK_FUNCTION_EXISTS(getnameinfo HAVE_GETNAMEINFO)
endif(UNIX)

# # [CMake with Qt4](https://wiki.qt.io/Using_CMake_build_system)
# find_package(Qt4 COMPONENTS QtCore)
# 
# if(Qt4_FOUND)
#     set(HAVE_QT_CORE 1)
#     set(HAVE_QT4_CORE 1)
# endif(Qt4_FOUND)
# 
# # [CMake Manual](http://doc.qt.io/qt-5/cmake-manual.html)
# find_package(Qt5Core)
# 
# if (Qt5::Core)
#     set(HAVE_QT_CORE 1)
#     set(HAVE_QT5_CORE 1)
# endif()

# # PostgreSQL
# #
# # When CMake not found PostgreSQL but last is exists.
# # 1. On Ubuntu CMake outputs "-- Could NOT find PostgreSQL (missing:  PostgreSQL_TYPE_INCLUDE_DIR) (found version "9.X.X")"
# #    This occured because CMake not found 'pg_type.h'. Need to install package 'postgresql-server-dev-9.X'
# #
# find_package(PostgreSQL)
# 
# if (PostgreSQL_FOUND)
#     set(HAVE_POSTGRESQL 1)
# endif()

# Optional: Stack trace pretty printer library for C++
# https://github.com/bombela/backward-cpp
if (EXISTS ${CMAKE_CURRENT_LIST_DIR}/../3rdparty/backward-cpp/backward.hpp)
    set(HAVE_BACKWARD_CPP 1)
endif()

if (EXISTS ${CMAKE_CURRENT_LIST_DIR}/../3rdparty/variant/include/mpark/variant.hpp)
    set(HAVE_MPARK_VARIANT 1)
    include_directories(${CMAKE_CURRENT_LIST_DIR}/../3rdparty/variant/include)
endif()

#configure_file(${CMAKE_SOURCE_DIR}/pfs_config.h.in ${CMAKE_BINARY_DIR}/pfs_config.h)
configure_file(${CMAKE_CURRENT_LIST_DIR}/../pfs_config.h.in ${CMAKE_BINARY_DIR}/pfs_config.h)
