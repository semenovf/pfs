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
    
    typedef pfs::traits::list<Foundation, int> list_type;
    list_type l;
    
    TEST_OK(l.begin() == l.end());
    TEST_OK(l.cbegin() == l.cend());
    TEST_OK(l.rbegin() == l.rend());
    TEST_OK(l.crbegin() == l.crend());
    
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

