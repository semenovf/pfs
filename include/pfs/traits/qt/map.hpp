/* 
 * File:   map.hpp
 * Author: wladt
 *
 * Created on May 8, 2017
 */

#ifndef __PFS_TRAITS_QT_MAP_HPP__
#define __PFS_TRAITS_QT_MAP_HPP__

#include <QMap>
#include <pfs/traits/value_ref.hpp>

namespace pfs {
namespace qt {

template <typename KvType>
struct map_wrapper
{
    typedef QMap<typename KvType::key_type, typename KvType::mapped_type> type;
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
    //typedef typename native_type::reverse_iterator reverse_iterator;
    //typedef typename native_type::const_reverse_iterator const_reverse_iterator;
    typedef typename native_type::difference_type  difference_type;
    typedef typename native_type::size_type        size_type;

    typedef typename native_type::key_type         key_type;
    typedef typename native_type::mapped_type      mapped_type;
    typedef pair<key_type, mapped_type>            value_type;
    
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
//    pfs::pair<iterator, bool> insert (value_type const & value)
//    {
//        return _p->insert(value);
//    }

    pfs::pair<iterator, bool> insert (key_type const & key, mapped_type const & value)
    {
        iterator it = iterator(_p->insert(key, value));
        return pfs::pair<iterator, bool>(it, true);
//        return _p->insert(key, value);
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
    
//    value_compare value_comp () const
//    {
//        return _p->value_comp();
//    }
    // *************************************************************************
    // } END Requirements for ordered associative container traits
    // *************************************************************************
    
    static key_type const & key (iterator it)
    {
        return it.key();
    }

    static mapped_type & mapped_reference (iterator it)
    {
        return it.value();
    }
    
    static mapped_type const & mapped_reference (const_iterator it)
    {
        return it.value();
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
class map_reference : public map_basic<KvType, traits::container_reference<KvType, map_wrapper> >
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

//template <typename T>
//struct map_wrapper
//{
//    typedef QMap<typename T::first_type, typename T::second_type> type;
//};
//
//template <typename T>
//class map : public container<T, map_wrapper>
//{
//    typedef container<T, map_wrapper> base_class;
//    
//public:
//    typedef typename base_class::native_type      native_type;
//    typedef typename base_class::native_reference native_reference;
//    typedef typename base_class::size_type        size_type;
//    
//    typedef typename pfs::pair<typename T::first_type
//            , typename T::second_type>                value_type;
//    typedef typename native_type::mapped_type &       reference;
//    typedef typename native_type::mapped_type const & const_reference;
//
//public:
//    map (native_reference rhs)
//        : base_class(rhs)
//    {}
//        
//    size_type max_size () const pfs_noexcept
//    {
//        return ((INT_MAX)/sizeof(T) - sizeof(native_type)) / 2; // FIXME
//    }
//};

}} // pfs::qt

#if __OBSOLETE__

#include <pfs/cxxlang.hpp>
#include <pfs/traits/map.hpp>

namespace pfs {
namespace traits {

namespace qt {

template <typename Key, typename T>
struct map_iterator : public QMap<Key, T>::iterator
{
    typedef typename QMap<Key, T>::iterator base_type;

    map_iterator () pfs_noexcept
        : base_type()
    {}

    map_iterator (base_type lhs) pfs_noexcept
        : base_type(lhs)
    {}
};

template <typename Key, typename T>
struct map_const_iterator : public QMap<Key, T>::const_iterator
{
    typedef typename QMap<Key, T>::const_iterator base_type;

    map_const_iterator () pfs_noexcept
        : base_type()
    {}
    
    map_const_iterator (base_type lhs) pfs_noexcept
        : base_type(lhs)
    {}
};

} // qt

template <typename Key, typename T>
struct map_rep<Key, T, QMap> : public QMap<Key, T>
{
    typedef QMap<Key, T> base_class;
    typedef QMap<Key, T> native_type;

    typedef native_type const &                            const_native_reference;
    typedef typename native_type::size_type                size_type;
    typedef typename native_type::key_type                 key_type;
    typedef typename native_type::mapped_type              mapped_type;
    typedef typename qt::map_iterator<Key, T>              iterator;
    typedef typename qt::map_const_iterator<Key, T>        const_iterator;
    typedef typename std::reverse_iterator<iterator>       reverse_iterator;
    typedef typename std::reverse_iterator<const_iterator> const_reverse_iterator;
    
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
    
    mapped_type & at (Key const & key)
    {
        iterator it = this->find(key);
        if (it == this->end())
            throw out_of_range("map::at");
        return it.value();
    }

    mapped_type const & at (Key const & key) const
    {
        const_iterator it = this->find(key);
        if (it == this->end())
            throw out_of_range("map::at");
        return it.value();
    }
};
//    
//    virtual pfs::pair<iterator, bool> xinsert (Key const & key, T const & value)
//    {
//        iterator it = iterator(this->_d.insert(key, value));
//        return pfs::pair<iterator, bool>(it, true);
//    }
//};

}} // pfs::traits

#endif

#endif /* __PFS_TRAITS_QT_MAP_HPP__ */
