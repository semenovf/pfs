/* 
 * File:   statement.hpp
 * Author: wladt
 *
 * Created on April 27, 2017, 3:38 PM
 */

#ifndef __PFS_TRAITS_DB_STATEMENT_HPP__
#define __PFS_TRAITS_DB_STATEMENT_HPP__

#include <pfs/traits/string.hpp>
#include <pfs/traits/db/exception.hpp>

namespace pfs {
namespace db {

template <typename StringT, typename StatementRep>
struct statement_traits
{
    typedef traits::string<StringT> string_type;
    typedef traits::c_str<StringT>  c_str;
    typedef db::exception<StringT>  exception_type;
    typedef StatementRep            rep_type;
};

template <typename StringT, typename StatementRep>
class statement
{
public:
    typedef statement_traits<StringT, StatementRep> traits_type;
    typedef typename traits_type::string_type       string_type;
    typedef typename traits_type::exception_type    exception_type;
    typedef typename traits_type::rep_type          rep_type;

private:
    rep_type _d;

private:
    // Avoid CopyConstructable and CopyAssignable
    statement (statement const &);
    statement & operator = (statement const &);

public:
    statement ()
        : _d()
    {}
};

}} // pfs::db

#endif /* __PFS_TRAITS_DB_STATEMENT_HPP__ */

