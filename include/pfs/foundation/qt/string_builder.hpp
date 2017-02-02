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

#ifndef __PFS_FOUNDATION_QT_STRING_BUILDER_HPP__
#define __PFS_FOUNDATION_QT_STRING_BUILDER_HPP__

#include <string>
#include <pfs/foundation/qt/string.hpp>
#include <pfs/traits/string_builder.hpp>

namespace pfs {
namespace traits {

template <>
struct string_builder_traits<QChar>
{
    typedef QString       native_type;
    typedef size_t        size_type;
    typedef QChar         value_type;
    typedef QChar const * const_pointer;
    typedef native_type   data_type;
    
    static void xpush_back (data_type & d, const_pointer s, size_type n)
    {
        d.insert(d.size(), s, int(n));
    }
    
    static void xpush_back (data_type & d, size_type n, value_type c)
    {
        d.append(native_type(int(n), c));
    }
};

//template <>
//template <>
//inline string_builder<QChar> & string_builder<QChar>::push_back (char const * s)
//{
//    _d.append(s);
//    return *this;
//}

//template <>
//template <>
//inline string_builder<char> & string_builder<char>::push_back<std::string> (std::string const & s)
//{
//    _d.append(s);
//    return *this;
//}
//
//template <>
//template <>
//inline string_builder<wchar_t> & string_builder<wchar_t>::push_back<std::wstring> (std::wstring const & s)
//{
//    _d.append(s);
//    return *this;
//}
//
//template <>
//template <>
//std::string string_builder<char>::str<std::string> () const
//{
//    return _d;
//}
//
//template <>
//template <>
//std::wstring string_builder<char>::str<std::wstring> () const
//{
//    return _d;
//}
//
//template <>
//template <>
//traits::string<char> string_builder<char>::str<traits::string<char> > () const
//{
//    return traits::string<char>(_d);
//}
//
//template <>
//template <>
//traits::string<wchar_t> string_builder<wchar_t>::str<traits::string<wchar_t> > () const
//{
//    return traits::string<wchar_t>(_d);
//}
//
//template <>
//template <>
//char const * string_builder<char>::str<char const *> () const
//{
//    return _d.data();
//}
//
//template <>
//template <>
//wchar_t const * string_builder<wchar_t>::str<wchar_t const *> () const
//{
//    return _d.data();
//}

}} // pfs::traits

#endif /* __PFS_FOUNDATION_QT_STRING_BUILDER_HPP__ */

