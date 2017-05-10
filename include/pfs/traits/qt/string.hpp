/* 
 * File:   string.hpp
 * Author: wladt
 *
 * Created on January 11, 2017, 4:05 PM
 */

#ifndef __PFS_TRAITS_QT_STRING_HPP__
#define __PFS_TRAITS_QT_STRING_HPP__

#include <QString>
#include <pfs/iterator.hpp>

namespace pfs {
namespace traits {
namespace qt {

class string
{
public:
    typedef QString                      native_type;
    
    typedef native_type &                native_reference;
    typedef native_type const &          const_native_reference;
    typedef native_type::value_type      value_type;
    typedef QChar const *                const_pointer;
    typedef typename native_type::reference       reference;
    typedef native_type::const_reference          const_reference;
    typedef native_type::iterator                 iterator;
    typedef native_type::const_iterator           const_iterator;
    typedef std::reverse_iterator<iterator>       reverse_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
//    typedef native_type::difference_type          difference_type;
    typedef int                                   size_type; // no native_type::size_type found, but documentation mentions.

protected:
    native_type * _p;
    
public:
    string (native_reference rhs)
        : _p(& rhs)
    {}
        
    string & operator = (native_reference rhs)
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
};

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
//    
//    static size_type xmax_size (data_type const &)
//    {
//        //return (INT_MAX)/sizeof(QChar) - 1;
//        return pfs::numeric_limits<int>::max()/sizeof(QChar) - sizeof(native_type);
//    }
//    
    const_reverse_iterator rbegin () const
    {
        return const_reverse_iterator(d.end());
    }
    
    const_reverse_iterator rend () const
    {
        return const_reverse_iterator(d.begin());
    }
    
//    static value_type xat (data_type const & d, size_type pos)
//    {
//        return d.at(int(pos));
//    }
//
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

