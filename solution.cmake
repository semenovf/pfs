#include(CheckIncludeFileCXX) # For use of CHECK_INCLUDE_FILE_CXX()
include(CheckCXXCompilerFlag)
include(${CMAKE_CURRENT_LIST_DIR}/compiler.cmake)

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
find_package(Qt4 COMPONENTS QTCORE)

# [CMake Manual](http://doc.qt.io/qt-5/cmake-manual.html)
find_package(Qt5Core)
find_package(Qt5Widgets)

configure_file(${CMAKE_CURRENT_LIST_DIR}/pfs_config.h.in ${CMAKE_BINARY_DIR}/pfs_config.h)

include_directories(${CMAKE_CURRENT_LIST_DIR}/include ${CMAKE_BINARY_DIR})
