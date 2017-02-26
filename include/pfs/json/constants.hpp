/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

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

enum value_type_enum
{
      null_value
    , boolean_value
    , integer_value
    , uinteger_value
    , real_value
    , string_value
    , object_value
    , array_value
};

}} // pfs::json

#endif /* __PFS_JSON_CONSTANTS_HPP__ */

