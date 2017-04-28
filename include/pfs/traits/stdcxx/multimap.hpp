/* 
 * File:   multimap.hpp
 * Author: wladt
 *
 * Created on January 20, 2017, 3:54 PM
 */

#ifndef __PFS_TRAITS_STDCXX_MULTIMAP_HPP__
#define __PFS_TRAITS_STDCXX_MULTIMAP_HPP__

#include <map>
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

//namespace stdcxx {
//
//template <typename Key, typename T>
//struct multimap_iterator : public ::stdcxx::multimap<Key, T>::type::iterator
//{
//    typedef typename ::stdcxx::multimap<Key, T>::type::iterator base_type;
//
//    multimap_iterator (base_type lhs) pfs_noexcept
//        : base_type(lhs)
//    {}
//    
//    Key const & key () const
//    {
//        return base_type::operator->()->first;
//    }
//    
//    T &	value () const
//    {
//        return base_type::operator->()->second;
//    }
//};
//
//template <typename Key, typename T>
//struct multimap_const_iterator : public ::stdcxx::multimap<Key, T>::type::const_iterator
//{
//    typedef typename ::stdcxx::multimap<Key, T>::type::const_iterator base_type;
//
//    multimap_const_iterator (base_type lhs) pfs_noexcept
//        : base_type(lhs)
//    {}
//    
//    Key const & key () const
//    {
//        return base_type::operator->()->first;
//    }
//    
//    T const & value () const
//    {
//        return base_type::operator->()->second;
//    }
//};
//
//} // stdcxx

template <typename Key, typename T>
struct multimap_rep<Key, T, ::stdcxx::multimap> : public ::stdcxx::multimap<Key, T>::type
{
    typedef typename ::stdcxx::multimap<Key, T>::type    base_class;
    typedef typename ::stdcxx::multimap<Key, T>::type    native_type;

    typedef native_type const &                          const_native_reference;
    typedef typename native_type::size_type              size_type;
    typedef typename native_type::key_type               key_type;
    typedef typename native_type::mapped_type            mapped_type;
    typedef typename native_type::iterator               iterator;
    typedef typename native_type::const_iterator         const_iterator;
    typedef typename native_type::reverse_iterator       reverse_iterator;
    typedef typename native_type::const_reverse_iterator const_reverse_iterator;
    
	explicit multimap_rep ()
		: base_class()
	{}

    multimap_rep (const_native_reference rhs)
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

#endif /* __PFS_TRAITS_STDCXX_MULTIMAP_HPP__ */
