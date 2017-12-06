project(pfs-test CXX)

include(../../solution.cmake)

set(PFS_TARGET_NAME pfs-test)
set(PFS_COMMON_SOURCES test.cpp)

if (UNIX)
    list(APPEND PFS_PLATFORM_SOURCES unix/profiler.cpp)
endif(UNIX)

add_library(${PFS_TARGET_NAME} SHARED ${PFS_COMMON_SOURCES} ${PFS_PLATFORM_SOURCES})
