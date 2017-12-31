#include(CheckIncludeFileCXX) # For use of CHECK_INCLUDE_FILE_CXX()
include(CheckFunctionExists)
include(CheckCXXCompilerFlag)
include(${CMAKE_CURRENT_LIST_DIR}/compiler.cmake)

if(UNIX)
    set(CMAKE_REQUIRED_INCLUDES net/if.h)
    CHECK_FUNCTION_EXISTS(if_nameindex HAVE_IF_NAMEINDEX)

    set(CMAKE_REQUIRED_INCLUDES ifaddrs.h)
    CHECK_FUNCTION_EXISTS(getifaddrs HAVE_GETIFADDRS)

    set(CMAKE_REQUIRED_INCLUDES netdb.h)
    CHECK_FUNCTION_EXISTS(getnameinfo HAVE_GETNAMEINFO)
endif(UNIX)

# pthread
find_package(Threads)

if(CMAKE_USE_PTHREADS_INIT)
    set(HAVE_PTHREAD 1)
endif()

# [FindBoost](https://cmake.org/cmake/help/git-master/module/FindBoost.html)
find_package(Boost COMPONENTS
    system
    filesystem
    chrono
    regex
    thread)

if(Boost_FOUND)
    # TODO try to use `find_path` instead of `if(EXISTS ...)`

    if(EXISTS ${Boost_INCLUDE_DIR}/boost/ratio.hpp)
        set(HAVE_BOOST_RATIO 1)
    endif()

    if(EXISTS ${Boost_INCLUDE_DIR}/boost/smart_ptr.hpp)
        set(HAVE_BOOST_SMART_PTR 1)
    endif()

    if(EXISTS ${Boost_INCLUDE_DIR}/boost/filesystem.hpp)
        set(HAVE_BOOST_FILESYSTEM 1)
    endif()

    if(EXISTS ${Boost_INCLUDE_DIR}/boost/chrono.hpp)
        set(HAVE_BOOST_CHRONO 1)
    endif()

    if(EXISTS ${Boost_INCLUDE_DIR}/boost/regex.hpp)
        set(HAVE_BOOST_REGEX 1)
    endif()

    if(EXISTS ${Boost_INCLUDE_DIR}/boost/thread.hpp)
        set(HAVE_BOOST_THREAD 1)
    endif()

    if(EXISTS ${Boost_INCLUDE_DIR}/boost/system/system_error.hpp)
        set(HAVE_BOOST_SYSTEM_ERROR 1)
    endif()
endif(Boost_FOUND)

# [CMake with Qt4](https://wiki.qt.io/Using_CMake_build_system)
find_package(Qt4 COMPONENTS QtCore)

if(Qt4_FOUND)
    set(HAVE_QT_CORE 1)
    set(HAVE_QT4_CORE 1)
endif(Qt4_FOUND)

# [CMake Manual](http://doc.qt.io/qt-5/cmake-manual.html)
find_package(Qt5Core)
find_package(Qt5Widgets)

configure_file(${CMAKE_CURRENT_LIST_DIR}/pfs_config.h.in ${CMAKE_BINARY_DIR}/pfs_config.h)

include_directories(${CMAKE_CURRENT_LIST_DIR}/include ${CMAKE_BINARY_DIR})