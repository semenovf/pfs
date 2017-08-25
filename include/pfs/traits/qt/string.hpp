/* 
 * File:   string.hpp
 * Author: wladt
 *
 * Created on January 11, 2017, 4:05 PM
 */

#ifndef __PFS_TRAITS_QT_STRING_HPP__
#define __PFS_TRAITS_QT_STRING_HPP__

#include <QString>
#include <QChar>
#include <ostream>
#include <pfs/cxxlang.hpp>
#include <pfs/limits.hpp>
#include <pfs/ctype.hpp>
#include <pfs/unicode/unicode_iterator.hpp>
#include <pfs/traits/string_value_ref.hpp>

namespace pfs {
namespace unicode {

template <>
inline uint16_t code_point_cast<uint16_t, QChar> (QChar x)
{
    return static_cast<uint16_t>(x.unicode());
}

template <>
struct unicode_iterator_traits<QChar *>
    :  unicode_iterator_limits<QChar *, QChar *>
{
    typedef utf16_iterator<QChar *> iterator;
};

template <>
struct unicode_iterator_traits<QChar const *>
    :  unicode_iterator_limits<QChar const *, QChar const *>
{
    typedef utf16_iterator<QChar const *> iterator;
};

}}

namespace pfs {
namespace traits {

//template <>
//template <typename InputIt>
//string_value<QChar, QString>::string_value (InputIt first, InputIt last)
////    : v(first, pfs::distance(first, last))
//{
//    v = QString(first, pfs::distance(first, last));
//}

namespace qt {

template <typename ValueOrReference>
class basic_string
{
    typedef ValueOrReference internal_type;
    
public:
    typedef basic_string<typename internal_type::string_value_type>     string_value_type;
    typedef basic_string<typename internal_type::string_reference_type> string_reference_type;

    typedef typename internal_type::native_type            native_type;
    typedef typename internal_type::native_reference       native_reference;
    typedef typename internal_type::const_native_reference const_native_reference;
    
    typedef QChar                                 value_type;
    typedef QChar *                               pointer;
    typedef QChar const *                         const_pointer;
    typedef typename native_type::reference       reference;
    typedef typename native_type::const_reference const_reference;
    typedef typename native_type::iterator        iterator;
    typedef typename native_type::const_iterator  const_iterator;
    typedef std::reverse_iterator<iterator>       reverse_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
    typedef ptrdiff_t                             difference_type;
    typedef int                                   size_type; // no native_type::size_type found, but documentation mentions.
    
    static size_type const npos = size_type(-1);

protected:
    internal_type _p;
    
public:
    basic_string ()
    {}

    basic_string (native_reference rhs)
        : _p(rhs)
    {}

    basic_string (const_native_reference rhs)
        : _p(rhs)
    {}
    
    basic_string (char const * s)
        : _p(s)
    {}

    basic_string (const_pointer s, size_type n)
    {
        if (n != npos)
            _p.v = native_type(s, n);
        else
            _p.v = native_type(s);
    }

    template <typename InputIt>
    basic_string (InputIt first, InputIt last)
    {
        _p.v = native_type(first, pfs::distance(first, last));
    }
    
    basic_string & operator = (native_reference rhs)
    {
        *_p = rhs;
        return *this;
    }

    operator const_native_reference () const
    {
        return *_p;
    }

    /**
     * Casts to native container reference
     */
    operator native_reference ()
    {
        return *_p;
    }
    
        size_type size () const
    {
        return _p->size();
    }
    
    size_type max_size () const
    {
        //return (INT_MAX)/sizeof(QChar) - 1;
        return pfs::numeric_limits<int>::max()/sizeof(QChar) - sizeof(native_type);
    }
    
    const_pointer data () const
    {
        return _p->data();
    }
    
    const_iterator begin () const
    {
        return _p->begin();
    }

    const_iterator end () const
    {
        return _p->end();
    }

    const_reverse_iterator rbegin () const
    {
        return const_reverse_iterator(_p->end());
    }

    const_reverse_iterator rend () const
    {
        return const_reverse_iterator(_p->begin());
    }

    value_type at (size_type pos) const
    {
        return _p->at(pos);
    }

//    int compare (size_type pos1, size_type count1
//            , basic_string const & rhs, size_type pos2, size_type count2) const
//    {
//        return _p->compare(pos1, count1, rhs._p, pos2, count2) ;
//    }
    
    void clear ()
    {
        _p->clear();
    }
    
    void erase (size_type index, size_type count)
    {
        _p->remove(static_cast<int>(index), static_cast<int>(count));
    }
    
    iterator erase (const_iterator first, const_iterator last)
    {
        const_iterator begin = _p->begin();
        size_type index = pfs::distance(begin, first);
        size_type count = pfs::distance(first, last);
        _p->remove(index, count);
        return _p->begin() + index;
    }
    
    void push_back (value_type ch)
    {
        _p->push_back(ch);
    }
    
    friend inline bool operator == (basic_string const & lhs, basic_string const & rhs)
    {
        return *lhs._p == *rhs._p;
    }

    friend inline bool operator != (basic_string const & lhs, basic_string const & rhs)
    {
        return *lhs._p != *rhs._p;
    }

    friend inline bool operator < (basic_string const & lhs, basic_string const & rhs)
    {
        return *lhs._p < *rhs._p;
    }

    friend inline bool operator <= (basic_string const & lhs, basic_string const & rhs)
    {
        return *lhs._p <= *rhs._p;
    }

    friend inline bool operator > (basic_string const & lhs, basic_string const & rhs)
    {
        return *lhs._p > *rhs._p;
    }

    friend inline bool operator >= (basic_string const & lhs, basic_string const & rhs)
    {
        return *lhs._p >= *rhs._p;
    }    
};

typedef basic_string<string_value<QChar, QString> > string;
typedef basic_string<string_ref<QChar, QString> >   string_reference;

}}} // pfs::traits::qt

namespace pfs {

template <>
struct iterator_traits<traits::qt::string::const_iterator> 
{
    typedef bidirectional_iterator_tag                   iterator_category;
    typedef typename traits::qt::string::value_type      value_type;
    typedef typename traits::qt::string::difference_type difference_type;
    typedef typename traits::qt::string::const_pointer   pointer;
    typedef typename traits::qt::string::const_reference reference;
};

template <>
struct iterator_traits<traits::qt::string::iterator>
{
    typedef bidirectional_iterator_tag                   iterator_category;
    typedef typename traits::qt::string::value_type      value_type;
    typedef typename traits::qt::string::difference_type difference_type;
    typedef typename traits::qt::string::pointer         pointer;
    typedef typename traits::qt::string::reference       reference;
};


inline int operator - (QChar a, QChar b)
{
    return a.unicode() - b.unicode();
}

inline int operator + (QChar a, QChar b)
{
    return a.unicode() + b.unicode();
}

template <>
inline bool is_alnum<QChar> (QChar c)
{
    return c.isLetterOrNumber();
}

template <>
inline bool is_alpha<QChar> (QChar c)
{
    return c.isLetter();
}

template <>
inline bool is_cntrl<QChar> (QChar c)
{
    if (c == QChar())
        return true;
    if (c.toLatin1() == 0)
        return false;
    return is_cntrl(c.toLatin1());
}

template <>
inline bool is_digit<QChar>(QChar c)
{
    return c.isDigit();
}

/* checks for any printable character except space */
template <>
inline bool is_graph<QChar> (QChar c)
{
    return c.isPrint() && ! c.isSpace();
}

template <>
inline bool is_lower<QChar> (QChar c)
{
    return c.isLower();
}

template <>
inline bool is_print<QChar> (QChar c)
{
    return c.isPrint();
}

template <>
inline bool is_punct<QChar> (QChar c)
{
    return c.isPunct();
}

template <>
inline bool is_space<QChar> (QChar c)
{
    return c.isSpace();
}

template <>
inline bool is_upper<QChar> (QChar c)
{
    return c.isUpper();
}

template <>
inline bool is_xdigit<QChar> (QChar c)
{
    return c.isDigit() 
            || (c.unicode() >= 'a' && c.unicode() <= 'f')
            || (c.unicode() >= 'A' && c.unicode() <= 'F');
}

} // pfs

namespace std {

inline ostream & operator << (ostream & os, QString const & s)
{
    os << s.toStdString();
    return os;
}

} // std

#if __OBSOLETE__

template <>
struct string_rep<QString> : public QString
{

    static size_type length (const_pointer p)
    {
        size_type n = 0;
        while (p[n] != 0)
            ++n;
        return n;
    }

        string_rep ()
        : d()
    {}

    int compare (size_type pos1, size_type count1
            , native_type const & rhs
            , size_type pos2, size_type count2) const
    {
        return d.midRef(pos1, count1).compare(rhs.midRef(pos2, count2));
    }
    
//    static size_type xfind (data_type const & d
//            , data_type const & rhs
//            , size_type pos)
//    {
//        int i = d.indexOf(rhs, pos); 
//        return i < 0 ? size_type(-1) : size_type(i);
//    }
//    
//    static size_type xfind (data_type const & d
//            , value_type c
//            , size_type pos)
//    {
//        int i = d.indexOf(c, pos);
//        return i < 0 ? size_type(-1) : size_type(i);
//    }
//
//    static size_type xrfind (data_type const & d
//            , data_type const & rhs
//            , size_type pos)
//    {
//        int i = d.lastIndexOf(rhs, pos);
//        return i < 0 ? size_type(-1) : size_type(i);
//    }
//    
//    static size_type xrfind (data_type const & d
//            , value_type c
//            , size_type pos)
//    {
//        int i = d.lastIndexOf(c, pos);
//        return i < 0 ? size_type(-1) : size_type(i);
//    }
    
   
//    static void xclear (data_type & d)
//    {
//        d.clear();
//    }

    void append (size_type count, const_reference ch)
    {
        d.append(QString(count, ch));
    }
    
    void append (const_pointer s, size_type count)
    {
        d.append(QString(s, count));
    }

//    static void xinsert (data_type & d, size_type index, size_type count, value_type ch)
//    {
//        d.insert(index, QString(int(count), ch));
//    }
//    
//    static void xinsert (data_type & d, size_type index, const_pointer s)
//    {
//        d.insert(index, QString(s));
//    }
//    
//    static void xinsert (data_type & d, size_type index, const_pointer s, size_type count)
//    {
//        d.insert(index, s, int(count));
//    }
//    
//    static void xpush_back (data_type & d, value_type ch)
//    {
//        d.append(ch);
//    }
//
//    static const_pointer xdata (data_type const & d)
//    {
//        return d.constData();
//    }
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
        : _d(static_cast<QString const &>(s).toUtf8())
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
class c_wstr<QString>
{
public:
    typedef string<QString> string_type;
    
private:
    std::wstring _d;
    
public:
    explicit c_wstr (string_type const & s)
        : _d(static_cast<QString const &>(s).toStdWString())
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

#endif

#endif /* __PFS_TRAITS_QT_STRING_HPP__ */

