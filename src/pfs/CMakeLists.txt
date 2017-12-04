project(pfs C CXX)

include(../../solution.cmake)

#
#-- Projects
#
set(PFS_TARGET_NAME pfs)

set(PFS_COMMON_SOURCES
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

#   Application
    application/app.cpp
    application/dispatcher.cpp

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
    list(APPEND PFS_PLATFORM_SOURCES
        posix/date_posix.cpp
        posix/datetime_posix.cpp
        posix/dynamic_library_posix.cpp
        posix/time_posix.cpp
        application/posix/signal.cpp
        io/posix/file_posix.cpp
        io/posix/inet_server_posix.cpp
        io/posix/inet_socket_posix.cpp
        io/posix/posix_utils.cpp
    )

    if (CMAKE_COMPILER_IS_GNUCXX)
        list(APPEND PFS_PLATFORM_SOURCES gnuc/assert.cpp)
    endif()
endif(UNIX)

#--filter { "action:gmake*", "files:../src/pfs/db/sqlite3.c" }
#--    buildoptions { "-Wno-implicit-fallthrough" }
#filter { "action:gmake*", "files:**/sqlite3.c" }
#    buildoptions { "-Wno-implicit-fallthrough" }

# [Is it possible to get CMake to build both a static and shared version of the same library?](https://stackoverflow.com/questions/2152077/is-it-possible-to-get-cmake-to-build-both-a-static-and-shared-version-of-the-sam)
#===============================================================================
# this is the "object library" target: compiles the sources only once
add_library(PFS_OBJLIB OBJECT ${PFS_COMMON_SOURCES} ${PFS_PLATFORM_SOURCES})

# shared libraries need PIC
set_property(TARGET PFS_OBJLIB PROPERTY POSITION_INDEPENDENT_CODE 1)

# Use C++98 standard
set_property(TARGET PFS_OBJLIB PROPERTY CXX_STANDARD ${PFS_PREFERRED_CXX_STANDARD})

# shared and static libraries built from the same object files
add_library(${PFS_TARGET_NAME}        SHARED $<TARGET_OBJECTS:PFS_OBJLIB>)
add_library(${PFS_TARGET_NAME}_static STATIC $<TARGET_OBJECTS:PFS_OBJLIB>)
#===============================================================================

