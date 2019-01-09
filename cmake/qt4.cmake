#
# [CMake with Qt4](https://wiki.qt.io/Using_CMake_build_system)
#

if(MY_QT_REQUIRED)
    find_package(Qt4 REQUIRED COMPONENTS QtCore ${MY_QT_REQUIRED})
else()
    find_package(Qt4 REQUIRED COMPONENTS QtCore)
endif()

set(HAVE_QT_CORE ${Qt4_FOUND})
set(HAVE_QT4_CORE ${Qt4_FOUND})

include(${QT_USE_FILE})
add_definitions(${QT_DEFINITIONS})
include_directories(${CMAKE_CURRENT_BINARY_DIR})

# using Qt meta-system (precompiler)
if(MY_QT_MOC_HEADERS)
    QT4_WRAP_CPP(MY_QT_MOC_SOURCES ${MY_QT_MOC_HEADERS})
endif()

if(MY_QT_RESOURCES)
    QT4_ADD_RESOURCES(MY_QT_RC_SOURCES ${MY_QT_RESOURCES})
endif()

if(MY_QT_UI_FILES)
    QT4_WRAP_UI(MY_QT_UI_HEADERS ${MY_QT_UI_FILES})
endif()
