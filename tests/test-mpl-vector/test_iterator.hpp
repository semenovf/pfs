/* 
 * File:   test_iterator.hpp
 * Author: wladt
 *
 * Created on January 19, 2017, 2:15 PM
 */

#ifndef __PFS_TEST_ITERATOR_HPP__
#define __PFS_TEST_ITERATOR_HPP__

namespace test_iterator_00 {

template <typename Foundation>
void test ()
{
    ADD_TESTS(4);
    
    typedef pfs::traits::vector<Foundation, int> vector_type;
    vector_type v;
    
    TEST_OK(v.begin()   == v.end());
    TEST_OK(v.cbegin()  == v.cend());
    TEST_OK(v.rbegin()  == v.rend());
    TEST_OK(v.crbegin() == v.crend());
    
//    for (int i = 0; i < 10; ++i)
//        l.push_back(i);
}

} // test_iterator_00

template <typename Foundation>
void test_iterator ()
{
    test_iterator_00::test<Foundation>();
}

#endif /* __PFS_TEST_ITERATOR_HPP__ */
