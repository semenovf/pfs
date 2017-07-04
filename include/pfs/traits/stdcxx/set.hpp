/* 
 * File:   set.hpp
 * Author: wladt
 *
 * Created on May 8, 2017
 */

#ifndef __PFS_TRAITS_STDCXX_SET_HPP__
#define __PFS_TRAITS_STDCXX_SET_HPP__

#include <set>
#include <pfs/traits/value_ref.hpp>

namespace pfs {
namespace traits {
namespace stdcxx {

template <typename T>
struct set_wrapper
{
    typedef std::set<T> type;
};

template <typename T, typename ValueOrReference>
class basic_set
{
    typedef ValueOrReference internal_type;

public:
    typedef basic_set<T, container_value<T, set_wrapper> >     container_value_type;
    typedef basic_set<T, container_reference<T, set_wrapper> > container_reference_type;
    
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
    
    typedef typename native_type::key_type         key_type;
    
    typedef typename native_type::key_compare      key_compare;
    typedef typename native_type::value_compare    value_compare;

protected:
    internal_type _p;
    
public:
    basic_set ()
    {}

    basic_set (native_reference rhs)
        : _p(rhs)
    {}

    basic_set (const_native_reference rhs)
        : _p(rhs)
    {}
    
//    set (const_pointer s)
//        : _p(s)
//    {}
//
//    template <typename InputIt>
//    basic_string (InputIt first, InputIt last)
//        : _p(first, last)
//    {}
    
    basic_set & operator = (native_reference rhs)
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
    
    void clear() pfs_noexcept
    {
        _p->clear();
    }
    
    std::pair<iterator,bool> insert (value_type const & value)
    {
        return _p->insert(value);
    }
    
    iterator erase (const_iterator pos)
    {
        return _p->erase(pos);
    }
    
//    iterator erase (iterator pos)
//    {
//        return _p->erase(pos);
//    }
    
    iterator erase (const_iterator first, const_iterator last)
    {
        return _p->erase(first, last);
    }
    
    // FIXME
//    void swap (container & rhs)
//    {
//        _p->swap(*rhs._p);
//    }
    
    // *** NON-MEMBER FUNCTIONS (OPERATORS) ***
    //
    
    friend inline bool operator == (basic_set const & lhs
        , basic_set const & rhs)
    {
        return *lhs._p == *rhs._p;
    }

    friend inline bool operator != (basic_set const & lhs
        , basic_set const & rhs)
    {
        return *lhs._p != *rhs._p;
    }

    // *************************************************************************
    // } END Requirements for container traits
    // *************************************************************************

    // *************************************************************************
    // BEGIN Requirements for associative container traits {
    // *************************************************************************

    // *** LOOKUP ***
    //
    
    iterator find (key_type const & key)
    {
        return _p->find(key);
    }
    
    const_iterator find (key_type const & key) const
    {
        return _p->find(key);
    }
    
    // *************************************************************************
    // } END Requirements for associative container traits
    // *************************************************************************
};

template <typename T>
class set : public basic_set<T, container_value<T, set_wrapper> >
{
    typedef basic_set<T, container_value<T, set_wrapper> > base_class;

public:
    typedef typename base_class::native_type            native_type;
    typedef typename base_class::native_reference       native_reference;
    typedef typename base_class::const_native_reference const_native_reference;
    
public:
    set ()
        : base_class()
    {}

    set (native_reference rhs)
        : base_class(rhs)
    {}

    set (const_native_reference rhs)
        : base_class(rhs)
    {}
};

template <typename T>
class set_reference : public basic_set<T, container_reference<T, set_wrapper> >
{
    typedef basic_set<T, container_reference<T, set_wrapper> > base_class;

public:
    typedef typename base_class::native_type            native_type;
    typedef typename base_class::native_reference       native_reference;
    typedef typename base_class::const_native_reference const_native_reference;
    
public:
    set_reference ()
        : base_class()
    {}

    set_reference (native_reference rhs)
        : base_class(rhs)
    {}

    set_reference (const_native_reference rhs)
        : base_class(rhs)
    {}
};

}}} // pfs::traits::stdcxx

#endif /* __PFS_TRAITS_STDCXX_SET_HPP__ */
