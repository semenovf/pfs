/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   map.hpp
 * Author: wladt
 *
 * Created on January 16, 2017, 3:12 PM
 */

#ifndef __PFS_FOUNDATION_QT_MAP_HPP__
#define __PFS_FOUNDATION_QT_MAP_HPP__

#include <QMap>
#include <pfs/cxxlang.hpp>
#include <pfs/traits/map.hpp>

namespace qt {

template <typename Key, typename T>
struct map
{
    typedef QMap<Key, T> type;
};

} // qt

namespace pfs {
namespace traits {

namespace qt {

template <typename Key, typename T>
struct map_iterator : public QMap<Key, T>::iterator
{
    typedef typename QMap<Key, T>::iterator base_type;

    map_iterator (base_type lhs) pfs_noexcept
        : base_type(lhs)
    {}
};

template <typename Key, typename T>
struct map_const_iterator : public QMap<Key, T>::const_iterator
{
    typedef typename QMap<Key, T>::const_iterator base_type;

    map_const_iterator (base_type lhs) pfs_noexcept
        : base_type(lhs)
    {}
};

} // qt

template <typename Key, typename T>
struct map_traits<Key, T, ::qt::map>
{
    typedef typename ::qt::map<Key, T>::type             native_type;
    typedef typename native_type::size_type              size_type;
    typedef typename native_type::key_type               key_type;
    typedef typename native_type::mapped_type            mapped_type;
    typedef typename qt::map_iterator<Key, T>            iterator;
    typedef typename qt::map_const_iterator<Key, T>      const_iterator;
    typedef typename std::reverse_iterator<iterator>     reverse_iterator;
    typedef typename std::reverse_iterator<const_iterator> const_reverse_iterator;
    typedef typename native_type::difference_type        difference_type;
    typedef native_type                                  data_type;
};


template <typename Key, typename T>
class basic_map<Key, T, ::qt::map>
    : public details::basic_map<Key, T, ::qt::map>
{
protected:
    typedef details::basic_map<Key, T, ::qt::map>        base_type;
    typedef typename base_type::size_type              size_type;
    typedef typename base_type::mapped_type            mapped_type;
    typedef typename base_type::native_type            native_type;
    typedef typename base_type::iterator               iterator;
    typedef typename base_type::const_iterator         const_iterator;
    typedef typename base_type::reverse_iterator       reverse_iterator;
    typedef typename base_type::const_reverse_iterator const_reverse_iterator;
    
protected:
    virtual iterator xbegin ()
    {
        return iterator(this->_d.begin());
    }
    
    virtual const_iterator xbegin () const
    {
        return const_iterator(this->_d.begin());
    }
    
    virtual iterator xend ()
    {
        return iterator(this->_d.end());
    }
    
    virtual const_iterator xend () const
    {
        return const_iterator(this->_d.end());
    }
    
    virtual reverse_iterator xrbegin ()
    {
        return reverse_iterator(this->xend());
    }
    
    virtual const_reverse_iterator xrbegin () const
    {
        return const_reverse_iterator(this->xend());
    }
    
    virtual reverse_iterator xrend ()
    {
        return reverse_iterator(this->xbegin());
    }
    
    virtual const_reverse_iterator xrend () const
    {
        return const_reverse_iterator(this->xbegin());
    }
    
    virtual size_type xsize () const
    {
        return this->_d.size();
    }
    
    virtual mapped_type & xat (Key const & key)
    {
        iterator it = this->xfind(key);
        if (it == this->xend())
            throw out_of_range("map::at");
        return it.value();
    }

    virtual mapped_type const & xat (Key const & key) const
    {
        const_iterator it = this->xfind(key);
        if (it == this->xend())
            throw out_of_range("map::at");
        return it.value();
    }
    
    virtual mapped_type & xsubscript (Key const & key)
    {
        return this->_d[key];
    }

    virtual void xclear ()
    {
        this->_d.clear();
    }
    
    virtual iterator xerase (iterator position)
    {
        return iterator(this->_d.erase(position));
    }
    
    virtual void xswap (base_type & rhs)
    {
        this->_d.swap(rhs._d);
    }

    virtual size_type xcount (Key const & key) const
    {
        return this->_d.count(key);
    }
    
    virtual iterator xfind (Key const & key)
    {
        return iterator(this->_d.find(key));
    }
		
    virtual const_iterator xfind (Key const & key) const
    {
        return const_iterator(this->_d.find(key));
    }
    
    virtual pfs::pair<iterator, bool> xinsert (Key const & key, T const & value)
    {
        iterator it = iterator(this->_d.insert(key, value));
        return pfs::pair<iterator, bool>(it, true);
    }
    
public:
    virtual native_type & native ()
    {
        return this->_d;
    }

    virtual native_type const & native () const
    {
        return this->_d;
    }
};

}} // pfs::traits

#endif /* __PFS_FOUNDATION_QT_MAP_HPP__ */
