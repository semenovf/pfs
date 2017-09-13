/* 
 * File:   string.hpp
 * Author: wladt
 *
 * Created on January 9, 2017, 3:21 PM
 */

#ifndef __PFS_TRAITS_STDCXX_STRING_HPP__
#define __PFS_TRAITS_STDCXX_STRING_HPP__

#include <string>
#include <cstring>
#include <pfs/iterator.hpp>
#include <pfs/cxxlang.hpp>
#include <pfs/traits/string_value_ref.hpp>

namespace pfs {
namespace stdcxx {

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
    
    typedef typename native_type::value_type       value_type;
    typedef typename native_type::pointer          pointer;
    typedef typename native_type::const_pointer    const_pointer;
    typedef typename native_type::reference        reference;
    typedef typename native_type::const_reference  const_reference;
    typedef typename native_type::iterator         iterator;
    typedef typename native_type::const_iterator   const_iterator;
    typedef typename native_type::reverse_iterator reverse_iterator;
    typedef typename native_type::const_reverse_iterator const_reverse_iterator;
    typedef typename native_type::difference_type  difference_type;
    typedef typename native_type::size_type        size_type;
    
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
        this->_p.v = native_type(first, last);
    }
    
    basic_string (size_type count, value_type ch)
        : _p(count, ch)
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
        return _p->max_size();
    }
    
    const_pointer data () const
    {
        return _p->data();
    }

    const_pointer c_str () const
    {
        return _p->c_str();
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
//        return _p->rbegin();
//    }
//
//    const_reverse_iterator rend () const
//    {
//        return _p->rend();
//    }

    bool empty () const
    {
        return _p->empty();
    }
    
    value_type at (size_type pos) const
    {
        return _p->at(pos);
    }

    int compare (size_type pos1, size_type count1
            , basic_string const & rhs, size_type pos2, size_type count2) const
    {
        return _p->compare(pos1, count1, *rhs._p, pos2, count2) ;
    }
    
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
    
    iterator erase (const_iterator first, const_iterator last)
    {
#if __cplusplus >= 201103L
        return native_type::erase(first, last);
#else
        //
        // C++ prior to C++11 
        // erase() has signature `iterator erase(iterator first, iterator last)`
        //
        const_iterator begin = _p->begin();
        size_type index = pfs::distance(begin, first);
        size_type count = pfs::distance(first, last);
        native_type::erase(index, count);
        return _p->begin() + index;
#endif
    }
    
    void push_back (value_type ch)
    {
        _p->push_back(ch);
    }
    
    basic_string & operator += (basic_string const & str)
    {
        *_p += *str._p;
        return *this;
    }

    basic_string & operator += (const_pointer s)
    {
        *_p += s;
        return *this;
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
    
    friend inline string_value_type operator + (basic_string const & lhs, basic_string const & rhs)
    {
        string_value_type result;
        *result._p = *lhs._p + *rhs._p;
        return result;
    }

    friend inline string_value_type operator + (const_pointer lhs, basic_string const & rhs)
    {
        string_value_type result;
        *result._p = lhs + *rhs._p;
        return result;
    }

//    friend inline string_value_type operator + (value_type lhs, basic_string const & rhs)
//    {
//        string_value_type result;
//        *result._p = lhs + *rhs._p;
//        return result;
//    }

    friend inline string_value_type operator + (basic_string const & lhs, const_pointer rhs)
    {
        string_value_type result;
        *result._p = *lhs._p + rhs;
        return result;
    }

//    friend inline string_value_type operator + (basic_string const & lhs, value_type rhs)
//    {
//        string_value_type result;
//        *result._p = *lhs._p + rhs;
//        return result;
//    }
    
    static size_type length (const_pointer str);
};

typedef basic_string<traits::string_value<char, std::string> >     string;
typedef basic_string<traits::string_value<wchar_t, std::wstring> > wstring;
typedef basic_string<traits::string_ref<char, std::string> >       string_reference;
typedef basic_string<traits::string_ref<wchar_t, std::wstring> >   wstring_reference;

#if __cplusplus >= 201103L
typedef basic_string<traits::string_value<char16_t, std::u16string>> u16string;
typedef basic_string<traits::string_value<char32_t, std::u32string>> u32string;
typedef basic_string<traits::string_ref<char16_t, std::u16string>>   u16string_reference;
typedef basic_string<traits::string_ref<char32_t, std::u32string>>   u32string_reference;
#endif

template <>
inline string::size_type string::length (const_pointer str)
{
    return ::strlen(str);
}

template <>
inline wstring::size_type wstring::length (const_pointer str)
{
    return ::wcslen(str);
}

}} // pfs::stdcxx

#endif /* __PFS_TRAITS_STDCXX_STRING_HPP__ */
