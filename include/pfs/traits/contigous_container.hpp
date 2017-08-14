/* 
 * File:   contigous_container.hpp
 * Author: wladt
 *
 * Created on May 5, 2017, 3:47 PM
 */

#ifndef __PFS_TRAITS_CONTIGOUS_CONTAINER_HPP__
#define __PFS_TRAITS_CONTIGOUS_CONTAINER_HPP__

#include <pfs/cxxlang.hpp>
#include <pfs/exception.hpp>
#include <pfs/traits/container.hpp>

namespace pfs {
namespace traits {

template <typename T, template <typename> class ContainerValueRef>
class contigous_container : public container<T, ContainerValueRef>
{
    typedef container<T, ContainerValueRef> base_class;
protected:    
    typedef typename base_class::internal_type internal_type;
    
public:
    typedef typename base_class::native_type            native_type;
    typedef typename base_class::native_reference       native_reference;
    typedef typename base_class::const_native_reference const_native_reference;

    typedef typename base_class::size_type          size_type;
    typedef typename internal_type::value_type      value_type;
    typedef typename internal_type::reference       reference;
    typedef typename internal_type::const_reference const_reference;
    
    typedef typename native_type::pointer         pointer;
    typedef typename native_type::const_pointer   const_pointer;
    
public:
    contigous_container ()
    {}
    
    contigous_container (native_reference rhs)
        : base_class(rhs)
    {}

    pointer data ()
    {
        return base_class::_p.data();
    }
    
    const_pointer data () const
    {
        return base_class::_p.data();
    }
    
    void resize (size_type count)
    {
        base_class::_p.resize(count);
    }
    
    void resize (size_type count, value_type const & value)
    {
        base_class::_p.resize(count, value);
    }

    void reserve (size_type new_cap)
    {
        if (new_cap > base_class::max_size())
            throw pfs::length_error("contigous_container::reserve()");
        base_class::_p.reserve(new_cap);
    }
    
    void shrink_to_fit ()
    {
        base_class::_p.shrink_to_fit();
    }
    
    void push_back (const_reference value)
    {
        base_class::_p.push_back(value);
    }

    //
    // *** ELEMENT ACCESS
    //
    
    reference at (size_type pos)
    {
        return base_class::_p.at(pos);
    }
		
    const_reference at (size_type pos) const
    {
        return base_class::_p.at(pos);
    }
	
    reference operator [] (size_type pos)
    {
        return base_class::_p.operator[](pos);
    }
		
    const_reference operator [] (size_type pos) const
    {
        return base_class::_p.operator[](pos);
    }
};

}}

#endif /* __PFS_TRAITS_CONTIGOUS_CONTAINER_HPP__ */

