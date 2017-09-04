/* 
 * File:   test_active_map.hpp
 * Author: wladt
 *
 * Created on Aug 22, 2017
 */

#ifndef __TEST_ACTIVE_MAP_HPP__
#define __TEST_ACTIVE_MAP_HPP__

//#include <cstdlib>
#include <pfs/test/test.hpp>
//#include <pfs/types.hpp>
//#include <pfs/limits.hpp>
//#include <pfs/atomic.hpp>
#include <pfs/thread.hpp>
#include <pfs/traits/stdcxx/map.hpp>
#include <pfs/active_map.hpp>

namespace test {
namespace active_map {

namespace basic {

typedef pfs::active_map<int, void
    , pfs::stdcxx::map
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

#endif /* __TEST_ACTIVE_MAP_HPP__ */
