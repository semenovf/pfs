project(pfs-unit-test CXX)

add_definitions(-DPFS_TEST)

if (HAVE_QT5_CORE)
#   TODO
elseif (HAVE_QT4_CORE)
    include(${QT_USE_FILE})
    add_definitions(${QT_DEFINITIONS})
endif()

# This is the "object library" target: compiles the sources only once
add_library(CATCH_OBJLIB OBJECT catch-main.cpp)
# Optional: shared libraries need PIC
set_property(TARGET CATCH_OBJLIB PROPERTY POSITION_INDEPENDENT_CODE 1)

#
# Add unit test targets
#
list(APPEND MY_TEST_TARGETS static_cast)
list(APPEND MY_TEST_TARGETS active_queue)
list(APPEND MY_TEST_TARGETS active_map)
list(APPEND MY_TEST_TARGETS algo-between)
list(APPEND MY_TEST_TARGETS algorithm)
list(APPEND MY_TEST_TARGETS base64)
list(APPEND MY_TEST_TARGETS binary_stream)
list(APPEND MY_TEST_TARGETS byte_string)
list(APPEND MY_TEST_TARGETS chrono)
list(APPEND MY_TEST_TARGETS command)
list(APPEND MY_TEST_TARGETS date)
list(APPEND MY_TEST_TARGETS datetime)
list(APPEND MY_TEST_TARGETS deque)
list(APPEND MY_TEST_TARGETS dynamic_library)
#list(APPEND MY_TEST_TARGETS filesystem) -- Fixme
list(APPEND MY_TEST_TARGETS fsm)
list(APPEND MY_TEST_TARGETS functional)
list(APPEND MY_TEST_TARGETS json)
list(APPEND MY_TEST_TARGETS io-buffer)
list(APPEND MY_TEST_TARGETS io-buffered_device)
list(APPEND MY_TEST_TARGETS io-file)
list(APPEND MY_TEST_TARGETS io-device_manager)
list(APPEND MY_TEST_TARGETS io-device_notifier_pool)
list(APPEND MY_TEST_TARGETS integral)
list(APPEND MY_TEST_TARGETS iterator)
list(APPEND MY_TEST_TARGETS list)
list(APPEND MY_TEST_TARGETS map)
list(APPEND MY_TEST_TARGETS math)
list(APPEND MY_TEST_TARGETS modulus)
list(APPEND MY_TEST_TARGETS net-basic)
list(APPEND MY_TEST_TARGETS net-uri)
list(APPEND MY_TEST_TARGETS numeric)
#list(APPEND MY_TEST_TARGETS optional) -- Experimental yet
list(APPEND MY_TEST_TARGETS ratio)
list(APPEND MY_TEST_TARGETS rational)
list(APPEND MY_TEST_TARGETS real)
list(APPEND MY_TEST_TARGETS regex)
#list(APPEND MY_TEST_TARGETS rpc)
list(APPEND MY_TEST_TARGETS safeformat)
list(APPEND MY_TEST_TARGETS set)
list(APPEND MY_TEST_TARGETS sigslot)
list(APPEND MY_TEST_TARGETS smart_ptr)
list(APPEND MY_TEST_TARGETS sql)
list(APPEND MY_TEST_TARGETS stack)
list(APPEND MY_TEST_TARGETS string)
list(APPEND MY_TEST_TARGETS time)
list(APPEND MY_TEST_TARGETS tuple)
list(APPEND MY_TEST_TARGETS utf8)
list(APPEND MY_TEST_TARGETS algo-tricks)
list(APPEND MY_TEST_TARGETS variadic-templates)
list(APPEND MY_TEST_TARGETS variant)
list(APPEND MY_TEST_TARGETS vector)

################################################################################
# SQL test specific configuration                                              #
################################################################################
#set(test-sql_SOURCES
#    sql/test.cpp
#    sql/sqlite3.cpp)

if (HAVE_POSTGRESQL)
    message("PostgrSQL support activated ('libpq-fe.h' found at '${PostgreSQL_INCLUDE_DIRS}')")
    set(test-sql_LIBS ${PostgreSQL_LIBRARIES})
    include_directories(${PostgreSQL_INCLUDE_DIRS})
else()
    message("PostgrSQL support is inactive")
endif()

################################################################################

set(test-integral_SOURCES
    integral/test.cpp
    integral/parse_integral_part.cpp
    integral/to_integral.cpp)

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

add_library(catch SHARED $<TARGET_OBJECTS:CATCH_OBJLIB>)

#
# Process all unit test targets
#
foreach (MY_TEST_TARGET ${MY_TEST_TARGETS})
    if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/bunch/${MY_TEST_TARGET}.cpp")
        add_executable(test-${MY_TEST_TARGET} "${CMAKE_CURRENT_SOURCE_DIR}/bunch/${MY_TEST_TARGET}.cpp")
    elseif (test-${MY_TEST_TARGET}_SOURCES)
        add_executable(test-${MY_TEST_TARGET} ${test-${MY_TEST_TARGET}_SOURCES})
    else()
        add_executable(test-${MY_TEST_TARGET} ${MY_TEST_TARGET}/test.cpp)
    endif()

    if(test-${MY_TEST_TARGET}_LIBS)
        target_link_libraries(test-${MY_TEST_TARGET} catch pfs ${test-${MY_TEST_TARGET}_LIBS})
    else()
        target_link_libraries(test-${MY_TEST_TARGET} catch pfs)
    endif()

    #set_property(TARGET test-${MY_TEST_TARGET} PROPERTY CXX_STANDARD ${PFS_PREFERRED_CXX_STANDARD})
    add_test(NAME ${MY_TEST_TARGET} COMMAND test-${MY_TEST_TARGET})
endforeach(MY_TEST_TARGET)

# Helper library for test `dynamic_library`
add_library(shared_object SHARED dynamic_library/shared_object.c)
#set_property(TARGET shared_object PROPERTY CXX_STANDARD ${PFS_PREFERRED_CXX_STANDARD})
