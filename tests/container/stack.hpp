#ifndef TEST_STACK_HPP
#define TEST_STACK_HPP

#include "pfs/stack.hpp"

template <typename T, template <typename> class ContainerValueRef>
void test_stack ()
{
    ADD_TESTS(0);

//    typedef pfs::traits::stack<T, ContainerValueRef> stack_traits;
//    typedef typename stack_traits::value_type        value_type;
    
//    typename container_traits::native_type c1;
//    
//    // container (native_reference rhs)
//    container_traits traits1(c1);
//   
//    key_type key = 1;
//    value_type value = pfs::make_pair(key, 1);
//    
//    pfs::pair<iterator, bool> result = traits1.insert(value);
//    
//    mapped_type & ref = result.first->second;
}


#endif /* TEST_STACK_HPP */

