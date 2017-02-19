/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   traits.hpp
 * Author: wladt
 *
 * Created on February 19, 2017, 3:39 PM
 */

#ifndef __PFS_JSON_TRAITS_HPP__
#define __PFS_JSON_TRAITS_HPP__

#include <pfs/traits/map.hpp>
#include <pfs/traits/vector.hpp>
#include <pfs/traits/string.hpp>
#include <pfs/limits.hpp>
#include <pfs/utility.hpp>

namespace pfs {
namespace json {

#define PFS_JSON_TRAITS_ARGS \
          typename CharT      // char\
        , typename BoolT      // bool\
        , typename IntegerT   // intmax_t\
        , typename UIntegerT  // uintmax_t\
        , typename RealT      // double\
        , template <typename> class ArrayT\
        , template <typename, typename> class ObjectT

template <typename CharT      // char
        , typename BoolT      // bool
        , typename IntegerT   // intmax_t
        , typename UIntegerT  // uintmax_t
        , typename RealT      // double
        , template <typename> class ArrayT
        , template <typename, typename> class ObjectT>
struct traits
{
    typedef ptrdiff_t     difference_type;
    typedef size_t        size_type;

    enum type_enum
    {
          type_null
        , type_boolean
        , type_integer
        , type_uinteger
        , type_float
        , type_string
        , type_object
        , type_array
    };
    
    typedef BoolT                      boolean_type;
    typedef IntegerT                   integer_type;
    typedef UIntegerT                  uinteger_type;
    typedef RealT                      real_type;
    typedef pfs::traits::string<CharT> string_type;

    union value_rep
    {
        boolean_type  boolean;
        integer_type  integer;
        real_type     real;
        string_type * string;
        array_type *  array;
        object_type * object;

        value_rep (boolean_type v)
            : boolean (v)
        {}

        value_rep (integer_type v) 
            : integer (v)
        {}

        value_rep (real_type v) 
            : real (v)
        {}

        value_rep ()
        {}

        value_rep (string_type const & v)
        {
            std::allocator<string_type> alloc;
            string = alloc.allocate(1);
            alloc.construct(string, v);
        }

        value_rep (array_type const & v)
        {
            std::allocator<array_type> alloc;
            array = alloc.allocate(1);
            alloc.construct(array, v);
        }

        value_rep (object_type const & v)
        {
            std::allocator<object_type> alloc;
            object = alloc.allocate(1);
            alloc.construct(object, v);
        }

        value_rep (type_enum t);
    };
    
    typedef pfs::traits::vector<value_rep, ArrayT>            array_type;
    typedef pfs::traits::map<string_type, value_rep, ObjectT> object_type;
};

}} // pfs::json

#endif /* __PFS_JSON_TRAITS_HPP__ */

