/* 
 * File:   map.hpp
 * Author: wladt
 *
 * Created on January 16, 2017, 3:03 PM
 */

#ifndef __PFS_TRAITS_STDCXX_MAP_HPP__
#define __PFS_TRAITS_STDCXX_MAP_HPP__

#include <map>
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

#endif /* __PFS_TRAITS_STDCXX_MAP_HPP__ */
