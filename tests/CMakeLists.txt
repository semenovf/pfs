project(pfs-unit-test CXX)

include(../solution.cmake)

add_definitions(-DPFS_TEST)

# Add unit test targets
list(APPEND PFS_TEST_TARGETS active_queue)
list(APPEND PFS_TEST_TARGETS active_map)
list(APPEND PFS_TEST_TARGETS algo-between)
list(APPEND PFS_TEST_TARGETS byte_string)
list(APPEND PFS_TEST_TARGETS chrono)
list(APPEND PFS_TEST_TARGETS container)
list(APPEND PFS_TEST_TARGETS date)
list(APPEND PFS_TEST_TARGETS db)
list(APPEND PFS_TEST_TARGETS dynamic_library)
list(APPEND PFS_TEST_TARGETS filesystem)
list(APPEND PFS_TEST_TARGETS fsm)
list(APPEND PFS_TEST_TARGETS json)
list(APPEND PFS_TEST_TARGETS io-buffer)
list(APPEND PFS_TEST_TARGETS io-file)
list(APPEND PFS_TEST_TARGETS io-server)
list(APPEND PFS_TEST_TARGETS iterator)
list(APPEND PFS_TEST_TARGETS lexical_cast)
list(APPEND PFS_TEST_TARGETS net-inet4_addr)
list(APPEND PFS_TEST_TARGETS net-uri)
list(APPEND PFS_TEST_TARGETS ratio)
list(APPEND PFS_TEST_TARGETS regex)
list(APPEND PFS_TEST_TARGETS safeformat)
list(APPEND PFS_TEST_TARGETS smart_ptr)
list(APPEND PFS_TEST_TARGETS string)
list(APPEND PFS_TEST_TARGETS time)
list(APPEND PFS_TEST_TARGETS utf8)

set(test-db_SOURCES
    db/test.cpp
    db/sqlite3.cpp)

set(test-io-server_SOURCES
    io-server/test.cpp
    io-server/test_pool_dispatcher.cpp)

set(test-utf8_SOURCES
    utf8/test.cpp
    utf8/cyrillic.c
    utf8/decode.cpp
    utf8/encode.cpp
    utf8/gothic.c
    utf8/greek.c
    utf8/iterator.cpp
    utf8/mideng.c
    utf8/midger.c
    utf8/multilang.c
    utf8/rune.c
    utf8/vietnamese.c)

# Porcess all unit test targets
foreach(PFS_TEST_TARGET ${PFS_TEST_TARGETS})
    if(test-${PFS_TEST_TARGET}_SOURCES)
        add_executable(test-${PFS_TEST_TARGET} ${test-${PFS_TEST_TARGET}_SOURCES})
    else()
        add_executable(test-${PFS_TEST_TARGET} ${PFS_TEST_TARGET}/test.cpp)
    endif()

    target_link_libraries(test-${PFS_TEST_TARGET} pfs-test pfs)
    set_property(TARGET test-${PFS_TEST_TARGET} PROPERTY CXX_STANDARD ${PFS_PREFERRED_CXX_STANDARD})
    add_test(NAME ${PFS_TEST_TARGET} COMMAND test-${PFS_TEST_TARGET})
endforeach(PFS_TEST_TARGET)

# Helper library for test `dynamic_library`
add_library(shared_object SHARED dynamic_library/shared_object.c)
set_property(TARGET shared_object PROPERTY CXX_STANDARD ${PFS_PREFERRED_CXX_STANDARD})
