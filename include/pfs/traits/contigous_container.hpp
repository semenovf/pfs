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

namespace pfs {
namespace traits {

template <typename T, template <typename> class RepType>
struct contigous_container_traits
{
    typedef RepType                                  rep_type;
    typedef typename RepType::size_type              size_type;
    typedef typename RepType::native_reference       native_reference;
    typedef typename RepType::const_native_reference const_native_reference;
    typedef typename RepType::pointer                pointer;
    typedef typename RepType::const_pointer          const_pointer;
};

template <typename T, template <typename> class RepType>
class contigous_container : public container
{
public:
    typedef contigous_container_traits<T, RepType>       traits_type;
    typedef typename traits_type::rep_type               rep_type;
    typedef typename traits_type::size_type              size_type;
    typedef typename traits_type::native_reference       native_reference;
    typedef typename traits_type::const_native_reference const_native_reference;
    typedef typename traits_type::pointer                pointer;
    typedef typename traits_type::const_pointer          const_pointer;
    
private:
    rep_type * _p;

public:
    contigous_container (contigous_container const & rhs)
        : _p(rhs._p)
    {}

    contigous_container & operator = (contigous_container const & rhs)
    {
        _p = rhs._p;
    }

    contigous_container (native_reference rhs)
        : _p(& rhs)
    {}
    
    operator const_native_reference () const
    {
        return *_p;
    }

    operator native_reference ()
    {
        return *_p;
    }
    
    pointer data ()
    {
        return _p->data();
    }
    
    const_pointer data () const
    {
        return _p->data();
    }
    
    size_type size () const
    {
        return _p->size();
    }
    
 	size_type max_size () const
    {
        return _p->max_size();
    }

    size_type capacity () const
    {
        return _p->capacity();
    }

    void resize (size_type count)
    {
        _p->resize(count);
    }
    
    void resize (size_type count, value_type const & value)
    {
        _p->resize(count, value);
    }

    void reserve (size_type new_cap)
    {
        if (new_cap > max_size())
            throw pfs::length_error("contigous_container::reserve()");
        _p->reserve(new_cap);
    }
    
    void shrink_to_fit ()
    {
        _p->shrink_to_fit();
    }
};

}}

#endif /* __PFS_TRAITS_CONTIGOUS_CONTAINER_HPP__ */

