#ifndef __PFS_JSON_CONSTANTS_HPP__
#define __PFS_JSON_CONSTANTS_HPP__

#include <pfs/types.hpp>

namespace pfs {
namespace json {

#if __cplusplus >= 201103L
enum class data_type
{
#else
struct data_type
{
    enum value_enum {
#endif
          null = 0
        , boolean
        , integer
        , real
        , string
        , object
        , array
#if __cplusplus < 201103L
    };

    value_enum v;

    data_type (value_enum x)
        : v(x)
    {}

    data_type & operator = (value_enum x)
    {
        v = x;
        return *this;
    }

    operator int () const
    {
        return static_cast<int>(v);
    }
#endif
};

typedef data_type data_type_t;

static int8_t const UBJSON_CHAR_UNSPEC       = 0; // Special value unspecified
static int8_t const UBJSON_CHAR_NOOP         = static_cast<int8_t>('N');
static int8_t const UBJSON_CHAR_NULL         = static_cast<int8_t>('Z');
static int8_t const UBJSON_CHAR_TRUE         = static_cast<int8_t>('T');
static int8_t const UBJSON_CHAR_FALSE        = static_cast<int8_t>('F');
static int8_t const UBJSON_CHAR_INT8         = static_cast<int8_t>('i');
static int8_t const UBJSON_CHAR_UINT8        = static_cast<int8_t>('U');
static int8_t const UBJSON_CHAR_INT16        = static_cast<int8_t>('I');
static int8_t const UBJSON_CHAR_INT32        = static_cast<int8_t>('l');
static int8_t const UBJSON_CHAR_INT64        = static_cast<int8_t>('L');
static int8_t const UBJSON_CHAR_REAL32       = static_cast<int8_t>('d');
static int8_t const UBJSON_CHAR_REAL64       = static_cast<int8_t>('D');
static int8_t const UBJSON_CHAR_CHAR         = static_cast<int8_t>('C');
static int8_t const UBJSON_CHAR_STRING       = static_cast<int8_t>('S');
static int8_t const UBJSON_CHAR_ARRAY_BEGIN  = static_cast<int8_t>('[');
static int8_t const UBJSON_CHAR_ARRAY_END    = static_cast<int8_t>(']');
static int8_t const UBJSON_CHAR_OBJECT_BEGIN = static_cast<int8_t>('{');
static int8_t const UBJSON_CHAR_OBJECT_END   = static_cast<int8_t>('}');
static int8_t const UBJSON_CHAR_TYPE         = static_cast<int8_t>('$');
static int8_t const UBJSON_CHAR_SIZE         = static_cast<int8_t>('#');

}} // pfs::json

#endif /* __PFS_JSON_CONSTANTS_HPP__ */

