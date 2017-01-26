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

#ifndef __PFS_STRING_BUILDER_HPP__
#define __PFS_STRING_BUILDER_HPP__

#include <pfs/string.hpp>

namespace pfs {

template <typename T>
class basic_string_builder;

namespace details {

template <typename T>
class basic_string_builder
{};

} // details

template <typename T>
class string_builder : public basic_string_builder<T>
{
    typedef basic_string_builder<T> base_type;

public:
    typedef T value_type;
    typedef typename base_type::size_type              size_type;

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

} // pfs

#endif /* __PFS_STRING_BUILDER_HPP__ */

