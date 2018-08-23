project(pfs-demo-logger CXX)

include(${CMAKE_CURRENT_SOURCE_DIR}/../../solution.cmake)

set(PFS_DEMO_SOURCES main.cpp)

add_executable(pfs-demo-logger ${PFS_DEMO_SOURCES})
target_link_libraries(pfs-demo-logger pfs)

