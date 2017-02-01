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

#ifndef __PFS_FOUNDATION_STDCXX_STRING_BUILDER_HPP__
#define __PFS_FOUNDATION_STDCXX_STRING_BUILDER_HPP__

#include <string>
#include <pfs/foundation/stdcxx/string.hpp>
#include <pfs/traits/string_builder.hpp>

namespace pfs {
namespace traits {

template <>
class basic_string_builder<char> : public details::basic_string_builder<char>
{
    typedef details::basic_string_builder<char> base_type;
    typedef std::string data_type;
    
public:    
    typedef traits::string<std::string>     string_type;
    typedef typename string_type::size_type size_type;
    
protected:
    data_type _d;
};

template <>
inline string_builder<char>::string_builder ()
{}

template <>
template <>
inline string_builder<char> & string_builder<char>::push_back<std::string> (std::string const & s)
{
    _d.append(s);
    return *this;
}

template <>
inline string_builder<char> & string_builder<char>::push_back (char const * s)
{
    _d.append(s);
    return *this;
}

template <>
inline string_builder<char> & string_builder<char>::push_back (char const * s, size_type n)
{
    _d.append(s, n);
    return *this;
}

template <>
inline string_builder<char> & string_builder<char>::push_back (value_type c)
{
    _d.append(1, c);
    return *this;
}

template <>
inline string_builder<char> & string_builder<char>::push_back (size_type n, value_type c)
{
    _d.append(n, c);
    return *this;
}

template <>
template <>
std::string string_builder<char>::str<std::string> () const
{
    return _d;
}

template <>
template <>
traits::string<std::string> string_builder<char>::str<traits::string<std::string> > () const
{
    return traits::string<std::string>(_d);
}

template <>
template <>
char const * string_builder<char>::str<char const *> () const
{
    return _d.data();
}

}} // pfs::traits

#endif /* __PFS_FOUNDATION_STDCXX_STRING_BUILDER_HPP__ */

