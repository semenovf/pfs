/* 
 * File:   string.hpp
 * Author: wladt
 *
 * Created on January 9, 2017, 3:21 PM
 */

#ifndef __PFS_FOUNDATION_STDCXX_STRING_HPP__
#define __PFS_FOUNDATION_STDCXX_STRING_HPP__

#include <string>
#include <cstring>
#include <pfs/traits/string.hpp>
#include <pfs/foundation/cxx/stdcxx.hpp>

namespace pfs {
namespace traits {

namespace stdcxx {

template <typename Foundation, typename CharT>
struct string_traits
{
    typedef std::basic_string<CharT>                     native_type;
    typedef native_type const &                          const_native_reference;
    typedef typename native_type::size_type              size_type;
    typedef typename native_type::value_type             value_type;
    typedef typename native_type::reference              reference;
    typedef typename native_type::const_reference        const_reference;
    typedef typename native_type::const_pointer          const_pointer;
    typedef typename native_type::iterator               iterator;
    typedef typename native_type::const_iterator         const_iterator;
    typedef typename native_type::const_reverse_iterator const_reverse_iterator;
    typedef native_type                                  data_type;

    static size_type xlength (const_pointer p);
    
    static void xassign (data_type & d, const_native_reference lhs)
    {
        d = lhs;
    }

    static void xassign (data_type & d, const_pointer lhs, size_type n)
    {
        d = (n == size_type(-1))
                ? data_type(lhs)
                : data_type(lhs, n);
    }
    
    static size_type xsize (data_type const & d)
    {
        return d.size();    
    }
    
    static size_type xmax_size (data_type const & d)
    {
        return d.max_size();
    }
    
    static const_iterator xbegin (data_type const & d)
    {
        return d.begin();
    }
    
    static const_iterator xend (data_type const & d)
    {
        return d.end();
    }
    
    static const_reverse_iterator xrbegin (data_type const & d)
    {
        return d.rbegin();
    }
    
    static const_reverse_iterator xrend (data_type const & d)
    {
        return d.rend();
    }
    
    static value_type xat (data_type const & d, size_type pos)
    {
        return d.at(pos);
    }

    static int xcompare (data_type const & d
            , size_type pos1, size_type count1
            , data_type const & rhs
            , size_type pos2, size_type count2)
    {
        return d.compare(pos1, count1, rhs, pos2, count2);
    }
    
    static size_type xfind (data_type const & d
            , data_type const & rhs
            , size_type pos)
    {
        return d.find(rhs, pos);
    }
    
    static size_type xfind (data_type const & d
            , value_type c
            , size_type pos)
    {
        return d.find(c, pos);
    }

    static size_type xrfind (data_type const & d
            , data_type const & rhs
            , size_type pos)
    {
        return d.rfind(rhs, pos);
    }
    
    static size_type xrfind (data_type const & d
            , value_type c
            , size_type pos)
    {
        return d.rfind(c, pos);
    }
    
    static void xerase (data_type & d, size_type index, size_type count)
    {
        d.erase(index, count);
    }

    static iterator xerase (data_type & d, const_iterator first, const_iterator last)
#if __cplusplus >= 201103L
    {
        return d.erase(first, last);
    }
#else
    ;
#endif
    
    static void xclear (data_type & d)
    {
        d.clear();
    }

    static void xappend (data_type & d, size_type count, const_reference ch)
    {
        d.append(count, ch);
    }
    
    static void xappend (data_type & d, const_pointer s, size_type count)
    {
        d.append(s, count);
    }

    static void xinsert (data_type & d, size_type index, size_type count, value_type ch)
    {
        d.insert(index, count, ch);
    }
    
    static void xinsert (data_type & d, size_type index, const_pointer s)
    {
        d.insert(index, index, s);
    }
    
    static void xinsert (data_type & d, size_type index, const_pointer s, size_type count)
    {
        d.insert(index, s, count);
    }
    
    static void xpush_back (data_type & d, value_type ch)
    {
        d.push_back(ch);
    }

    static const_pointer xdata (data_type const & d)
    {
        return d.c_str();
    }
    
    static const_native_reference xcast (data_type const & d)
    {
        return d;
    }
};

#if __cplusplus < 201103L
//
// C++ prior to C++11 
// erase() has signature `iterator erase(iterator first, iterator last)`
//
template <typename Foundation, typename CharT>
typename string_traits<Foundation, CharT>::iterator 
string_traits<Foundation, CharT>::xerase (data_type & d
    , const_iterator first
    , const_iterator last)
{
    const_iterator begin = d.begin();
    size_type index = pfs::distance(begin, first);
    size_type count = pfs::distance(first, last);
    d.erase(index, count);
    return d.begin() + index;
}

#endif

template <typename Foundation, typename CharT>
class c_str
{
public:
    typedef string<Foundation, CharT> string_type;
    
protected:
    string_type const & _d;
    
public:
    explicit c_str (string_type const & s)
        : _d(s)
    {}
    
    CharT const * operator () () const
    {
        return _d.data();
    }
    
    operator CharT const * () const
    {
        return _d.data();
    }
};

template <>
inline string_traits<foundation::stdcxx, char>::size_type 
string_traits<foundation::stdcxx, char>::xlength (const_pointer p)
{
    return std::strlen(p);
}

template <>
inline string_traits<foundation::stdcxx, wchar_t>::size_type
string_traits<foundation::stdcxx, wchar_t>::xlength (const_pointer p)
{
    return std::wcslen(p);
}

} // stdcxx

template <>
struct string_traits<foundation::stdcxx, char> 
    : public stdcxx::string_traits<foundation::stdcxx, char>
{};

template <>
struct string_traits<foundation::stdcxx, wchar_t> 
    : public stdcxx::string_traits<foundation::stdcxx, wchar_t>
{};

template <>
class c_str<foundation::stdcxx, char> 
    : public stdcxx::c_str<foundation::stdcxx, char>
{
    typedef stdcxx::c_str<foundation::stdcxx, char> base_type;
public:
    explicit c_str (string_type const & s)
        : base_type(s)
    {}
};

template <>
class c_wstr<foundation::stdcxx, wchar_t> 
    : public stdcxx::c_str<foundation::stdcxx, wchar_t>
{
    typedef stdcxx::c_str<foundation::stdcxx, wchar_t> base_type;
public:
    explicit c_wstr (string_type const & s)
        : base_type(s)
    {}
};

template <>
inline int compare<foundation::stdcxx, char> (
          string<foundation::stdcxx, char> const & lhs
        , char const * rhs)
{
    return lhs._d.compare(rhs);
}

template <>
inline int compare<foundation::stdcxx, wchar_t> (
          string<foundation::stdcxx, wchar_t> const & lhs
        , wchar_t const * rhs)
{
    return lhs._d.compare(rhs);
}

}} // pfs::traits

#endif /* __PFS_FOUNDATION_STDCXX_STRING_HPP__ */
