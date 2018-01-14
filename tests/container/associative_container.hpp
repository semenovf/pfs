#ifndef TEST_ASSOCIATIVE_CONTAINER_HPP
#define TEST_ASSOCIATIVE_CONTAINER_HPP

#include "pfs/utility.hpp"
#include "pfs/traits/associative_container.hpp"

template <typename T, template <typename> class ContainerValueRef>
void test_associative_container_coverage ()
{
    ADD_TESTS(0);
    
    typedef pfs::traits::associative_container<T, ContainerValueRef> container_traits;
    typedef typename container_traits::mapped_type mapped_type;
    typedef typename container_traits::iterator    iterator;
    
    typename container_traits::native_type c1;
    
    // container (native_reference rhs)
    container_traits traits1(c1);
   
    pfs::pair<iterator, bool> result = traits1.insert(1 ,1);
    
    mapped_type & ref = result.first->second;
    PFS_UNUSED(ref);
}

#endif /* TEST_ASSOCIATIVE_CONTAINER_HPP */
