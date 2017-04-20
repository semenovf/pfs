/* 
 * File:   database.hpp
 * Author: wladt
 *
 * Created on February 8, 2017, 4:02 PM
 */

#ifndef __PFS_DB_DATABASE_HPP__
#define __PFS_DB_DATABASE_HPP__

#include <pfs/traits/string.hpp>
#include <pfs/traits/string_builder.hpp>
#include <pfs/traits/db/exception.hpp>

namespace pfs {
namespace db {

template <typename CharT, typename DatabaseTag>
struct database_traits
{
    typedef traits::string<CharT> string_type;
    typedef traits::c_str<CharT>  c_str;
    typedef traits::string_builder<CharT> string_builder;
    typedef struct {}             data_type;
    
    class exception : public db::exception<CharT>
    {};
    
    static bool xopen (data_type & d, string_type const & uri);
    static bool xopen (data_type & d, CharT const * uri);
    static void xclose (data_type & d);
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
    
    bool open (string_type const & uri)
    {
        return traits_type::xopen(_d, uri);
    }

    bool open (CharT const * uri)
    {
        return traits_type::xopen(_d, uri);
    }

    void close ()
    {
        traits_type::xclose(_d);
    }
};

}} // pfs::db

#endif /* __PFS_DB_DATABASE_HPP__ */

