project(pfs C CXX)

include(../../solution.cmake)

if(NOT PFS_TARGET_SHARED_LIB_NAME)
    message(FATAL_ERROR "PFS_TARGET_SHARED_LIB_NAME must be specified")
endif()

if(NOT PFS_TARGET_STATIC_LIB_NAME)
    message(FATAL_ERROR "PFS_TARGET_STATIC_LIB_NAME must be specified")
endif()

set(PFS_LIB_COMMON_SOURCES
    crc32.cpp
    crc64.cpp
    date.cpp
    datetime.cpp
    double_to_cstr.cpp
    dynamic_library.cpp
    endian.cpp
    filesystem.cpp
    json.cpp
    lexical_cast.cpp
    string.cpp
    system_error.cpp
    time.cpp
    u8string.cpp

#   Db
    db/exception.cpp
    db/sqlite3.c

#   IO
    io/buffer.cpp
    io/buffered_device.cpp
    io/device.cpp
    io/exception.cpp
    io/file.cpp
    io/inet4_addr.cpp
    io/null.cpp
    io/server.cpp
)

if (UNIX)
    #filter "platforms:unix* or elbrus*"
    list(APPEND PFS_LIB_PLATFORM_SOURCES
        posix/date_posix.cpp
        posix/datetime_posix.cpp
        posix/dynamic_library_posix.cpp
        posix/time_posix.cpp
        app/posix/signal.cpp
        io/posix/file_posix.cpp
        io/posix/inet_server_posix.cpp
        io/posix/inet_socket_posix.cpp
        io/posix/posix_utils.cpp
    )

    if (CMAKE_COMPILER_IS_GNUCXX)
        list(APPEND PFS_LIB_PLATFORM_SOURCES gnuc/assert.cpp)
        list(APPEND PFS_LIB_PLATFORM_SOURCES gnuc/net/inet_addrinfo.cpp)
    endif()
endif(UNIX)

#--filter { "action:gmake*", "files:../src/pfs/db/sqlite3.c" }
#--    buildoptions { "-Wno-implicit-fallthrough" }
#filter { "action:gmake*", "files:**/sqlite3.c" }
#    buildoptions { "-Wno-implicit-fallthrough" }

# Set `pfs` library dependencies

set(PFS_TARGET_LIB_LINKS "")

if (HAVE_BOOST_FILESYSTEM)
    list(APPEND PFS_TARGET_LIB_LINKS "boost_filesystem")
endif()

if (HAVE_BOOST_THREAD)
    list(APPEND PFS_TARGET_LIB_LINKS "boost_thread")
endif()

if (HAVE_BOOST_CHRONO)
    list(APPEND PFS_TARGET_LIB_LINKS "boost_chrono")
endif()

if (HAVE_BOOST_REGEX)
    list(APPEND PFS_TARGET_LIB_LINKS "boost_regex")
endif()

if (HAVE_BOOST_SYSTEM_ERROR)
    list(APPEND PFS_TARGET_LIB_LINKS "boost_system")
endif()

if (HAVE_QT5_CORE)
#   TODO
elseif (HAVE_QT4_CORE)
    include(${QT_USE_FILE})
    add_definitions(${QT_DEFINITIONS})
    list(APPEND PFS_TARGET_LIB_LINKS ${QT_LIBRARIES})
endif()

if(UNIX)
    list(APPEND PFS_TARGET_LIB_LINKS "dl")

    if(HAVE_PTHREAD)
        list(APPEND PFS_TARGET_LIB_LINKS "pthread")
    endif()
endif()

if (HAVE_STDCXX_FS)
elseif (HAVE_STDCXX_FS_EXPERIMENTAL)
    list(APPEND PFS_TARGET_LIB_LINKS "stdc++fs")
endif()

# [Is it possible to get CMake to build both a static and shared version of the same library?](https://stackoverflow.com/questions/2152077/is-it-possible-to-get-cmake-to-build-both-a-static-and-shared-version-of-the-sam)
#===============================================================================
# this is the "object library" target: compiles the sources only once
add_library(PFS_LIB_OBJLIB OBJECT ${PFS_LIB_COMMON_SOURCES} ${PFS_LIB_PLATFORM_SOURCES})

# shared libraries need PIC
set_property(TARGET PFS_LIB_OBJLIB PROPERTY POSITION_INDEPENDENT_CODE 1)

# Use preferred standard
#set_property(TARGET PFS_LIB_OBJLIB PROPERTY CXX_STANDARD ${PFS_PREFERRED_CXX_STANDARD})

# shared and static libraries built from the same object files
add_library(${PFS_TARGET_SHARED_LIB_NAME} SHARED $<TARGET_OBJECTS:PFS_LIB_OBJLIB>)
add_library(${PFS_TARGET_STATIC_LIB_NAME} STATIC $<TARGET_OBJECTS:PFS_LIB_OBJLIB>)
target_link_libraries(${PFS_TARGET_SHARED_LIB_NAME} ${PFS_TARGET_LIB_LINKS})
target_link_libraries(${PFS_TARGET_STATIC_LIB_NAME} ${PFS_TARGET_LIB_LINKS})
#===============================================================================

