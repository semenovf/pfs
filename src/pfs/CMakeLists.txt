# Ubuntu 14.04 - VERSION 2.8.12
cmake_minimum_required (VERSION 2.8.12)

project(pfs C CXX)

include_directories(../include)
#    include("../../.gbs/solution-common.lua")
#
#-- Projects
#
#project "pfs"
#    kind "StaticLib"
set(PFS_TARGET_NAME pfs)
#    links { PFS_LINKS }
#
set(PFS_COMMON_SOURCES
#      "../src/pfs/*.cpp"
#    , "../src/pfs/io/*.cpp"
#    , "../src/pfs/db/*.c"
#    , "../src/pfs/db/*.cpp"
#    , "../src/pfs/application/*.cpp"
)

if (UNIX)
#filter "platforms:unix* or elbrus*"
#    files {
#          "../src/pfs/gnuc/*.cpp"
#        , "../src/pfs/posix/*.cpp"
#        , "../src/pfs/io/posix/*.cpp"
#        , "../src/pfs/application/posix/*.cpp"
#    }
endif(UNIX)

#--filter { "action:gmake*", "files:../src/pfs/db/sqlite3.c" }
#--    buildoptions { "-Wno-implicit-fallthrough" }
#filter { "action:gmake*", "files:**/sqlite3.c" }
#    buildoptions { "-Wno-implicit-fallthrough" }

add_library(${PFS_TARGET_NAME} STATIC ${PFS_COMMON_SOURCES} ${PFS_PLATFORM_SOURCES})
add_library(${PFS_TARGET_NAME} SHARED ${PFS_COMMON_SOURCES} ${PFS_PLATFORM_SOURCES})