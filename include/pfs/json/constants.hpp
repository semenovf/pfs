#ifndef __PFS_JSON_CONSTANTS_HPP__
#define __PFS_JSON_CONSTANTS_HPP__

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

}} // pfs::json

#endif /* __PFS_JSON_CONSTANTS_HPP__ */

