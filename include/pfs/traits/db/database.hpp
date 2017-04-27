/* 
 * File:   database.hpp
 * Author: wladt
 *
 * Created on February 8, 2017, 4:02 PM
 */

#ifndef __PFS_TRAITS_DB_DATABASE_HPP__
#define __PFS_TRAITS_DB_DATABASE_HPP__

#include <pfs/traits/string.hpp>
#include <pfs/traits/db/exception.hpp>

namespace pfs {
namespace db {

template <typename StringT, typename DatabaseTag>
struct database_rep
{};

template <typename StringT, typename DatabaseTag>
struct statement_rep
{};

template <typename StringT, typename DatabaseTag>
struct database_traits
{
    typedef traits::string<StringT>   string_type;
    typedef traits::c_str<StringT>    c_str;
    typedef database_rep<DatabaseTag> rep_type;
    typedef db::exception<StringT>    exception_type;
    
    static bool xopen (rep_type & d, string_type const & uri);
    static void xclose (rep_type & d);
};

template <typename StringT, typename DatabaseTag>
struct statement_traits
{
    typedef traits::string<StringT>    string_type;
    typedef traits::c_str<StringT>     c_str;
    typedef statement_rep<DatabaseTag> rep_type;
    typedef db::exception<StringT>     exception_type;
};

template <typename StringT, typename DatabaseTag>
class database
{
public:
    typedef database_traits<StringT, DatabaseTag> traits_type;
    typedef typename traits_type::string_type     string_type;
    typedef typename traits_type::exception_type  exception_type;
    typedef typename traits_type::rep_type        rep_type;

private:
    rep_type _d;

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

    void close ()
    {
        traits_type::xclose(_d);
    }
};

}} // pfs::db

#endif /* __PFS_TRAITS_DB_DATABASE_HPP__ */

