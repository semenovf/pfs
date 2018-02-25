#ifndef __PFS_TRAITS_QT_STRING_HPP__
#define __PFS_TRAITS_QT_STRING_HPP__

#include <QString>
#include <QChar>
#include <istream>
#include <ostream>
#include <pfs/cxxlang.hpp>
#include <pfs/limits.hpp>
#include <pfs/ctype.hpp>
#include <pfs/byte_string.hpp>
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
        : _p()
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

    basic_string (std::string const & s)
        : _p(native_type::fromStdString(s))
    {}

    basic_string (std::wstring const & s)
        : _p(native_type::fromStdWString(s))
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

//    const_reverse_iterator rbegin () const
//    {
//        return const_reverse_iterator(_p->end());
//    }
//
//    const_reverse_iterator rend () const
//    {
//        return const_reverse_iterator(_p->begin());
//    }

    value_type at (size_type pos) const
    {
        return _p->at(pos);
    }

//    int compare (size_type pos1, size_type count1
//            , basic_string const & rhs, size_type pos2, size_type count2) const
//    {
//        return _p->compare(pos1, count1, rhs._p, pos2, count2) ;
//    }

    basic_string & append (size_type count, value_type ch)
    {
        _p->append(count, ch);
        return *this;
    }

    basic_string & append (const_pointer str, size_type count)
    {
        _p->append(str, count);
        return *this;
    }

    basic_string & append (const_native_reference s)
    {
        _p->append(s);
        return *this;
    }

    void clear ()
    {
        _p->clear();
    }

    const_iterator find (basic_string const & rhs, const_iterator pos) const
    {
        int p = pfs::distance(this->begin().base(), pos);
        int index = _p->indexOf(*rhs._p, p, Qt::CaseSensitive);
        return index < 0 ? this->end() : const_iterator(this->begin() + index);
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

typedef basic_string<traits::string_value<QChar, QString> > string;
typedef basic_string<traits::string_ref<QChar, QString> >   string_reference;

template <typename StringImplType, typename OStream>
OStream & output_utf8_string (OStream & os, StringImplType const & s)
{
    typedef pfs::ostream_iterator<OStream> ostream_iterator_type;
    QString const & native = static_cast<typename StringImplType::const_native_reference>(s);

    ostream_iterator_type osi(os);
    unicode::u8_output_iterator<ostream_iterator_type> it(osi);

    if (!native.isEmpty()) {
        for (QChar const * first = native.constData(); *first != QChar(); ++first) {
            *it++ = unicode::char_t(static_cast<intmax_t>(first->unicode()));
        }
    }
}

inline std::ostream & operator << (std::ostream & os, string const & s)
{
    return output_utf8_string(os, s);
}

inline std::ostream & operator << (std::ostream & os, string_reference const & s)
{
    return output_utf8_string(os, s);
}

inline byte_ostream & operator << (byte_ostream & os, string const & s)
{
    return output_utf8_string(os, s);
}

inline byte_ostream & operator << (byte_ostream & os, string_reference const & s)
{
    return output_utf8_string(os, s);
}

}} // pfs::qt

namespace pfs {

template <>
struct iterator_traits<qt::string::const_iterator>
{
    typedef bidirectional_iterator_tag           iterator_category;
    typedef typename qt::string::value_type      value_type;
    typedef typename qt::string::difference_type difference_type;
    typedef typename qt::string::const_pointer   pointer;
    typedef typename qt::string::const_reference reference;
};

template <>
struct iterator_traits<qt::string::iterator>
{
    typedef bidirectional_iterator_tag           iterator_category;
    typedef typename qt::string::value_type      value_type;
    typedef typename qt::string::difference_type difference_type;
    typedef typename qt::string::pointer         pointer;
    typedef typename qt::string::reference       reference;
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

#endif /* __PFS_TRAITS_QT_STRING_HPP__ */

