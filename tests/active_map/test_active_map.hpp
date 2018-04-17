#pragma once
#include <pfs/test.hpp>
#include <pfs/thread.hpp>
#include <pfs/map.hpp>
#include <pfs/active_map.hpp>

namespace test {
namespace active_map {

namespace basic {

typedef pfs::active_map<int, void
    , pfs::map
    , pfs::mutex> active_map_type;

static int counter = 0;

void func ()
{
    ++counter;
}

void test ()
{
    ADD_TESTS(1);

    active_map_type m;

    int max_count = 100;

    for (int i = 0; i < max_count; ++i) {
        m.insert_function(i, & func);
    }

    for (int i = 0; i < max_count; ++i) {
        m.call(i);
    }

    TEST_OK(counter == max_count);
}

} // basic

}} //test::active_queue
