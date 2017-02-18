#ifndef __PFS_JSON_JSON_HPP__
#define __PFS_JSON_JSON_HPP__
 
#include <pfs/json/value.hpp>

namespace pfs { namespace json {

typedef value json;

bool parse (json & result, string const & s);

inline json parse (string const & s)
{
    json result;
    parse(result, s);
    return result;
}

template <typename T>
pfs::json::value to_json (T const & v, bool plain = false)
{
    return pfs::json::value(); // default is null value
}

}} // pfs

#endif /* __PFS_JSON_JSON_HPP__ */
