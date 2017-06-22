/* 
 * File:   test_constructor.hpp
 * Author: wladt
 *
 * Created on May 07, 2017
 */

#ifndef __PFS_TEST_CONSTRUCTOR_HPP__
#define __PFS_TEST_CONSTRUCTOR_HPP__

template <typename T, template <typename> class ContainerRef>
void test_container_basic ()
{
    ADD_TESTS(7);
    
    typedef pfs::traits::container<T, ContainerRef> container_traits;
    
    typename container_traits::native_type c1;
    typename container_traits::native_type c2;
    typename container_traits::native_type c3;
    
    // container (native_reference rhs)
    container_traits traits1(c1);
    container_traits traits2(c2);
    container_traits traits3 = c3;
    
    // container (container const & rhs)
    container_traits traits4(traits1);
    
    TEST_OK(traits3 == traits4); // both are empty
    
    // container & operator = (container const & rhs)
    traits4 = traits2;
    
    TEST_OK(traits3 == traits4); // both are empty too
    
    TEST_OK(traits1.size() == 0);
    TEST_OK(traits1.empty());
    TEST_OK(traits1.max_size() > 0);

    TEST_OK(traits1.begin() == traits1.end());
    TEST_OK(traits1.cbegin() == traits1.cend());
    
    // TODO
    //traits1.swap(traits2);
}

#endif /* __PFS_TEST_CONSTRUCTOR_HPP__ */
