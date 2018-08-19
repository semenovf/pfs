#pragma once
#include <pfs/sql/sqlite3/sqlite3.h>
#include <pfs/utility.hpp>
#include <pfs/string.hpp>
#include <pfs/system_error.hpp>
#include <pfs/sql/sqlite3/result_code.hpp>
#include <pfs/sql/sqlite3/result.hpp>
#include <pfs/sql/sqlite3/private_data.hpp>
#include <pfs/sql/sqlite3/binder.hpp>
#include <pfs/sql/exception.hpp>

namespace pfs {
namespace sql {
namespace sqlite3 {

template <typename StringT = pfs::string>
class statement
{
    typedef StringT                 string_type;
    typedef result<string_type>     result_type;
    typedef stmt_native_handle_type native_handle_type;

    stmt_handle_shared _pd;

public:
    statement () {}
    statement (native_handle_type sth) : _pd(sth, stmt_handle_deleter()) {}

    native_handle_type native_handle () const
    {
        return _pd.get();
    }

    operator bool () const
    {
        return _pd.get() != 0;
    }

    id::native_type last_insert_id () const
    {
        db_native_handle_type dbh = sqlite3_db_handle(_pd.get());
        return sqlite3_last_insert_rowid(dbh);
    }

    result_type exec (pfs::error_code & ec, string_type & errstr)
    {
        if (!_pd) {
            ec = pfs::make_error_code(sql_errc::bad_handle);
            errstr = "statement is not initialized";
            return result_type();
        }

        // If the most recent call to sqlite3_step(S) for the prepared
        // statement S returned SQLITE_ROW or SQLITE_DONE, or if sqlite3_step(S)
        // has never before been called on S, then sqlite3_reset(S)
        // returns SQLITE_OK.
//        sqlite3_reset(_pd.get());
//        sqlite3_clear_bindings(_pd.get());

        int rc = sqlite3_step(_pd.get());

        if (!(rc == SQLITE_ROW || rc == SQLITE_DONE || rc == SQLITE_OK)) {
            db_native_handle_type dbh = sqlite3_db_handle(_pd.get());
            ec = pfs::make_error_code(pfs::sql_errc::query_fail);
            errstr = result_code<string_type>::errorstr(sqlite3_errmsg(dbh), rc);

            return result_type();
        }

        return result_type(_pd, rc);
    }

    //
    // The leftmost SQL parameter has an index of 1 for sqlite3_bind_*()
    // routines, but 'debby' starts field numbering at 0.
    //
    template <typename T>
    bool bind (int index, T const & value, pfs::error_code & ec, string_type & errstr)
    {
        return details::binder<T, string_type>()(_pd.get(), index + 1, value, ec, errstr);
    }

    bool bind (int index, char const * value, pfs::error_code & ec, string_type & errstr)
    {
        // If the fourth parameter to sqlite3_bind_text() or sqlite3_bind_text16()
        // is negative, then the length of the string is the number of bytes up
        // to the first zero terminator.
        int rc = sqlite3_bind_text(_pd.get(), index + 1, value, -1, SQLITE_STATIC);
        return (rc != SQLITE_OK) ? details::binder_fail<StringT>(_pd.get(), rc, ec, errstr) : true;
    }
};

}}} // pfs::sql::sqlite3
