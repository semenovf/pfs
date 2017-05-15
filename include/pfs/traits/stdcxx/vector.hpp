/* 
 * File:   vector.hpp
 * Author: wladt
 *
 * Created on April 27, 2017, 7:17 PM
 */

#ifndef __PFS_TRAITS_STDCXX_VECTOR_HPP__
#define __PFS_TRAITS_STDCXX_VECTOR_HPP__

#include <vector>
#include <pfs/traits/value_ref.hpp>

namespace pfs {
namespace traits {
namespace stdcxx {

template <typename T>
struct vector_wrapper
{
    typedef std::vector<T> type;
};

template <typename T, typename ValueOrReference>
class vector_basic
{
    typedef ValueOrReference internal_type;

public:
    typedef vector_basic<T, container_value<T, vector_wrapper> > container_value_type;
    typedef vector_basic<T, container_ref<T, vector_wrapper> >   container_reference_type;
    
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
    
public:
    vector_basic ()
    {}

    vector_basic (native_reference rhs)
        : _p(rhs)
    {}

    vector_basic (const_native_reference rhs)
        : _p(rhs)
    {}
    
//    vector (const_pointer s)
//        : _p(s)
//    {}
//
//    template <typename InputIt>
//    basic_string (InputIt first, InputIt last)
//        : _p(first, last)
//    {}
    
    vector_basic & operator = (native_reference rhs)
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
    
#if __cplusplus >= 201103L
    const_iterator cbegin () const
    {
        return _p->cbegin();
    }
    
    const_iterator cend () const
    {
        return _p->cbegin();
    }
#else
    const_iterator cbegin () const
    {
        return _p->begin();
    }
    
    const_iterator cend () const
    {
        return _p->begin();
    }
#endif    

    // *** CAPACITY ***
    // 

    size_type size () const pfs_noexcept
    {
        return _p->size();
    }
    
    size_type max_size () const pfs_noexcept
    {
        return _p->max_size();
    }
    
    bool empty () const pfs_noexcept
    {
        return _p->empty();
    }
    
    // *** MODIFIERS ***
    //
    
    // FIXME
//    void swap (container & rhs)
//    {
//        _p->swap(*rhs._p);
//    }
    
    // *** NON-MEMBER FUNCTIONS (OPERATORS) ***
    //
    
    friend inline bool operator == (vector_basic const & lhs
        , vector_basic const & rhs)
    {
        return *lhs._p == *rhs._p;
    }

    friend inline bool operator != (vector_basic const & lhs
        , vector_basic const & rhs)
    {
        return *lhs._p != *rhs._p;
    }

    // *************************************************************************
    // } END Requirements for container traits
    // *************************************************************************
};

template <typename T>
class vector : public vector_basic<T, container_value<T, vector_wrapper> >
{
    typedef vector_basic<T, container_value<T, vector_wrapper> > base_class;

public:
    typedef typename base_class::native_type            native_type;
    typedef typename base_class::native_reference       native_reference;
    typedef typename base_class::const_native_reference const_native_reference;
    
public:
    vector ()
        : base_class()
    {}

    vector (native_reference rhs)
        : base_class(rhs)
    {}

    vector (const_native_reference rhs)
        : base_class(rhs)
    {}
};

template <typename T>
class vector_reference : public vector_basic<T, container_ref<T, vector_wrapper> >
{
    typedef vector_basic<T, container_ref<T, vector_wrapper> > base_class;

public:
    typedef typename base_class::native_type            native_type;
    typedef typename base_class::native_reference       native_reference;
    typedef typename base_class::const_native_reference const_native_reference;
    
public:
    vector_reference ()
        : base_class()
    {}

    vector_reference (native_reference rhs)
        : base_class(rhs)
    {}

    vector_reference (const_native_reference rhs)
        : base_class(rhs)
    {}
};

}}} // pfs::traits::stdcxx

#endif /* __PFS_TRAITS_STDCXX_VECTOR_HPP__ */

