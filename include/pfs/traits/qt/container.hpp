/* 
 * File:   container.hpp
 * Author: wladt
 *
 * Created on May 8, 2017, 11:27 AM
 */

#ifndef __PFS_TRAITS_QT_CONTAINER_HPP__
#define __PFS_TRAITS_QT_CONTAINER_HPP__

#include <pfs/cxxlang.hpp>
#include <pfs/traits/value_ref.hpp>

namespace pfs {
namespace traits {
namespace qt {

template <typename T, typename ValueOrReference>
class container_basic
{
    typedef ValueOrReference internal_type;

public:
    typedef typename internal_type::container_value_type     container_value_type;
    typedef typename internal_type::container_reference_type container_reference_type;
    
    typedef typename internal_type::native_type            native_type;
    typedef typename internal_type::native_reference       native_reference;
    typedef typename internal_type::const_native_reference const_native_reference;
    
    typedef typename native_type::value_type       value_type;
    typedef typename native_type::const_pointer    const_pointer;
    typedef typename native_type::reference        reference;
    typedef typename native_type::const_reference  const_reference;
    typedef typename native_type::iterator         iterator;
    typedef typename native_type::const_iterator   const_iterator;
    typedef typename native_type::reverse_iterator reverse_iterator;
    typedef typename native_type::const_reverse_iterator const_reverse_iterator;
    typedef typename native_type::difference_type  difference_type;
    typedef typename native_type::size_type        size_type;

protected:
    internal_type _p;
    
//public:
//    typedef typename NativeTypeWrapperT<T>::type  native_type;
//    
//    // BEGIN Requirement typedefs for container traits
//    //
//    typedef native_type &                         native_reference;
//    typedef native_type const &                   const_native_reference;
//    typedef typename native_type::iterator        iterator;
//    typedef typename native_type::const_iterator  const_iterator;
//    typedef typename native_type::difference_type difference_type;
//    typedef typename native_type::size_type       size_type;
//    //
//    // END Requirement typedefs for container traits
//
//protected:
//    native_type * _p;
//    
public:
    container_basic (native_reference rhs)
        : _p(& rhs)
    {}
        
    container_basic & operator = (native_reference rhs)
    {
        *_p = rhs;
        return *this;
    }

    operator const_native_reference () const
    {
        return *_p;
    }

    /**
     * Casts to native container reference
     */
    operator native_reference ()
    {
        return *_p;
    }

    // *************************************************************************
    // BEGIN Requirements for container traits
    // Based on [C++ concepts: Container](http://en.cppreference.com/w/cpp/concept/Container)
    // *************************************************************************

    // *** ITERATORS ***
    //
    iterator begin ()
    {
        return _p->begin();
    }
    
    const_iterator begin () const
    {
        return _p->begin();
    }
    
    iterator end ()
    {
        return _p->end();
    }
    
    const_iterator end () const
    {
        return _p->end();
    }
    
    const_iterator cbegin () const
    {
        return _p->begin();
    }
    
    const_iterator cend () const
    {
        return _p->begin();
    }

    // *** CAPACITY ***
    // 

    size_type size () const pfs_noexcept
    {
        return _p->size();
    }
    
//    size_type max_size () const pfs_noexcept
//    {
//        return _p->max_size();
//    }
    
    bool empty () const pfs_noexcept
    {
        return _p->empty();
    }
    
    // *** MODIFIERS ***
    //
    // TODO
//    void swap (container & rhs)
//    {
//        _p->swap(*rhs._p);
//    }
    
    // *** NON-MEMBER FUNCTIONS (OPERATORS) ***
    //
    
    friend inline bool operator == (container_basic const & lhs
        , container_basic const & rhs)
    {
        return *lhs._p == *rhs._p;
    }

    friend inline bool operator != (container_basic const & lhs
        , container_basic const & rhs)
    {
        return *lhs._p != *rhs._p;
    }

    // *************************************************************************
    // } END Requirements for container traits
    // *************************************************************************
};

}}} // pfs::traits::stdcxx

#endif /* __PFS_TRAITS_QT_CONTAINER_HPP__ */

