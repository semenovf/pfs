project(pfs-demo-sfinae CXX)

include(${CMAKE_CURRENT_SOURCE_DIR}/../../solution.cmake)

set(PFS_DEMO_SOURCES main.cpp)

add_executable(pfs-demo-sfinae ${PFS_DEMO_SOURCES})
#target_link_libraries(pfs-demo-sfinae pfs)

