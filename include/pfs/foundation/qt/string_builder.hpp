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
#include <QByteArray>

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

template <>
inline string_builder<QChar> & 
string_builder<QChar>::push_back (traits::string<QChar> const & s)
{
    _d.append(s.cast());
    return *this;
}

template <>
template <>
inline string_builder<QChar> & string_builder<QChar>::push_back (char const * s)
{
    _d.append(s);
    return *this;
}

template <>
template <>
inline string_builder<QChar> & string_builder<QChar>::push_back (char const * s, size_type n )
{
    _d.append(QByteArray(s, int(n)).constData());
    return *this;
}

template <>
template <>
inline string_builder<QChar> & string_builder<QChar>::push_back (char c)
{
    _d.append(QChar(c));
    return *this;
}

template <>
template <>
inline string_builder<QChar> & string_builder<QChar>::push_back (size_type n, char c)
{
    _d.append(QString(int(n), QChar(c)));
    return *this;
}

template <>
template <>
traits::string<QChar> string_builder<QChar>::str<traits::string<QChar> > () const
{
    return traits::string<QChar>(_d);
}

}} // pfs::traits

#endif /* __PFS_FOUNDATION_QT_STRING_BUILDER_HPP__ */

