/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   vector.hpp
 * Author: wladt
 *
 * Created on February 26, 2017, 10:58 AM
 */

#ifndef __PFS_FOUNDATION_QT_VECTOR_HPP__
#define __PFS_FOUNDATION_QT_VECTOR_HPP__

#include <QVector>
#include <pfs/algo/compare.hpp>
#include <pfs/traits/vector.hpp>

namespace qt {

template <typename T>
struct vector
{
    typedef QVector<T> type;
};

} // qt

namespace pfs {
namespace traits {

template <typename T>
struct vector_traits<T, ::qt::vector>
{
    typedef typename ::qt::vector<T>::type                 native_type;
    typedef native_type const &                            const_native_reference;
    typedef typename native_type::size_type                size_type;
    typedef typename native_type::value_type               value_type;
    typedef typename native_type::reference                reference;
    typedef typename native_type::const_reference          const_reference;
    typedef typename native_type::pointer                  pointer;
    typedef typename native_type::const_pointer            const_pointer;
    typedef typename native_type::iterator                 iterator;
    typedef typename native_type::const_iterator           const_iterator;
    typedef typename std::reverse_iterator<iterator>       reverse_iterator;
    typedef typename std::reverse_iterator<const_iterator> const_reverse_iterator;
    typedef native_type                                    data_type;

    static void xassign (data_type & d, const_native_reference lhs)
    {
        d = lhs;
    }
    
    static size_type xsize (data_type const & d)
    {
        return d.size();
    }
    
    static size_type xmax_size (data_type const & d)
    {
        return (INT_MAX)/sizeof(T) - sizeof(native_type);
    }
    
    static iterator xbegin (data_type & d)
    {
        return d.begin();
    }
    
    static const_iterator xbegin (data_type const & d)
    {
        return d.begin();
    }
    
    static iterator xend (data_type & d)
    {
        return d.end();
    }
    
    static const_iterator xend (data_type const & d)
    {
        return d.end();
    }
    
    static reverse_iterator xrbegin (data_type & d)
    {
        return reverse_iterator(xend(d));
    }
    
    static const_reverse_iterator xrbegin (data_type const & d)
    {
        return const_reverse_iterator(xend(d));
    }
    
    static reverse_iterator xrend (data_type & d)
    {
        return reverse_iterator(xbegin(d));
    }
    
    static const_reverse_iterator xrend (data_type const & d)
    {
        return const_reverse_iterator(xbegin(d));
    }
    
    static reference xat (data_type & d, size_type pos)
    {
        return d[pos];
    }
    
    static const_reference xat (data_type const & d, size_type pos)
    {
        return d.at(pos);
    }
    
    static void xreserve (data_type & d, size_type new_capacity)
    {
        d.reserve(new_capacity);
    }
    
    static size_type xcapacity (data_type const & d)
    {
        return d.capacity(d);
    }
    
    static iterator xerase (data_type & d, const_iterator first, const_iterator last)
    {
        return d.erase(first, last);
    }
    
    static void xclear (data_type & d)
    {
        d.clear();
    }
    
    static void xpush_back (data_type & d, const_reference value)
    {
        d.push_back(value);
    }
    
    static void xpop_back (data_type & d)
    {
        d.pop_back();
    }
    
    static void xresize (data_type & d, size_type count, const_reference value)
    {
        d.resize(count, value);
    }
    
    static int xcompare (data_type const & lhs, data_type const & rhs)
    {
        return pfs::compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
    }
    
    static void xswap (data_type & lhs, data_type & rhs)
    {
        lhs.swap(rhs);
    }
    
    static pointer xdata (data_type & d)
    {
        return d.data();
    }
    
    static const_native_reference xcast (data_type const & d)
    {
        return d;
    }
};

}} // pfs::traits

#endif /* __PFS_FOUNDATION_QT_VECTOR_HPP__ */

