/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   string_builder.hpp
 * Author: wladt
 *
 * Created on January 24, 2017, 2:49 PM
 */

#ifndef __PFS_FOUNDATION_STDCXX_STRING_BUILDER_HPP__
#define __PFS_FOUNDATION_STDCXX_STRING_BUILDER_HPP__

#include <string>
#include <pfs/foundation/stdcxx/string.hpp>
#include <pfs/traits/string_builder.hpp>

namespace pfs {
namespace traits {

namespace stdcxx {

template <typename CharT>
struct string_builder_traits
{
    typedef std::basic_string<CharT>            native_type;
    typedef typename native_type::size_type     size_type;
    typedef typename native_type::value_type    value_type;
    typedef typename native_type::const_pointer const_pointer;
    typedef native_type                         data_type;
    
    static void xpush_back (data_type & d, const_pointer s, size_type n)
    {
        if (n == size_type(-1))
            d.append(s);
        else
            d.append(s, n);
    }
    
    static void xpush_back (data_type & d, size_type n, value_type c)
    {
        d.append(n, c);
    }
};

} // stdcxx

template <>
struct string_builder_traits<char> : public stdcxx::string_builder_traits<char>
{};

template <>
struct string_builder_traits<wchar_t> : public stdcxx::string_builder_traits<wchar_t>
{};

template <>
template <>
inline string_builder<char> & 
string_builder<char>::push_back (traits::string<char> const & s)
{
    _d.append(s.cast());
    return *this;
}

template <>
template <>
inline string_builder<wchar_t> &
string_builder<wchar_t>::push_back (traits::string<wchar_t> const & s)
{
    _d.append(s.cast());
    return *this;
}

template <>
template <>
inline string_builder<char> & string_builder<char>::push_back (std::string const & s)
{
    _d.append(s);
    return *this;
}

template <>
template <>
inline string_builder<wchar_t> & string_builder<wchar_t>::push_back (std::wstring const & s)
{
    _d.append(s);
    return *this;
}

template <>
template <>
std::string string_builder<char>::str<std::string> () const
{
    return _d;
}

template <>
template <>
std::wstring string_builder<wchar_t>::str<std::wstring> () const
{
    return _d;
}

template <>
template <>
traits::string<char> string_builder<char>::str<traits::string<char> > () const
{
    return traits::string<char>(_d);
}

template <>
template <>
traits::string<wchar_t> string_builder<wchar_t>::str<traits::string<wchar_t> > () const
{
    return traits::string<wchar_t>(_d);
}

template <>
template <>
char const * string_builder<char>::str<char const *> () const
{
    return _d.data();
}

template <>
template <>
wchar_t const * string_builder<wchar_t>::str<wchar_t const *> () const
{
    return _d.data();
}

}} // pfs::traits

#endif /* __PFS_FOUNDATION_STDCXX_STRING_BUILDER_HPP__ */

