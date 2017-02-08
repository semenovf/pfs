/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   database.hpp
 * Author: wladt
 *
 * Created on February 8, 2017, 4:02 PM
 */

#ifndef __PFS_DB_DATABASE_HPP__
#define __PFS_DB_DATABASE_HPP__

#include <pfs/traits/string.hpp>
#include <pfs/db/exception.hpp>

namespace pfs {
namespace db {

template <typename CharT, typename DatabaseTag>
struct database_traits
{
    typedef string<CharT>    string_type;
    typedef struct {}        data_type;
    
    class exception : public db::exception<CharT>
    {};
    
    bool xconnect (string_type const & uri);
};

template <typename CharT, typename DatabaseTag>
class database
{
public:
    typedef database_traits<CharT, DatabaseTag> traits_type;
    typedef typename traits_type::string_type   string_type;
    typedef typename traits_type::exception     exception;
    typedef typename traits_type::data_type     data_type;

private:
    data_type _d;

private:
    // Avoid CopyConstructable and CopyAssignable
    database (database const &);
    database & operator = (database const &);

public:
    database ()
        : _d()
    {}
    
    void connect (string_type const & uri)
    {
        traits_type::xconnect(uri);
    }
};

}} // pfs::db

#endif /* __PFS_DB_DATABASE_HPP__ */

