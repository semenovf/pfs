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
#include <pfs/traits/string.hpp>

namespace pfs {
namespace traits {

namespace stdcxx {

template <typename CharT>
struct string_rep : public std::basic_string<CharT>
{
    typedef std::basic_string<CharT>                     native_type;
    typedef native_type const &                          const_native_reference;
    typedef typename native_type::size_type              size_type;
    typedef typename native_type::value_type             value_type;
    typedef typename native_type::reference              reference;
    typedef typename native_type::const_reference        const_reference;
    typedef typename native_type::pointer                pointer;
    typedef typename native_type::const_pointer          const_pointer;
    typedef typename native_type::iterator               iterator;
    typedef typename native_type::const_iterator         const_iterator;
    typedef typename native_type::reverse_iterator       reverse_iterator;
    typedef typename native_type::const_reverse_iterator const_reverse_iterator;
    
    native_type d;
    
    using native_type::erase;
    
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
        : d(begin, end)
    {}
    
    iterator erase (const_iterator first, const_iterator last)
    {
#if __cplusplus < 201103L
    //
    // C++ prior to C++11 
    // erase() has signature `iterator erase(iterator first, iterator last)`
    //
        const_iterator begin = d.begin();
        size_type index = pfs::distance(begin, first);
        size_type count = pfs::distance(first, last);
        d.erase(index, count);
        return d.begin() + index;
#else
        return d.erase(first, last);
#endif
    }
};

template <typename StringT>
class c_str
{
public:
    typedef string<StringT> string_type;
    typedef typename string_type::const_pointer const_pointer;
    
protected:
    string_type const & _d;
    
public:
    explicit c_str (string_type const & s)
        : _d(s)
    {}
    
    //CharT const * operator () () const
    const_pointer operator () () const
    {
        return _d.data();
    }
    
    //operator CharT const * () const
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
    : public stdcxx::c_str<std::string>
{
    typedef stdcxx::c_str<std::string> base_type;
public:
    explicit c_str (string_type const & s)
        : base_type(s)
    {}
};

template <>
class c_wstr<std::wstring> 
    : public stdcxx::c_str<std::wstring>
{
    typedef stdcxx::c_str<std::wstring> base_type;
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

#endif /* __PFS_TRAITS_STDCXX_STRING_HPP__ */
