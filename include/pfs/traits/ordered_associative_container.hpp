/* 
 * File:   ordered_associative_container.hpp
 * Author: wladt
 *
 * Created on June 22, 2017, 1:24 PM
 */

#ifndef __PFS_TRAITS_ORDERED_ASSOCIATIVE_CONTAINER_HPP__
#define __PFS_TRAITS_ORDERED_ASSOCIATIVE_CONTAINER_HPP__

namespace pfs {
namespace traits {

template <typename T, template <typename> class ContainerRef>
class ordered_associative_container : public associative_container<T, ContainerRef>
{
    typedef associative_container<T, ContainerRef> base_class;
    
public:
    typedef ContainerRef<T>                           ref_type;
    typedef typename ref_type::native_reference       native_reference;
    
    typedef typename ref_type::key_type      key_type;
    typedef typename ref_type::key_compare   key_compare;
    typedef typename ref_type::value_compare value_compare;
    
public:
    ordered_associative_container ()
        : base_class()
    {}
    
    /**
     * Initializes with reference to native container
     */
    ordered_associative_container (native_reference rhs)
        : base_class(rhs)
    {}

    // *** OBSERVERS ***
    //
    
    key_compare key_comp () const
    {
        return base_class::_p.key_comp();
    }
    
    value_compare value_comp () const
    {
        return base_class::_p.value_comp();
    }
};

}} // pfs::traits

#endif /* __PFS_TRAITS_ORDERED_ASSOCIATIVE_CONTAINER_HPP__ */

