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
    contigous_container (native_reference rhs)
        : _p(rhs)
    {}

    pointer data ()
    {
        return _p.data();
    }
    
    const_pointer data () const
    {
        return _p->data();
    }
    
    void resize (size_type count)
    {
        _p.resize(count);
    }
    
    void resize (size_type count, value_type const & value)
    {
        _p.resize(count, value);
    }

    void reserve (size_type new_cap)
    {
        if (new_cap > max_size())
            throw pfs::length_error("contigous_container::reserve()");
        _preserve(new_cap);
    }
    
    void shrink_to_fit ()
    {
        _p.shrink_to_fit();
    }
};

}}

#endif /* __PFS_TRAITS_CONTIGOUS_CONTAINER_HPP__ */

