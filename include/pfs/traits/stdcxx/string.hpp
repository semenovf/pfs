/* 
 * File:   string.hpp
 * Author: wladt
 *
 * Created on January 9, 2017, 3:21 PM
 */

#ifndef __PFS_TRAITS_STDCXX_STRING_HPP__
#define __PFS_TRAITS_STDCXX_STRING_HPP__

#include <string>
#include <pfs/iterator.hpp>

namespace pfs {
namespace traits {
namespace stdcxx {

template <typename CharT>
class basic_string
{
public:
    typedef typename std::basic_string<CharT>      native_type;
    
    typedef native_type &                          native_reference;
    typedef native_type const &                    const_native_reference;
    typedef typename native_type::value_type       value_type;
    typedef typename native_type::const_pointer    const_pointer;
    typedef typename native_type::reference        reference;
    typedef typename native_type::const_reference  const_reference;
    typedef typename native_type::iterator         iterator;
    typedef typename native_type::const_iterator   const_iterator;
    typedef typename native_type::reverse_iterator reverse_iterator;
    typedef typename native_type::const_reverse_iterator const_reverse_iterator;
    typedef typename native_type::difference_type difference_type;
    typedef typename native_type::size_type       size_type;

protected:
    native_type * _p;
    
public:
    basic_string (native_reference rhs)
        : _p(& rhs)
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
        return _p->rbegin();
    }

    const_reverse_iterator rend () const
    {
        return _p->rend();
    }

    value_type at (size_type pos) const
    {
        return _p->at(pos);
    }

    int compare (size_type pos1, size_type count1
            , basic_string const & rhs, size_type pos2, size_type count2) const
    {
        return _p->compare(pos1, count1, rhs._p, pos2, count2) ;
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
};

typedef basic_string<char> string;
typedef basic_string<wchar_t> wstring;

#if __cplusplus >= 201103L
typedef basic_string<char16_t> u16string;
typedef basic_string<char32_t> u32string;
#endif

}}} // pfs::traits::stdcxx

#if __OBSOLETE__

#include <string>
#include <cstring>
#include <pfs/traits/string.hpp>

namespace pfs {
namespace traits {

namespace stdcxx {

template <typename StringT>
class c_str
{
public:
    typedef string<StringT>                     string_type;
    typedef typename string_type::const_pointer const_pointer;
    
protected:
    string_type const & _d;
    
public:
    explicit c_str (string_type const & s)
        : _d(s)
    {}
    
    const_pointer operator () () const
    {
        return _d.data();
    }
    
    operator const_pointer () const
    {
        return _d.data();
    }
};

} // stdcxx

template <>
struct string_rep<std::string> 
    : public stdcxx::string_rep<char>
{
    typedef stdcxx::string_rep<char> base_class;
    
    string_rep ()
        : base_class()
    {}
    
    explicit string_rep (const_native_reference s)
        : base_class(s)
    {}

    explicit string_rep (const_pointer str, size_type n)
        : base_class(str, n)
    {}

    explicit string_rep (const_pointer str)
        : base_class(str)
    {}
    
    string_rep (const_iterator begin, const_iterator end)
        : base_class(begin, end)
    {}
    
    static size_type length (const_pointer p)
    {
        return std::strlen(p);
    }
};

template <>
struct string_rep<std::wstring> 
    : public stdcxx::string_rep<wchar_t>
{
    typedef stdcxx::string_rep<wchar_t> base_class;
    
    string_rep ()
        : base_class()
    {}
    
    explicit string_rep (const_native_reference s)
        : base_class(s)
    {}

    explicit string_rep (const_pointer str, size_type n)
        : base_class(str, n)
    {}

    explicit string_rep (const_pointer str)
        : base_class(str)
    {}
    
    string_rep (const_iterator begin, const_iterator end)
        : base_class(begin, end)
    {}
    
    static size_type length (const_pointer p)
    {
        return std::wcslen(p);
    }
};

template <>
class c_str<std::string> 
    : public traits::stdcxx::c_str<std::string>
{
    typedef traits::stdcxx::c_str<std::string> base_type;
public:
    explicit c_str (string_type const & s)
        : base_type(s)
    {}
};

template <>
class c_wstr<std::wstring> 
    : public traits::stdcxx::c_str<std::wstring>
{
    typedef traits::stdcxx::c_str<std::wstring> base_type;
public:
    explicit c_wstr (string_type const & s)
        : base_type(s)
    {}
};

template <>
inline int compare<std::string> (
          string<std::string> const & lhs
        , char const * rhs)
{
    return static_cast<std::string const &>(lhs).compare(rhs);
}

template <>
inline int compare<std::wstring> (
          string<std::wstring> const & lhs
        , wchar_t const * rhs)
{
    return static_cast<std::wstring const &>(lhs).compare(rhs);
}

}} // pfs::traits

#endif

#endif /* __PFS_TRAITS_STDCXX_STRING_HPP__ */
