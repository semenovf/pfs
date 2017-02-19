/* 
 * File:   vector.hpp
 * Author: wladt
 *
 * Created on January 16, 2017, 11:52 AM
 */

#ifndef __PFS_FOUNDATION_STDCXX_VECTOR_HPP__
#define __PFS_FOUNDATION_STDCXX_VECTOR_HPP__

#include <vector>
#include <pfs/traits/vector.hpp>

namespace stdcxx {

template <typename T>
struct vector
{
    typedef std::vector<T> type;
};

} // stdcxx

namespace pfs {
namespace traits {

template <typename T>
struct vector_traits<T, ::stdcxx::vector>
{
    typedef typename ::stdcxx::vector<T>::type           native_type;
    typedef native_type const &                          const_native_reference;
    typedef typename native_type::size_type              size_type;
    typedef typename native_type::value_type             value_type;
    typedef typename native_type::reference              reference;
    typedef typename native_type::const_reference        const_reference;
    typedef typename native_type::pointer                pointer;
    typedef typename native_type::const_pointer          const_pointer;
    typedef typename native_type::iterator               iterator;
    typedef typename native_type::const_iterator         const_iterator;
    typedef typename native_type::reverse_iterator       reverse_iterator;
    typedef typename native_type::const_reverse_iterator const_reverse_iterator;
    typedef native_type                                  data_type;

    static void xassign (data_type & d, const_native_reference lhs)
    {
        
    }
    
    static void xassign (data_type & d, const_pointer lhs, size_type n)
    {
        d.assign(n, lhs);
    }
    
    static size_type xsize (data_type const & d)
    {
        return d.size();
    }
    
    static size_type xmax_size (data_type const & d)
    {
        return d.max_size();
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
    
    static reference xat (data_type const & d, size_type pos)
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

#endif /* __PFS_FOUNDATION_STDCXX_VECTOR_HPP__ */

