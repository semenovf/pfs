#
# [CMake Manual](http://doc.qt.io/qt-5/cmake-manual.html)
#

if(MY_QT_REQUIRED)
    foreach(module ${MY_QT_REQUIRED})
        find_package(Qt5Core REQUIRED)
        add_definitions(${Qt5Core_DEFINITIONS})
        include_directories(${Qt5Core_INCLUDE_DIRS})
        list(APPEND QT_LIBRARIES ${Qt5Core_LIBRARIES})

        set(HAVE_QT5_CORE ${Qt5Core_FOUND})
        set(HAVE_QT_CORE ${Qt5Core_FOUND})

        if(${module} STREQUAL "QtGui")
            find_package(Qt5Widgets REQUIRED)
            add_definitions(${Qt5Widgets_DEFINITIONS})
            include_directories(${Qt5Widgets_INCLUDE_DIRS})
            list(APPEND QT_LIBRARIES ${Qt5Widgets_LIBRARIES})
        elseif(${module} STREQUAL "QtCore")
            # Already found
        else()
            find_package(${module} REQUIRED)
            add_definitions(${${module}_DEFINITIONS})
            include_directories(${${module}_INCLUDE_DIRS})
            list(APPEND QT_LIBRARIES ${${module}_LIBRARIES})
        endif()
    endforeach()
endif()

# using Qt meta-system (precompiler)
if(MY_QT_MOC_HEADERS)
    qt5_wrap_cpp(MY_QT_MOC_SOURCES ${MY_QT_MOC_HEADERS})
endif()

if(MY_QT_RESOURCES)
    qt5_add_resources(MY_QT_RC_SOURCES ${MY_QT_RESOURCES})
endif()

if(MY_QT_UI_FILES)
    qt5_wrap_ui(MY_QT_UI_HEADERS ${MY_QT_UI_FILES})
endif()
