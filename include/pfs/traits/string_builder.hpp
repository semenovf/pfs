/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   string_builder.hpp
 * Author: wladt
 *
 * Created on January 24, 2017, 2:22 PM
 */

#ifndef __PFS_TRAITS_STRING_BUILDER_HPP__
#define __PFS_TRAITS_STRING_BUILDER_HPP__

#include <pfs/type_traits.hpp>
#include <pfs/string.hpp>

namespace pfs {
namespace traits {

template <typename CharT>
struct string_builder_traits
{
    typedef CharT *       native_type;
    typedef size_t        size_type;
    typedef CharT         value_type;
    typedef CharT const * const_pointer;
    typedef native_type   data_type;
    
    static void xpush_back (data_type & d, const_pointer s, size_type n);
    static void xpush_back (data_type & d, size_type n, value_type c);
};

template <typename CharT>
class string_builder
{
public:
    typedef string_builder_traits<CharT>        traits_type;
    typedef typename traits_type::size_type     size_type;
    typedef typename traits_type::value_type    value_type;
    typedef typename traits_type::const_pointer const_pointer;
    typedef typename traits_type::data_type     data_type;

private:
    data_type _d;

public:
    string_builder () 
        : _d()
    {}

    template <typename CharU>
    string_builder & push_back (traits::string<CharU> const & s);

    template <typename StringT>
    string_builder & push_back (StringT const & s);

//    template <typename StringT>
//    string_builder & push_back (enable_if<! is_same<CharT, char>, char>::value_type const * s);

    string_builder & push_back (const_pointer s)
    {
        traits_type::xpush_back(_d, s, size_type(-1));
        return *this;
    }
    
    string_builder & push_back (const_pointer s, size_type n)
    {
        traits_type::xpush_back(_d, s, n);
        return *this;
    }
    
    string_builder & push_back (value_type c)
    {
        traits_type::xpush_back(_d, 1, c);
        return *this;
    }
    
    string_builder & push_back (size_type n, value_type c)
    {
        traits_type::xpush_back(_d, n, c);
        return *this;
    }

    template <typename StringT>
    StringT str () const;
};

}} // pfs

#endif /* __PFS_TRAITS_STRING_BUILDER_HPP__ */

