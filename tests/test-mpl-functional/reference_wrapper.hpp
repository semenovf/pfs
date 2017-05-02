/* 
 * File:   reference_wrapper.hpp
 * Author: wladt
 *
 * Created on May 2, 2017, 4:25 PM
 */

#ifndef __TEST_REFERENCE_WRAPPER_HPP__
#define __TEST_REFERENCE_WRAPPER_HPP__

#include <vector>
#include <pfs/functional.hpp>
#include <pfs/numeric.hpp>

void test_reference_wrapper ()
{
    static int const SIZE = 10;
    
    ADD_TESTS(1);
    
    std::vector<int> orig(SIZE);
    pfs::iota(orig.begin(), orig.end(), -4);
    
    std::vector<pfs::reference_wrapper<int> > same(orig.begin(), orig.end());
    
    for (int i = 0; i < same.size(); i++)
        same[i] += 5;
    
    int i = 0;
    
    for (; i < SIZE; i++)
        if (orig[i] != same[i])
            break;
    
    TEST_OK2(i == SIZE, "test_reference_wrapper()");
}

#endif /* __TEST_REFERENCE_WRAPPER_HPP__ */
