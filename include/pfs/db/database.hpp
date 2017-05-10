/* 
 * File:   database.hpp
 * Author: wladt
 *
 * Created on February 8, 2017, 4:02 PM
 */

#ifndef __PFS_DB_DATABASE_HPP__
#define __PFS_DB_DATABASE_HPP__

#include <pfs/traits/string.hpp>
#include <pfs/traits/db/exception.hpp>

namespace pfs {
namespace db {

template <typename StringT, typename DatabaseTag>
struct database_rep
{
    typedef StringT                   string_type;
    typedef traits::c_str<StringT>    c_str;
    typedef db::exception<StringT>    exception;
};

template <typename DatabaseRep>
class database
{
public:
    typedef typename DatabaseRep               rep_type;
    typedef typename rep_type::string_type     string_type;
    typedef typename rep_type::exception       exception;

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
        return _d.open(uri);
    }

    void close ()
    {
        _d.close();
    }
};

}} // pfs::db

#endif /* __PFS_DB_DATABASE_HPP__ */

