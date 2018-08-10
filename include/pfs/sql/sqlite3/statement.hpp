#pragma once
#include <pfs/sql/sqlite3/sqlite3.h>
#include <pfs/string.hpp>
#include <pfs/system_error.hpp>
#include <pfs/sql/sqlite3/result_code.hpp>
#include <pfs/sql/sqlite3/id.hpp>
#include <pfs/sql/exception.hpp>

namespace pfs {
namespace sql {
namespace sqlite3 {

template <typename StringT = pfs::string>
class statement
{
    typedef StringT          string_type;
    typedef struct sqlite3 * db_native_handle_type;
    typedef struct sqlite3_stmt * native_handle_type;

private:
    native_handle_type _h;
    int _nrows;
    int _last_id;
    int _column_count;

    void reset ()
    {
        _nrows        = -1;
        _last_id      =  0;
        _column_count =  0;
    }

public:
    statement () : _h(0) { reset(); }
    // statement (statement const & rhs) = default;
    // statement & operator = (statement const & rhs) = default;

    native_handle_type native_handle () const
    {
        return _h;
    }

    ~statement ()
    {
        if (_h) {
            int rc = sqlite3_finalize(_h);
            _h = 0;
            // TODO Here may be check the result code and output error string
            // without throwing an exception
        }
    }

    bool prepare (db_native_handle_type dbh
            , string_type const & query
            , pfs::error_code & ec
            , string_type & errstr)
    {
        unsigned int prep_flags = 0;
        std::string utf8_query(query.utf8());

        int rc = sqlite3_prepare_v3(dbh // Database handle
                , utf8_query.c_str()     // SQL statement, UTF-8 encoded
                , utf8_query.size()      // Maximum length of zSql in bytes
                , prep_flags             // Zero or more SQLITE_PREPARE_ flags
                , & _h                   // OUT: Statement handle
                , NULL);                 // OUT: Pointer to unused portion of zSql (unsigned)

        if (rc != SQLITE_OK) {
            ec = pfs::make_error_code(pfs::sql_errc::query_fail);
            errstr = result_code<string_type>::errorstr(sqlite3_errmsg(dbh), rc);
            return false;
        }

        return true;
    }

    bool exec (pfs::error_code & ec, string_type & errstr)
    {
        if (_h) {
            ec = pfs::make_error_code(sql_errc::bad_handle);
            errstr = "statement is not initialized";
            return false;
        }

        int rc = SQLITE_OK;
        db_native_handle_type dbh = sqlite3_db_handle(_h);

        reset();

        // Return the number of columns in the result set returned by the prepared
        // statement. If this routine returns 0, that means the prepared statement
        // returns no data (for example an UPDATE). However, just because this
        // routine returns a positive number does not mean that one or more rows
        // of data will be returned. A SELECT statement will always have a positive
        // sqlite3_column_count() but depending on the WHERE clause constraints
        // and the table content, it might return no rows.
        _column_count = sqlite3_column_count(_h);

        if (_column_count > 0) {
            rc = sqlite3_reset(_h);
        } else {
            rc = sqlite3_step(_h);

            _nrows   = sqlite3_changes(dbh);
            _last_id = sqlite3_last_insert_rowid(dbh);

            if (rc == SQLITE_DONE) {
                // If the most recent call to sqlite3_step(S) for the prepared
                // statement S returned SQLITE_ROW or SQLITE_DONE,
                // or if sqlite3_step(S) has never before been called on S,
                // then sqlite3_reset(S) returns SQLITE_OK.
                sqlite3_reset(_h);

                // sqlite3_reset() does not reset the bindings on a prepared
                // statement. Use this routine to reset all host parameters to NULL.
                sqlite3_clear_bindings(_h);
            }
        }

        if (rc != SQLITE_OK && rc != SQLITE_DONE) {

            ec = pfs::make_error_code(pfs::sql_errc::query_fail);
            errstr = result_code<string_type>::errorstr(sqlite3_errmsg(dbh), rc);

            return false;
        }

        return true;
    }

    id::native_type last_insert_id ()
    {
        return _last_id;
    }
};

}}} // pfs::sql::sqlite3
