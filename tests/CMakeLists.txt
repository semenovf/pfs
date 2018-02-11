project(pfs-unit-test CXX)

include(../solution.cmake)

add_definitions(-DPFS_TEST)

if (HAVE_QT5_CORE)
#   TODO
elseif (HAVE_QT4_CORE)
    include(${QT_USE_FILE})
    add_definitions(${QT_DEFINITIONS})
endif()

# Add unit test targets
list(APPEND MY_TEST_TARGETS active_queue)
list(APPEND MY_TEST_TARGETS active_map)
list(APPEND MY_TEST_TARGETS algo-between)
list(APPEND MY_TEST_TARGETS byte_string)
list(APPEND MY_TEST_TARGETS chrono)
list(APPEND MY_TEST_TARGETS command)
list(APPEND MY_TEST_TARGETS container)
list(APPEND MY_TEST_TARGETS date)
list(APPEND MY_TEST_TARGETS db)
list(APPEND MY_TEST_TARGETS dynamic_library)
list(APPEND MY_TEST_TARGETS filesystem)
list(APPEND MY_TEST_TARGETS fsm)
list(APPEND MY_TEST_TARGETS json)
list(APPEND MY_TEST_TARGETS json-rpc)
list(APPEND MY_TEST_TARGETS io-buffer)
list(APPEND MY_TEST_TARGETS io-file)
list(APPEND MY_TEST_TARGETS io-server)
list(APPEND MY_TEST_TARGETS io-device_manager)
list(APPEND MY_TEST_TARGETS iterator)
list(APPEND MY_TEST_TARGETS lexical_cast)
list(APPEND MY_TEST_TARGETS net-basic)
list(APPEND MY_TEST_TARGETS net-uri)
list(APPEND MY_TEST_TARGETS ratio)
list(APPEND MY_TEST_TARGETS regex)
list(APPEND MY_TEST_TARGETS safeformat)
list(APPEND MY_TEST_TARGETS smart_ptr)
list(APPEND MY_TEST_TARGETS string)
list(APPEND MY_TEST_TARGETS time)
list(APPEND MY_TEST_TARGETS utf8)
list(APPEND MY_TEST_TARGETS algo-tricks)

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
foreach(MY_TEST_TARGET ${MY_TEST_TARGETS})
    if(test-${MY_TEST_TARGET}_SOURCES)
        add_executable(test-${MY_TEST_TARGET} ${test-${MY_TEST_TARGET}_SOURCES})
    else()
        add_executable(test-${MY_TEST_TARGET} ${MY_TEST_TARGET}/test.cpp)
    endif()

    target_link_libraries(test-${MY_TEST_TARGET} pfs-test pfs)
    #set_property(TARGET test-${MY_TEST_TARGET} PROPERTY CXX_STANDARD ${PFS_PREFERRED_CXX_STANDARD})
    add_test(NAME ${MY_TEST_TARGET} COMMAND test-${MY_TEST_TARGET})
endforeach(MY_TEST_TARGET)

# Helper library for test `dynamic_library`
add_library(shared_object SHARED dynamic_library/shared_object.c)
#set_property(TARGET shared_object PROPERTY CXX_STANDARD ${PFS_PREFERRED_CXX_STANDARD})
