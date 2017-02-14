/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   string.hpp
 * Author: wladt
 *
 * Created on January 9, 2017, 3:21 PM
 */

#ifndef __PFS_FOUNDATION_STDCXX_STRING_HPP__
#define __PFS_FOUNDATION_STDCXX_STRING_HPP__

#include <string>
#include <pfs/traits/string.hpp>

namespace pfs {
namespace traits {

namespace stdcxx {

template <typename CharT>
struct string_traits
{
    typedef std::basic_string<CharT>                     native_type;
    typedef native_type const &                          const_native_reference;
    typedef typename native_type::size_type              size_type;
    typedef typename native_type::value_type             value_type;
    typedef typename native_type::const_pointer          const_pointer;
    typedef typename native_type::iterator               iterator;
    typedef typename native_type::const_iterator         const_iterator;
    typedef typename native_type::const_reverse_iterator const_reverse_iterator;
    typedef native_type                                  data_type;

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
template <typename CharT>
typename string_traits<CharT>::iterator string_traits<CharT>::xerase (data_type & d
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

template <typename CharT>
class c_str
{
public:
    typedef string<CharT> string_type;
    
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

} // stdcxx

template <>
struct string_traits<char> : public stdcxx::string_traits<char>
{};

template <>
struct string_traits<wchar_t> : public stdcxx::string_traits<wchar_t>
{};

template <>
class c_str<char> : public stdcxx::c_str<char>
{
    typedef stdcxx::c_str<char> base_type;
public:
    explicit c_str (string_type const & s)
        : base_type(s)
    {}
};

template <>
class c_wstr<wchar_t> : public stdcxx::c_str<wchar_t>
{
    typedef stdcxx::c_str<wchar_t> base_type;
public:
    explicit c_wstr (string_type const & s)
        : base_type(s)
    {}
};

template <>
int compare<char> (string<char> const & lhs, char const * rhs)
{
    return lhs._d.compare(rhs);
}

template <>
int compare<wchar_t> (string<wchar_t> const & lhs, wchar_t const * rhs)
{
    return lhs._d.compare(rhs);
}

}} // pfs::traits

#endif /* __PFS_FOUNDATION_STDCXX_STRING_HPP__ */