/* 
 * File:   iota.hpp
 * Author: wladt
 *
 * Created on May 2, 2017, 1:25 PM
 */

#ifndef __TEST_IOTA_HPP__
#define __TEST_IOTA_HPP__

#include <pfs/numeric.hpp>

void test_iota ()
{
    static int const SIZE = 10;
    int ints[SIZE];
    
    ADD_TESTS(1);
    
    pfs::iota(ints, ints + SIZE, 0);
    
    int i = 0;
    
    for (; i < SIZE; ++i)
        if (ints[i] != i)
            break;
    
    TEST_OK2(i == SIZE, "test_iota()");
}

#endif /* __TEST_IOTA_HPP__ */

