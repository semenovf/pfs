/* 
 * File:   string.hpp
 * Author: wladt
 *
 * Created on January 11, 2017, 4:05 PM
 */

#ifndef __PFS_TRAITS_QT_STRING_HPP__
#define __PFS_TRAITS_QT_STRING_HPP__

#include <QString>
#include <pfs/cxxlang.hpp>
#include <pfs/iterator.hpp>
#include <pfs/traits/string_value_ref.hpp>

namespace pfs {
namespace traits {

template <>
template <typename InputIt>
string_value<QChar, QString>::string_value (InputIt first, InputIt last)
    : v(first, pfs::distance(first, last))
{}

namespace qt {

//template <typename CharT>
//struct string_value
//{
//    typedef QString             native_type;
//    typedef native_type &       native_reference;
//    typedef native_type const & const_native_reference;
//    
//    native_type v;
//
//    string_value ()
//    {}
//
//    string_value (CharT const * s)
//        : v(s)
//    {}
//
//    string_value (native_reference rhs)
//        : v(rhs)
//    {}
//
//    string_value (const_native_reference rhs)
//        : v(rhs)
//    {}
//    
//    template <typename InputIt>
//    string_value (InputIt first, InputIt last)
//        : v(first, pfs::distance(first, last))
//    {}
//
//    native_reference operator * ()
//    {
//        return v;
//    }
//    
//    const_native_reference operator * () const
//    {
//        return v;
//    }
//    
//    native_type * operator -> ()
//    {
//        return & v;
//    }
//    
//    native_type const * operator -> () const
//    {
//        return & v;
//    }
//};
//
//template <typename CharT>
//struct string_ref
//{
//    typedef QString             native_type;
//    typedef native_type &       native_reference;
//    typedef native_type const & const_native_reference;
//
//    native_type * p;
//    
//    string_ref ()
//    {
//        static_assert(false, "Constructor denied");
//    }
//
//    string_ref (CharT const * s)
//    {
//        static_assert(false, "Constructor denied");
//    }
//    
//    string_ref (native_reference rhs)
//        : p(& rhs)
//    {}
//
//    string_ref (const_native_reference rhs)
//    {
//        static_assert(false, "Constructor denied");
//    }
//
//    template <typename InputIt>
//    string_ref (InputIt first, InputIt last)
//    {
//        static_assert(false, "Constructor denied");
//    }
//
//    native_reference operator * ()
//    {
//        return *p;
//    }
//    
//    const_native_reference operator * () const
//    {
//        return *p;
//    }
//
//    native_type * operator -> ()
//    {
//        return p;
//    }
//    
//    native_type const * operator -> () const
//    {
//        return p;
//    }
//};

template <typename CharT, typename ValueOrReference>
class basic_string
{
    typedef ValueOrReference internal_type;
    
public:
    typedef basic_string<CharT, string_value<CharT, typename internal_type::native_type> > string_value_type;
    typedef basic_string<CharT, string_ref<CharT, typename internal_type::native_type> >   string_reference_type;

//    typedef basic_string<string_value<QChar> > string_value_type;
//    typedef basic_string<string_ref<QChar> >   string_reference_type;

    typedef typename internal_type::native_type            native_type;
    typedef typename internal_type::native_reference       native_reference;
    typedef typename internal_type::const_native_reference const_native_reference;
    
    typedef QChar                                 value_type;
    typedef QChar const *                         const_pointer;
    typedef typename native_type::reference       reference;
    typedef typename native_type::const_reference const_reference;
    typedef typename native_type::iterator        iterator;
    typedef typename native_type::const_iterator  const_iterator;
    typedef std::reverse_iterator<iterator>       reverse_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
//    typedef native_type::difference_type          difference_type;
    typedef int                                   size_type; // no native_type::size_type found, but documentation mentions.

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
    
    basic_string (const_pointer s)
        : _p(s)
    {}

    template <typename InputIt>
    basic_string (InputIt first, InputIt last)
        : _p(first, last)
    {}
    
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

typedef basic_string<QChar, string_value<QChar, QString> > string;
typedef basic_string<QChar, string_ref<QChar, QString> >   string_reference;

}}} // pfs::traits::qt

#if __OBSOLETE__

#include <QString>
#include <pfs/limits.hpp>
#include <pfs/traits/string.hpp>

namespace pfs {
namespace traits {

template <>
struct string_rep<QString> : public QString
{
    typedef QString                                      native_type;
    typedef native_type const &                          const_native_reference;
    typedef size_t                                       size_type;
    typedef typename native_type::value_type             value_type;
    typedef typename native_type::reference              reference;
    typedef typename native_type::const_reference        const_reference;
    typedef value_type *                                 pointer;
    typedef value_type const *                           const_pointer;
    typedef typename native_type::iterator               iterator;
    typedef typename native_type::const_iterator         const_iterator;
    typedef std::reverse_iterator<iterator>              reverse_iterator;
    typedef std::reverse_iterator<const_iterator>        const_reverse_iterator;

    native_type d;

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
    
    explicit string_rep (const_native_reference s)
        : d(s)
    {}

    explicit string_rep (const_pointer str, size_type n)
        : d(str, n)
    {}

    explicit string_rep (const_pointer str)
        : d(str)
    {}
    
    string_rep (const_iterator begin, const_iterator end)
        : d(begin, std::distance(begin,end))
    {}
    
//    static void xassign (data_type & d, const_native_reference lhs)
//    {
//        d = lhs;
//    }
//
//    static void xassign (data_type & d, const_pointer lhs, size_type n)
//    {
//        d = (n == size_type(-1)) 
//                ? data_type(lhs)
//                : data_type(lhs, int(n));
//    }

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

