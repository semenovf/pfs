/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   regex.hpp
 * Author: wladt
 *
 * Created on February 2, 2017, 10:09 AM
 */

#ifndef __PFS_TRAITS_REGEX_HPP__
#define __PFS_TRAITS_REGEX_HPP__

//#include <bits/regex.h>

namespace pfs {
namespace traits {

template <typename CharT>
struct regex_traits
{
    typedef T const &                          const_native_reference;
    typedef typename T::size_type              size_type;
    typedef typename T::value_type             value_type;
    typedef typename T::const_pointer          const_pointer;
    typedef typename T::const_iterator         const_iterator;
    typedef typename T::const_reverse_iterator const_reverse_iterator;
    
    typedef T data_type;
};

template <typename CharT, typename Traits>
class basic_regex;

namespace details {

template <typename CharT>
class basic_regex
{};

} // details

template <typename CharT>
class regex : public basic_regex<CharT>
{
    typedef basic_regex<CharT> base_type;
    typedef CharT                               value_type;
    typedef typename traits_type::string_type   string_type;
    typedef regex_constants::syntax_option_type flag_type;
//    typedef typename traits_type::locale_type   locale_type;


public:
    typedef CharT value_type;
    typedef typename base_type::size_type size_type;

public:
    string_builder ();

    template <typename StringT>
    string_builder & push_back (traits::string<StringT> const & s)
    {
        this->push_back(s.native());
    }

    template <typename StringT>
    string_builder & push_back (StringT const & s);

    string_builder & push_back (char const * s);
    string_builder & push_back (char const * s, size_type n);
    string_builder & push_back (value_type c);
    string_builder & push_back (size_type n, value_type c);

    template <typename StringT>
    StringT str () const;
};

}} // pfs

#endif /* __PFS_TRAITS_REGEX_HPP__ */

