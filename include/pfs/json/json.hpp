#ifndef __PFS_JSON_JSON_HPP__
#define __PFS_JSON_JSON_HPP__
 
#include <pfs/json/value.hpp>

namespace pfs { namespace json {

#if __cplusplus >= 201103L

template <typename BoolT
        , typename IntT
        , typename RealT
        , typename StringT
        , template <typename> class ArrayT
        , template <typename> class ObjectT>
using json = value<BoolT, IntT, RealT, StringT, ArrayT, ObjectT>;

#else
#   error "Implement json for C++ prior to C++11"
#endif

//typedef value json;
//
//bool parse (json & result, string const & s);
//
//inline json parse (string const & s)
//{
//    json result;
//    parse(result, s);
//    return result;
//}

template <typename JsonValueT, typename T>
JsonValueT to_json (T const & v, bool plain = false);
//{
//    return pfs::json::value(); // default is null value
//}

}} // pfs

#endif /* __PFS_JSON_JSON_HPP__ */
