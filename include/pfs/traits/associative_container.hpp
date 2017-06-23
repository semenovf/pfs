/* 
 * File:   associative_container.hpp
 * Author: wladt
 *
 * Created on June 22, 2017, 11:59 AM
 */

#ifndef __PFS_TRAITS_ASSOCIATIVE_CONTAINER_HPP__
#define __PFS_TRAITS_ASSOCIATIVE_CONTAINER_HPP__

#include <pfs/utility.hpp>
#include <pfs/traits/container.hpp>

namespace pfs {
namespace traits {

template <typename Key, typename T>
struct kv_type
{
    typedef Key key_type;
    typedef T   mapped_type;
};

// AssociativeContainer traits.
// Based on [C++ concepts: AssociativeContainer](http://en.cppreference.com/w/cpp/concept/AssociativeContainer)

template <typename KvType, template <typename> class ContainerValueRef>
class associative_container : public container<KvType, ContainerValueRef>
{
protected:    
    typedef container<KvType, ContainerValueRef> base_class;
    typedef typename base_class::internal_type internal_type;
    
public:
    typedef typename base_class::native_type            native_type;
    typedef typename base_class::native_reference       native_reference;
    typedef typename base_class::const_native_reference const_native_reference;
    typedef typename base_class::iterator               iterator;
    typedef typename base_class::const_iterator         const_iterator;

    typedef typename native_type::key_type    key_type;
    typedef typename native_type::value_type  value_type;
    typedef typename native_type::mapped_type mapped_type;
    
public:
    associative_container ()
        : base_class()
    {}
    
    /**
     * Initializes with reference to native container
     */
    associative_container (native_reference rhs)
        : base_class(rhs)
    {}

    // *** LOOKUP ***
    //
    
    iterator find (key_type const & key)
    {
        return base_class::_p.find(key);
    }
    
    const_iterator find (key_type const & key) const
    {
        return base_class::_p.find(key);
    }
    
    // *** MODIFIERS
    //
    pfs::pair<iterator, bool> insert (value_type const & value)
    {
        return base_class::_p.insert(value);
    }
};

}} // pfs::traits

#endif /* __PFS_TRAITS_ASSOCIATIVE_CONTAINER_HPP__ */

