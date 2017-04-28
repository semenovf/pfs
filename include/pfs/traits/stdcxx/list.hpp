/* 
 * File:   list.hpp
 * Author: wladt
 *
 * Created on January 16, 2017, 12:58 PM
 */

#ifndef __PFS_FOUNDATION_STDCXX_LIST_HPP__
#define __PFS_FOUNDATION_STDCXX_LIST_HPP__

#include <list>
#include <pfs/traits/list.hpp>
#include <pfs/foundation/cxx/stdcxx.hpp>

namespace pfs {
namespace traits {

template <typename T>
struct list_traits<foundation::stdcxx, T>
{
    typedef std::list<T>                                 native_type;
    typedef native_type const &                          const_native_reference;
    typedef typename native_type::size_type              size_type;
    typedef typename native_type::value_type             value_type;
    typedef typename native_type::reference              reference;
    typedef typename native_type::const_reference        const_reference;
    typedef typename native_type::iterator               iterator;
    typedef typename native_type::const_iterator         const_iterator;
    typedef typename native_type::reverse_iterator       reverse_iterator;
    typedef typename native_type::const_reverse_iterator const_reverse_iterator;
    typedef native_type                                  data_type;
    
    static void xassign (data_type & d, const_native_reference lhs)
    {
        d = lhs;    
    }
    
    static size_type xsize (data_type const & d)
    {
        return d.size();        
    }
    
//    static size_type xmax_size (data_type const & d);
    
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
        return d.rbegin();
    }
    
    static const_reverse_iterator xrbegin (data_type const & d)
    {
        return d.rbegin();
    }
    
    static reverse_iterator xrend (data_type & d)
    {
        return d.rend();
    }
    
    static const_reverse_iterator xrend (data_type const & d)
    {
        return d.rend();
    }
    
//    static void xreserve (data_type & d, size_type new_capacity);
//    static size_type xcapacity (data_type const & d);
    
    static iterator xerase (data_type & d
            , iterator first
            , iterator last)
    {
#if __cplusplus >= 201103
        return d.erase(first, last);
#else
        d.erase(first, last);
        return last; // TODO check in manual if iterator is valid after eraser.
#endif        
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
    
    static void xresize (data_type & d, size_type count, const_reference value)
    {
        d.resize(count, value);
    }
    
//    static int xcompare (data_type const & lhs, data_type const & rhs);
    
    static void xswap (data_type & lhs, data_type & rhs)
    {
        lhs.swap(rhs);
    }

    static void xsplice (data_type & lhs, iterator pos, data_type & rhs)
    {
        lhs.splice(pos, rhs);
    }
    
    static const_native_reference xcast (data_type const & d)
    {
        return d;
    }

    static iterator xinsert (data_type & d, iterator pos, value_type const & value)
    {
#if __cplusplus >= 201103L
        return d.insert(pos, value);
#else
        d.insert(pos++, value);
        return pos;
#endif
    }
};

}} // pfs::traits

#endif /* __PFS_FOUNDATION_STDCXX_LIST_HPP__ */
