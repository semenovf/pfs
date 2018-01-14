#ifndef __PFS_DB_STATEMENT_HPP__
#define __PFS_DB_STATEMENT_HPP__

#include <pfs/string.hpp>
#include <pfs/db/exception.hpp>

namespace pfs {
namespace db {

template <typename StringImplType, typename StatementRep>
struct statement_traits
{
    typedef string<StringImplType>     string_type;
    typedef db::exception<string_type> exception_type;
    typedef StatementRep               rep_type;
};

template <typename Traits>
class statement
{
public:
    typedef Traits                               traits_type;
    typedef typename traits_type::string_type    string_type;
    typedef typename traits_type::exception_type exception_type;
    typedef typename traits_type::rep_type       rep_type;

private:
    rep_type _d;

#if __cplusplus >= 201103L
    // Avoid CopyConstructable and CopyAssignable
    statement (statement const &) = delete;
    statement & operator = (statement const &) = delete;
#else
private:
    statement (statement const &);
    statement & operator = (statement const &);
#endif

public:
    statement ()
        : _d()
    {}
};

}} // pfs::db

#endif /* __PFS_DB_STATEMENT_HPP__ */

