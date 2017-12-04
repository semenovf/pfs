include(CheckIncludeFileCXX) # For use of CHECK_INCLUDE_FILE_CXX()

set(PFS_PREFERRED_CXX_STANDARD 98) # FIXME

# pthread
find_package(Threads)

# [FindBoost](https://cmake.org/cmake/help/git-master/module/FindBoost.html)
find_package(Boost COMPONENTS
    system
    filesystem
    chrono
    regex
    thread)

if(Boost_FOUND)
#    include_directories(${Boost_INCLUDE_DIRS})
    check_include_file_cxx("boost/ratio.hpp" HAVE_BOOST_RATIO)
    check_include_file_cxx("boost/smart_ptr.hpp" HAVE_BOOST_SMART_PTR)
    check_include_file_cxx("boost/filesystem.hpp" HAVE_BOOST_FILESYSTEM)
endif(Boost_FOUND)

# [CMake with Qt4](https://wiki.qt.io/Using_CMake_build_system)
find_package(Qt4 COMPONENTS QTCORE)

# [CMake Manual](http://doc.qt.io/qt-5/cmake-manual.html)
find_package(Qt5Core)
find_package(Qt5Widgets)

configure_file(${CMAKE_CURRENT_LIST_DIR}/config.h.in ${CMAKE_CURRENT_LIST_DIR}/config.h)

include_directories(include)
