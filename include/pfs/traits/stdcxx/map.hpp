/* 
 * File:   map.hpp
 * Author: wladt
 *
 * Created on January 16, 2017, 3:03 PM
 */

#ifndef __PFS_TRAITS_STDCXX_MAP_HPP__
#define __PFS_TRAITS_STDCXX_MAP_HPP__

#include <map>
#include <pfs/traits/value_ref.hpp>

namespace pfs {
namespace stdcxx {

template <typename KvType>
struct map_wrapper
{
    typedef std::map<typename KvType::key_type, typename KvType::mapped_type> type;
};

template <typename KvType, typename ValueOrReference>
class map_basic
{
    typedef ValueOrReference internal_type;

public:
    typedef map_basic<KvType, traits::container_value<KvType, map_wrapper> >     container_value_type;
    typedef map_basic<KvType, traits::container_reference<KvType, map_wrapper> > container_reference_type;
    
    typedef typename internal_type::native_type            native_type;
    typedef typename internal_type::native_reference       native_reference;
    typedef typename internal_type::const_native_reference const_native_reference;
    
    //typedef typename native_type::const_pointer    const_pointer;
    //typedef typename native_type::reference        reference;
    //typedef typename native_type::const_reference  const_reference;
    typedef typename native_type::iterator         iterator;
    typedef typename native_type::const_iterator   const_iterator;
    typedef typename native_type::reverse_iterator reverse_iterator;
    typedef typename native_type::const_reverse_iterator const_reverse_iterator;
    typedef typename native_type::difference_type  difference_type;
    typedef typename native_type::size_type        size_type;

    typedef typename native_type::key_type         key_type;
    typedef typename native_type::mapped_type      mapped_type;
    typedef typename native_type::value_type       value_type;
    
//    typedef typename native_type::key_compare      key_compare;
//    typedef typename native_type::value_compare    value_compare;

protected:
    internal_type _p;
    
public:
    map_basic ()
    {}

    map_basic (native_reference rhs)
        : _p(rhs)
    {}

    map_basic (const_native_reference rhs)
        : _p(rhs)
    {}
    
//    map (const_pointer s)
//        : _p(s)
//    {}
//
//    template <typename InputIt>
//    basic_string (InputIt first, InputIt last)
//        : _p(first, last)
//    {}
    
    map_basic & operator = (native_reference rhs)
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
        return _p->cend();
    }
#else
    const_iterator cbegin () const
    {
        return _p->begin();
    }
    
    const_iterator cend () const
    {
        return _p->end();
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
    
    void clear ()
    {
        _p->clear();
    }
    
    iterator erase (const_iterator pos)
    {
        return _p->erase(pos);
    }
    
    size_type erase (key_type const & key)
    {
        return _p->erase(key);
    }


    
    // FIXME
//    void swap (container & rhs)
//    {
//        _p->swap(*rhs._p);
//    }
    
    // *** NON-MEMBER FUNCTIONS (OPERATORS) ***
    //
    
    friend inline bool operator == (map_basic const & lhs
        , map_basic const & rhs)
    {
        return *lhs._p == *rhs._p;
    }

    friend inline bool operator != (map_basic const & lhs
        , map_basic const & rhs)
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
    
    // *** MODIFIERS
    //
    pfs::pair<iterator, bool> insert (key_type const & key, mapped_type const & value)
    {
        return _p->insert(pfs::make_pair(key, value));
    }

    // *************************************************************************
    // } END Requirements for associative container traits
    // *************************************************************************


    // *************************************************************************
    // BEGIN Requirements for ordered associative container traits {
    // *************************************************************************

    // *** OBSERVERS ***
    //
    
//    key_compare key_comp () const
//    {
//        return _p->key_comp();
//    }
//    
//    value_compare value_comp () const
//    {
//        return _p->value_comp();
//    }
    // *************************************************************************
    // } END Requirements for ordered associative container traits
    // *************************************************************************
    
    static key_type const & key (iterator it)
    {
        return it->first;
    }

    static mapped_type & mapped_reference (iterator it)
    {
        return it->second;
    }
    
    static mapped_type const & mapped_reference (const_iterator it)
    {
        return it->second;
    }
};

template <typename KvType>
class map : public map_basic<KvType, traits::container_value<KvType, map_wrapper> >
{
    typedef map_basic<KvType, traits::container_value<KvType, map_wrapper> > base_class;

public:
    typedef typename base_class::native_type            native_type;
    typedef typename base_class::native_reference       native_reference;
    typedef typename base_class::const_native_reference const_native_reference;

    typedef typename base_class::key_type               key_type;
    
public:
    map ()
        : base_class()
    {}

    map (native_reference rhs)
        : base_class(rhs)
    {}

    map (const_native_reference rhs)
        : base_class(rhs)
    {}
};

template <typename KvType>
class map_reference : public map_basic<KvType
        , traits::container_reference<KvType
        , map_wrapper> >
{
    typedef map_basic<KvType, traits::container_reference<KvType, map_wrapper> > base_class;

public:
    typedef typename base_class::native_type            native_type;
    typedef typename base_class::native_reference       native_reference;
    typedef typename base_class::const_native_reference const_native_reference;
    
    typedef typename base_class::key_type               key_type;
    
public:
    map_reference ()
        : base_class()
    {}

    map_reference (native_reference rhs)
        : base_class(rhs)
    {}

    map_reference (const_native_reference rhs)
        : base_class(rhs)
    {}
};

}} // pfs::stdcxx

#endif /* __PFS_TRAITS_STDCXX_MAP_HPP__ */
