/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   string.hpp
 * Author: wladt
 *
 * Created on January 11, 2017, 4:05 PM
 */

#ifndef __PFS_FOUNDATION_QT_STRING_HPP__
#define __PFS_FOUNDATION_QT_STRING_HPP__

#include <pfs/traits/string.hpp>
#include <QString>

namespace pfs {
namespace traits {

template <>
struct string_traits<QChar>
{
    typedef QString                               native_type;
    typedef QString const &                       const_native_reference;
    typedef size_t                                size_type;
    typedef QString::value_type                   value_type;
    typedef QChar const *                         const_pointer;
    typedef QString::const_iterator               const_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
    typedef native_type                           data_type;

    static void xassign (data_type & d, const_native_reference lhs)
    {
        d = lhs;
    }

    static void xassign (data_type & d, const_pointer lhs, size_type n)
    {
        d = (n == size_type(-1)) 
                ? data_type(lhs)
                : data_type(lhs, int(n));
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
        return const_reverse_iterator(d.end());
    }
    
    static const_reverse_iterator xrend (data_type const & d)
    {
        return const_reverse_iterator(d.begin());
    }
    
    static value_type xat (data_type const & d, size_type pos)
    {
        return d.at(int(pos));
    }

    static int xcompare (data_type const & d
            , size_type pos1, size_type count1
            , data_type const & rhs
            , size_type pos2, size_type count2)
    {
        return d.midRef(pos1, count1).compare(rhs.midRef(pos2, count2));
    }
    
    static size_type xfind (data_type const & d
            , data_type const & rhs
            , size_type pos)
    {
        int i = d.indexOf(rhs, pos); 
        return i < 0 ? size_type(-1) : size_type(i);
    }
    
    static size_type xfind (data_type const & d
            , value_type c
            , size_type pos)
    {
        int i = d.indexOf(c, pos);
        return i < 0 ? size_type(-1) : size_type(i);
    }

    static size_type xrfind (data_type const & d
            , data_type const & rhs
            , size_type pos)
    {
        int i = d.lastIndexOf(rhs, pos);
        return i < 0 ? size_type(-1) : size_type(i);
    }
    
    static size_type xrfind (data_type const & d
            , value_type c
            , size_type pos)
    {
        int i = d.lastIndexOf(c, pos);
        return i < 0 ? size_type(-1) : size_type(i);
    }
    
    static const_pointer xdata (data_type const & d)
    {
        return d.constData();
    }
    
    static const_native_reference xcast (data_type const & d)
    {
        return d;
    }
};

template <>
inline string<QChar>::string (const_iterator begin, const_iterator end)
    : _d(begin, end - begin)
{}

template <>
int compare<QChar> (string<QChar> const & lhs, char const * rhs)
{
    return lhs._d.compare(QString(rhs)); // TODO Reimplement
}

template <>
class c_str<QChar>
{
public:
    typedef string<QChar> string_type;
    
private:
    QByteArray _d;
    
public:
    explicit c_str (string_type const & s)
        : _d(s.cast().toUtf8())
    {}
    
    char const * operator () () const
    {
        return _d.constData();
    }
    
    operator char const * () const
    {
        return _d.constData();
    }
};

template <>
class c_wstr<QChar>
{
public:
    typedef string<QChar> string_type;
    
private:
    std::wstring _d;
    
public:
    explicit c_wstr (string_type const & s)
        : _d(s.cast().toStdWString())
    {}
    
    wchar_t const * operator () () const
    {
        return _d.c_str();
    }
    
    operator wchar_t const * () const
    {
        return _d.c_str();
    }
};

}} // pfs::traits

#endif /* __PFS_FOUNDATION_QT_STRING_HPP__ */

