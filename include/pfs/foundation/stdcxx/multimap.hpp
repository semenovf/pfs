/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   multimap.hpp
 * Author: wladt
 *
 * Created on January 20, 2017, 3:54 PM
 */

#ifndef __PFS_FOUNDATION_STDCXX_MULTIMAP_HPP__
#define __PFS_FOUNDATION_STDCXX_MULTIMAP_HPP__

#include <map>
#include <pfs/cxxlang.hpp>
#include <pfs/traits/multimap.hpp>

namespace stdcxx {

template <typename Key, typename T>
struct multimap
{
    typedef std::multimap<Key, T> type;
};

} // stdcxx

namespace pfs {
namespace traits {

namespace stdcxx {

template <typename Key, typename T>
struct multimap_iterator : public ::stdcxx::multimap<Key, T>::type::iterator
{
    typedef typename ::stdcxx::multimap<Key, T>::type::iterator base_type;

    multimap_iterator (base_type lhs) pfs_noexcept
        : base_type(lhs)
    {}
    
    Key const & key () const
    {
        return base_type::operator->()->first;
    }
    
    T &	value () const
    {
        return base_type::operator->()->second;
    }
};

template <typename Key, typename T>
struct multimap_const_iterator : public ::stdcxx::multimap<Key, T>::type::const_iterator
{
    typedef typename ::stdcxx::multimap<Key, T>::type::const_iterator base_type;

    multimap_const_iterator (base_type lhs) pfs_noexcept
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
struct multimap_traits<Key, T, ::stdcxx::multimap>
{
    typedef typename ::stdcxx::multimap<Key, T>::type        native_type;
    typedef typename native_type::size_type                  size_type;
    typedef typename native_type::key_type                   key_type;
    typedef typename native_type::mapped_type                mapped_type;
    typedef typename stdcxx::multimap_iterator<Key, T>       iterator;
    typedef typename stdcxx::multimap_const_iterator<Key, T> const_iterator;
    typedef typename std::reverse_iterator<iterator>         reverse_iterator;
    typedef typename std::reverse_iterator<const_iterator>   const_reverse_iterator;
    typedef native_type                                      data_type;
};

template <typename Key, typename T>
class basic_multimap<Key, T, ::stdcxx::multimap>
    : public details::basic_multimap<Key, T, ::stdcxx::multimap>
{
    typedef details::basic_multimap<Key, T, ::stdcxx::multimap> base_type;
    typedef basic_multimap<Key, T, ::stdcxx::multimap>          self_type;

protected:
    typedef typename base_type::size_type              size_type;
    typedef typename base_type::native_type            native_type;
    typedef typename base_type::key_type               key_type;
    typedef typename base_type::mapped_type            mapped_type;
    typedef typename base_type::iterator               iterator;
    typedef typename base_type::const_iterator         const_iterator;
    typedef typename base_type::reverse_iterator       reverse_iterator;
    typedef typename base_type::const_reverse_iterator const_reverse_iterator;

    virtual iterator xbegin ()
    {
        return this->_d.begin();
    }
    
    virtual const_iterator xbegin () const
    {
        return this->_d.begin();
    }
    
    virtual iterator xend ()
    {
        return this->_d.end();
    }
    
    virtual const_iterator xend () const
    {
        return this->_d.end();
    }
    
    virtual reverse_iterator xrbegin ()
    {
        return this->_d.rbegin();
    }
    
    virtual const_reverse_iterator xrbegin () const
    {
        return this->_d.rbegin();
    }
    
    virtual reverse_iterator xrend ()
    {
        return this->_d.rend();
    }
    
    virtual const_reverse_iterator xrend () const
    {
        return this->_d.rend();
    }
    
    virtual size_type xsize () const
    {
        return this->_d.size();
    }
    
    virtual void xclear ()
    {
        this->_d.clear();
    }

    virtual iterator xerase (iterator position)
    {
#if __cplusplus >= 201103L
        return _d.erase(position);
#else
        this->_d.erase(position++);
        return position;
#endif        
    }

    virtual iterator xerase (iterator first, iterator last)
    {
#if __cplusplus >= 201103L
        return _d.erase(first, last);
#else
        this->_d.erase(first, last);
        return last;
#endif        
    }
    
    virtual void xswap (base_type & rhs)
    {
        this->_d.swap(rhs._d);
    }
    
    virtual size_type xcount (key_type const & key) const
    {
        return this->_d.count(key);
    }

    virtual iterator xfind (key_type const & key)
    {
        return this->_d.find(key);
    }
		
    virtual const_iterator xfind (key_type const & key) const
    {
        return this->_d.find(key);
    }
    
    virtual pfs::pair<iterator,iterator> xequal_range (key_type const & key)
    {
        return this->_d.equal_range(key);
    }
    
    virtual pfs::pair<const_iterator,const_iterator> xequal_range (key_type const & key) const
    {
        return this->_d.equal_range(key);
    }

    virtual iterator xlower_bound (key_type const & key)
    {
        return this->_d.lower_bound(key);
    }
    
    virtual const_iterator xlower_bound (key_type const & key) const
    {
        return this->_d.lower_bound(key);
    }
    
    virtual iterator xupper_bound (key_type const & key)
    {
        return this->_d.upper_bound(key);
    }
    
    virtual const_iterator upper_bound (key_type const & key) const
    {
        return this->_d.upper_bound(key);
    }
    
    virtual iterator xinsert (key_type const & key, mapped_type const & value)
    {
        return this->_d.insert(std::pair<key_type, T>(key, value));
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

#endif /* __PFS_FOUNDATION_STDCXX_MULTIMAP_HPP__ */
