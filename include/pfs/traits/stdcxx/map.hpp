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
namespace traits {
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
    typedef map_basic<KvType, container_value<KvType, map_wrapper> >     container_value_type;
    typedef map_basic<KvType, container_reference<KvType, map_wrapper> > container_reference_type;
    
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
    typedef typename native_type::mapped_type      mapped_type;
    
    typedef typename native_type::key_compare      key_compare;
    typedef typename native_type::value_compare    value_compare;

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
    pfs::pair<iterator, bool> insert (value_type const & value)
    {
        return _p->insert(value);
    }

    pfs::pair<iterator, bool> insert2 (key_type const & key, mapped_type const & value)
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
    
    key_compare key_comp () const
    {
        return _p->key_comp();
    }
    
    value_compare value_comp () const
    {
        return _p->value_comp();
    }
    // *************************************************************************
    // } END Requirements for ordered associative container traits
    // *************************************************************************
};

template <typename KvType>
class map : public map_basic<KvType, container_value<KvType, map_wrapper> >
{
    typedef map_basic<KvType, container_value<KvType, map_wrapper> > base_class;

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
class map_reference : public map_basic<KvType, container_reference<KvType, map_wrapper> >
{
    typedef map_basic<KvType, container_reference<KvType, map_wrapper> > base_class;

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

}}} // pfs::traits::stdcxx

#if __OBSOLETE__

#include <pfs/traits/map.hpp>

namespace stdcxx {

template <typename Key, typename T>
struct map
{
    typedef std::map<Key, T> type;
};

} // stdcxx

namespace pfs {
namespace traits {

namespace stdcxx {

template <typename Key, typename T>
struct map_iterator : public ::stdcxx::map<Key, T>::type::iterator
{
    typedef typename ::stdcxx::map<Key, T>::type::iterator base_class;

    map_iterator () pfs_noexcept
        : base_class()
    {}

    map_iterator (base_class lhs) pfs_noexcept
        : base_class(lhs)
    {}
    
    Key const & key () const
    {
        return base_class::operator->()->first;
    }
    
    T &	value () const
    {
        return base_class::operator->()->second;
    }
};

template <typename Key, typename T>
struct map_const_iterator : public ::stdcxx::map<Key, T>::type::const_iterator
{
    typedef typename ::stdcxx::map<Key, T>::type::const_iterator base_type;

    map_const_iterator () pfs_noexcept
        : base_type()
    {}

    map_const_iterator (base_type lhs) pfs_noexcept
        : base_type(lhs)
    {}
    
    Key const & key () const
    {
        return base_type::operator->()->first;
    }
    
    T const & value () const
    {
        return base_type::operator->()->second;
    }
};

} // stdcxx

template <typename Key, typename T>
struct map_rep<Key, T, ::stdcxx::map> : public ::stdcxx::map<Key, T>::type
{
    typedef typename ::stdcxx::map<Key, T>::type base_class;
    typedef typename ::stdcxx::map<Key, T>::type native_type;

    typedef native_type const &                            const_native_reference;
    typedef typename native_type::size_type                size_type;
    typedef typename native_type::key_type                 key_type;
    typedef typename native_type::mapped_type              mapped_type;
    typedef typename stdcxx::map_iterator<Key, T>          iterator;
    typedef typename stdcxx::map_const_iterator<Key, T>    const_iterator;
    typedef typename std::reverse_iterator<iterator>       reverse_iterator;
    typedef typename std::reverse_iterator<const_iterator> const_reverse_iterator;
    
//    typedef typename native_type::iterator               iterator;
//    typedef typename native_type::const_iterator         const_iterator;
//    typedef typename native_type::reverse_iterator       reverse_iterator;
//    typedef typename native_type::const_reverse_iterator const_reverse_iterator;
    
	explicit map_rep ()
		: base_class()
	{}

    map_rep (const_native_reference rhs)
        : base_class(rhs)
    {}

    iterator erase (iterator position)
    {
#if __cplusplus >= 201103L
        return base_class::erase(position);
#else
        base_class::erase(position++);
        return position;
#endif        
    }

    iterator erase (iterator first, iterator last)
    {
#if __cplusplus >= 201103L
        return base_class::erase(first, last);
#else
        base_class::erase(first, last);
        return last;
#endif        
    }
};

}} // pfs::traits

//namespace pfs {
//namespace traits {
//
//template <typename Key, typename T>
//class basic_map<Key, T, ::stdcxx::map>
//    : public details::basic_map<Key, T, ::stdcxx::map>
//{
//    virtual mapped_type & xat (Key const & key)
//    {
//        // Note: pfs::out_of_range is synonym for std::out_of_range
//        return this->_d.at(key);
//    }
//
//    virtual mapped_type const & xat (Key const & key) const
//    {
//        // Note: pfs::out_of_range is synonym for std::out_of_range
//        return this->_d.at(key);
//    }
//    
//    virtual mapped_type & xsubscript (Key const & key)
//    {
//        return this->_d[key];
//    }
//    
//    virtual pfs::pair<iterator, bool> xinsert (Key const & key, T const & value)
//    {
//        std::pair<iterator,bool> r = this->_d.insert(std::pair<Key, T>(key, value));
//        return pfs::pair<iterator,bool>(r.first, r.second);
//    }
//};
//
//}} // pfs::traits

#endif

#endif /* __PFS_TRAITS_STDCXX_MAP_HPP__ */
