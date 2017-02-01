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
struct string_traits<QString>
{
    typedef QString const &                      const_native_reference;
    typedef size_t                               size_type;
    typedef QString::value_type                  value_type;
    typedef QChar const *                        const_pointer;
    typedef QString::const_iterator              const_iterator;
    typedef std::reverse_iterator<QChar const *> const_reverse_iterator;

    typedef QString data_type;
};

template <>
class basic_string<QString> : public details::basic_string<QString>
{
    typedef details::basic_string<QString> base_type;
    typedef typename base_type::data_type data_type;

public:    
    typedef typename base_type::traits_type            traits_type;
    typedef typename base_type::const_native_reference const_native_reference;
    typedef typename base_type::size_type              size_type;
    typedef typename base_type::value_type             value_type;
    typedef typename base_type::const_pointer          const_pointer;
    typedef typename base_type::const_iterator         const_iterator;
    typedef typename base_type::const_reverse_iterator const_reverse_iterator;
    
protected:
    virtual size_type xsize () const
    {
        return this->_d.size();
    }
    
    virtual const_iterator xbegin () const
    {
        return this->_d.begin();
    }

    virtual const_iterator xend () const
    {
        return this->_d.end();
    }

    virtual const_reverse_iterator xrbegin () const
    {
        return const_reverse_iterator(this->_d.end());
    }

    virtual const_reverse_iterator xrend () const
    {
        return const_reverse_iterator(this->_d.begin());
    }
    
    virtual value_type xat (size_type pos) const
    {
        return this->_d.at(int(pos));
    }

    virtual int xcompare (size_type pos1, size_type count1
        , base_type const & rhs, size_type pos2, size_type count2) const
    {
        return this->_d.midRef(pos1, count1).compare(rhs._d.midRef(pos2, count2));
    }

    virtual size_type xfind (const_native_reference rhs, size_type pos) const
    {
        int i = this->_d.indexOf(rhs, pos);
        return i < 0 ? size_type(-1) : size_type(i);
    }
    
    virtual size_type xfind (value_type c, size_type pos) const
    {
        int i = this->_d.indexOf(c, pos);
        return i < 0 ? size_type(-1) : size_type(i);
    }
    
    virtual size_type xrfind (const_native_reference rhs, size_type pos) const
    {
        int i = this->_d.lastIndexOf(rhs, pos);
        return i < 0 ? size_type(-1) : size_type(i);
    }

    virtual size_type xrfind (value_type c, size_type pos) const
    {
        int i = this->_d.lastIndexOf(c, pos);
        return i < 0 ? size_type(-1) : size_type(i);
    }

public:
    basic_string ()
    {}

    basic_string (const_native_reference s)
        : base_type(s)
    {}
    
    basic_string (const_iterator begin, const_iterator end)
        : base_type(data_type(begin, end - begin))
    {}

    basic_string (basic_string const & rhs)
        : base_type(rhs)
    {}

    explicit basic_string (char const * str, size_type n = size_type(-1))
    {
        this->_d.fromUtf8(str, n == size_type(-1) ? -1 : int(n));
    }
    
#ifdef _WCHAR_H
    explicit basic_string (wchar_t const * str, size_type n = size_type(-1))
    {
        this->_d.fromWCharArray(str, n == size_type(-1) ? -1 : int(n));
    }
#endif
    
    virtual const_native_reference native () const
    {
        return this->_d;
    }
};

template <>
class c_str<QString>
{
public:
    typedef string<QString> string_type;
    
private:
    QByteArray _d;
    
public:
    explicit c_str (string_type const & s)
        : _d(s.native().toUtf8())
    {}
    
    char const * operator () () const
    {
        return _d.constData();
    }
};

}} // pfs::traits

#endif /* __PFS_FOUNDATION_QT_STRING_HPP__ */

