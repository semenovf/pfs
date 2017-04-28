/* 
 * File:   list.hpp
 * Author: wladt
 *
 * Created on January 16, 2017, 2:34 PM
 */

#ifndef __PFS_FOUNDATION_QT_LIST_HPP__
#define __PFS_FOUNDATION_QT_LIST_HPP__

#include <QLinkedList>
#include <pfs/traits/list.hpp>
#include <pfs/foundation/cxx/qt.hpp>

namespace pfs {
namespace traits {

template <typename T>
struct list_traits<foundation::qt, T>
{
    typedef QLinkedList<T>                               native_type;
    typedef native_type const &                          const_native_reference;
    typedef typename native_type::size_type              size_type;
    typedef typename native_type::value_type             value_type;
    typedef typename native_type::reference              reference;
    typedef typename native_type::const_reference        const_reference;
    typedef typename native_type::iterator               iterator;
    typedef typename native_type::const_iterator         const_iterator;
    typedef typename std::reverse_iterator<iterator>     reverse_iterator;
    typedef typename std::reverse_iterator<const_iterator> const_reverse_iterator;
    typedef native_type                                  data_type;
    
    static void xassign (data_type & d, const_native_reference lhs)
    {
        d = lhs;    
    }
    
    static size_type xsize (data_type const & d)
    {
        return d.size();        
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
    
//    static void xreserve (data_type & d, size_type new_capacity);
//    static size_type xcapacity (data_type const & d);
    
    static iterator xerase (data_type & d
            , iterator first
            , iterator last)
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
    
    static void xpush_front (data_type & d, const_reference value)
    {
        d.push_front(value);
    }
    
    static void xpop_front (data_type & d)
    {
        d.pop_front();
    }
    
    static void xresize (data_type & d, size_type count, const_reference value);
    
    static void xswap (data_type & lhs, data_type & rhs)
    {
        lhs.swap(rhs);
    }
    
    static const_native_reference xcast (data_type const & d)
    {
        return d;
    }

    static void xsplice (data_type & lhs, iterator pos, data_type & rhs)
    {
        // Emulate splice operation through move elements
        // TODO Optimize it
        for (iterator it = xbegin(rhs); it != xend(rhs); ++it)
            pos = lhs.insert(pos, *it);
        rhs.clear();
    }
};

template <typename T>
void list_traits<foundation::qt, T>::xresize (data_type & d
        , size_type count
        , const_reference value)
{
    if (count < xsize(d)) {
        // Reduce container to first count elements
        //
        iterator first = xbegin(d);
        pfs::advance(first, count);
        xerase(d, first, xend(d));
    } else {
        for (size_type i = xsize(d); i < count; ++i)
            d.append(value);
    }
}

}} // pfs::traits

#endif /* __PFS_FOUNDATION_QT_LIST_HPP__ */

