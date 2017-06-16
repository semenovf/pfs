/* 
 * File:   contigous_container.hpp
 * Author: wladt
 *
 * Created on May 5, 2017, 3:47 PM
 */

#ifndef __PFS_TRAITS_CONTIGOUS_CONTAINER_HPP__
#define __PFS_TRAITS_CONTIGOUS_CONTAINER_HPP__

//#include <pfs/cxxlang.hpp>
//#include <pfs/exception.hpp>
#include <pfs/traits/container.hpp>

namespace pfs {
namespace traits {

//template <typename T, template <typename> class ContainerReference>
//struct contigous_container_traits
//{
//    typedef ContainerReference                                  rep_type;
//    typedef typename RepType::size_type              size_type;
//    typedef typename RepType::native_reference       native_reference;
//    typedef typename RepType::const_native_reference const_native_reference;
//    typedef typename RepType::pointer                pointer;
//    typedef typename RepType::const_pointer          const_pointer;
//};

template <typename T, template <typename> class ContainerRef>
class contigous_container : public container<T, ContainerRef>
{
    typedef container<T, ContainerRef> base_class;
    
public:
    typedef typename base_class::size_type     size_type;
    typedef typename base_class::value_type    value_type;
    typedef typename base_class::pointer       pointer;
    typedef typename base_class::const_pointer const_pointer;
    
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
        if (new_cap > max_size())
            throw pfs::length_error("contigous_container::reserve()");
        base_class::_p.reserve(new_cap);
    }
    
    void shrink_to_fit ()
    {
        base_class::_p.shrink_to_fit();
    }
};

}}

#endif /* __PFS_TRAITS_CONTIGOUS_CONTAINER_HPP__ */

