/* 
 * File:   map.hpp
 * Author: wladt
 *
 * Created on January 16, 2017, 3:12 PM
 */

#ifndef __PFS_TRAITS_QT_MAP_HPP__
#define __PFS_TRAITS_QT_MAP_HPP__

#include <QMap>
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


//template <typename Key, typename T>
//class basic_map<Key, T, ::qt::map>
//    : public details::basic_map<Key, T, ::qt::map>
//{
//    
//protected:
//    virtual iterator xbegin ()
//    {
//        return iterator(this->_d.begin());
//    }
//    
//    virtual const_iterator xbegin () const
//    {
//        return const_iterator(this->_d.begin());
//    }
//    
//    virtual iterator xend ()
//    {
//        return iterator(this->_d.end());
//    }
//    
//    virtual const_iterator xend () const
//    {
//        return const_iterator(this->_d.end());
//    }
//    
//    virtual reverse_iterator xrbegin ()
//    {
//        return reverse_iterator(this->xend());
//    }
//    
//    virtual const_reverse_iterator xrbegin () const
//    {
//        return const_reverse_iterator(this->xend());
//    }
//    
//    virtual reverse_iterator xrend ()
//    {
//        return reverse_iterator(this->xbegin());
//    }
//    
//    virtual const_reverse_iterator xrend () const
//    {
//        return const_reverse_iterator(this->xbegin());
//    }
//    
//    virtual size_type xsize () const
//    {
//        return this->_d.size();
//    }
//    
//    virtual iterator xerase (iterator position)
//    {
//        return iterator(this->_d.erase(position));
//    }
//    
//    virtual void xswap (base_type & rhs)
//    {
//        this->_d.swap(rhs._d);
//    }
//
//    virtual size_type xcount (Key const & key) const
//    {
//        return this->_d.count(key);
//    }
//    
//    virtual iterator xfind (Key const & key)
//    {
//        return iterator(this->_d.find(key));
//    }
//		
//    virtual const_iterator xfind (Key const & key) const
//    {
//        return const_iterator(this->_d.find(key));
//    }
//    
//    virtual pfs::pair<iterator, bool> xinsert (Key const & key, T const & value)
//    {
//        iterator it = iterator(this->_d.insert(key, value));
//        return pfs::pair<iterator, bool>(it, true);
//    }
//};

}} // pfs::traits

#endif /* __PFS_TRAITS_QT_MAP_HPP__ */
