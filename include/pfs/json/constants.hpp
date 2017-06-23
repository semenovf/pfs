/* 
 * File:   constants.hpp
 * Author: wladt
 *
 * Created on February 26, 2017, 2:50 PM
 */

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
    enum type {
#endif
          null = 0
        , boolean
        , integer
        , uinteger
        , real
        , string
        , object
        , array
#if __cplusplus < 201103L
    };
#endif    
};

#if __cplusplus >= 201103L
    typedef data_type data_type_t;
#else
    typedef data_type::type data_type_t;
#endif

}} // pfs::json

#endif /* __PFS_JSON_CONSTANTS_HPP__ */

