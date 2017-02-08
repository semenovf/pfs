/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   exception.hpp
 * Author: wladt
 *
 * Created on February 8, 2017, 4:23 PM
 */

#ifndef __PFS_DB_EXCEPTION_HPP__
#define __PFS_DB_EXCEPTION_HPP__

#include <pfs/traits/string.hpp>

namespace pfs {
namespace db {

template <typename CharT>
class exception
{
public:
    typedef pfs::traits::string<CharT> string_type;
        
protected:
    string_type _msg;
    
public:
    explicit exception (string_type const & msg)
        : _msg(msg)
    {}

    string_type const & what () const
    {
        return _msg;
    }
};

}} // pfs::db;

#endif /* __PFS_DB_EXCEPTION_HPP__ */

