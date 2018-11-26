#pragma once
#include <pfs/sql/sqlite3/sqlite3.h>
#include <pfs/string.hpp>
#include <pfs/stringlist.hpp>
#include <pfs/system_error.hpp>
#include <pfs/filesystem.hpp>
#include <pfs/regex.hpp>
#include <pfs/net/uri.hpp>
#include <pfs/sql/exception.hpp>
#include <pfs/sql/sqlite3/statement.hpp>
#include <pfs/sql/sqlite3/result.hpp>
#include <pfs/sql/sqlite3/private_data.hpp>

namespace pfs {
namespace sql {
namespace sqlite3 {

//
// Useful functions:
//
// 1. Databases's absolute file path for connection
//      char const * filename = sqlite3_db_filename(sqlite3 *, "main");
//
template <typename StringListT = pfs::stringlist<>, typename StringT = pfs::string>
class database
{
    static int const MAX_BUSY_TIMEOUT = 1000; // 1 second

public:
    typedef StringT               string_type;
    typedef StringListT           stringlist_type;
    typedef statement<StringT>    statement_type;
    typedef result<StringT>       result_type;
    typedef db_native_handle_type native_handle_type;

private:
    db_handle_shared _pd;

private:
    static bool query (db_native_handle_type dbh
        , string_type const & sql
        , pfs::error_code & ec
        , string_type & errstr)
    {
        char * errmsg;
        int rc = sqlite3_exec(dbh, sql.utf8().c_str(), NULL, NULL, & errmsg);

        if (SQLITE_OK != rc) {
            if (errmsg) {
                ec = pfs::make_error_code(pfs::sql_errc::query_fail);
                errstr = result_code<string_type>::errorstr(errmsg, rc);
                sqlite3_free(errmsg);
            }
            return false;
        }

        return true;
    }

    bool query (string_type const & sql, pfs::error_code & ec, string_type & errstr)
    {
        return query(_pd.get(), sql, ec, errstr);
    }

public:
    database () {}

    native_handle_type native_handle () const
    {
        return _pd.get();
    }

    /**
    * @brief Connects to sqlite3 database.
    *
    * @param db_uri Details see SQLite docs on sqlite3_open_v2 function.
    *        Examples:
    *            file:data.db
    *                Open the file "data.db" in the current directory.
    *
    *                  sqlite3:/home/fred/data.db
    *                  sqlite3:///home/fred/data.db
    *                  sqlite3://localhost/home/fred/data.db
    *                      Open the database file "/home/fred/data.db".
    *
    *                  sqlite3:///C:/Documents%20and%20Settings/fred/Desktop/data.db
    *                      Windows only: Open the file "data.db" on fred's desktop on drive C:.
    *                      Note that the %20 escaping in this example is not
    *                      strictly necessary - space characters can be used literally
    *                      in URI filenames.
    *
    *                  sqlite3:data.db?mode=ro&cache=private
    *                      Open file "data.db" in the current directory for read-only access.
    *                      Regardless of whether or not shared-cache mode is enabled
    *                      by default, use a private cache.
    *
    *                  sqlite3:/home/fred/data.db?vfs=unix-nolock
    *                      Open file "/home/fred/data.db". Use the special VFS "unix-nolock".
    *
    *                  sqlite3:data.db?mode=readonly
    *                      An error. "readonly" is not a valid option for the "mode" parameter.
    *
    *        Mode values:
    *            mode=ro (default)
    *            mode=rw
    *            mode=rwc
    *            mode=memory
    *
    *        Cache values:
    *            cache=shared
    *            cache=private
    *
    * @return DBI connection to specified sqlite3 databases.
    *
    * @note  Autocommit mode is on by default.
    */
    bool open (string_type const & db_uri, error_code & ec, string_type & errstr)
    {
        pfs::net::uri<string_type> uri;

        if (! db_uri.starts_with("sqlite3:")) {
            ec = make_error_code(sql_errc::bad_uri);
            return false;
        }

        if (!uri.parse(db_uri)) {
            ec = make_error_code(sql_errc::bad_uri);
            return false;
        }

        int flags = SQLITE_OPEN_URI;

        //
        // It is an error to specify a value for the mode parameter
        // that is less restrictive than that specified by the flags passed
        // in the third parameter to sqlite3_open_v2().
        //
        flags |= SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE;

        pfs::net::uri<string_type> filename;
        filename.set_scheme("file");
        filename.set_path(uri.path());
        filename.set_query(uri.query());

        db_native_handle_type dbh = 0;

        int rc = sqlite3_open_v2(filename.to_string().utf8().c_str(), & dbh, flags, NULL);

        if (rc != SQLITE_OK) {
            if (!dbh) {
                // Unable to allocate memory for database handler.
                // Internal error code.
                ec = make_error_code(sql_errc::bad_alloc);
            } else {
                switch (rc) {
                case SQLITE_CANTOPEN:
                    ec = make_error_code(sql_errc::open_fail);
                    errstr = result_code<string_type>::errorstr(sqlite3_errstr(rc), rc);
                    return false;
                default:
                    ec = make_error_code(sql_errc::specific_error);
                    errstr = result_code<string_type>::errorstr(sqlite3_errstr(rc), rc);
                    break;
                }

                sqlite3_close_v2(dbh);
                dbh = 0;
            }

            return false;
        } else {
            // TODO what for this call ?
            sqlite3_busy_timeout(dbh, MAX_BUSY_TIMEOUT);

            // Enable extended result codes
            sqlite3_extended_result_codes(dbh, 1);
        }

        if (!query(dbh, string_type("PRAGMA foreign_keys = ON"), ec, errstr))
            return false;

        db_handle_shared pd(dbh, db_handle_deleter());
        _pd.swap(pd);
        return true;
    }

    void close ()
    {
        db_handle_shared pd;
        _pd.swap(pd);
    }

    bool opened () const
    {
        return _pd.get() != 0;
    }

    bool clear (pfs::error_code & ec, string_type & errstr)
    {
        stringlist_type tlist = this->tables(string_type(), ec, errstr);

        if (ec)
            return false;

        typename stringlist_type::const_iterator first = tlist.cbegin();
        typename stringlist_type::const_iterator last = tlist.cend();

        if (!query(_pd.get(), string_type("PRAGMA foreign_keys = OFF"), ec, errstr))
            return false;

        for (; first != last; ++first) {
            string_type sql("DROP TABLE IF EXISTS [");
            sql += *first;
            sql += ']';
            result_type res = this->exec(sql, ec, errstr);

            if (!res) return false;
        }

        return query(_pd.get(), string_type("PRAGMA foreign_keys = ON"), ec, errstr);
    }

    result_type exec (string_type const & sql, pfs::error_code & ec, string_type & errstr)
    {
        statement_type stmt = prepare(sql, ec, errstr);

        if (! stmt)
            return result_type();

        result_type res = stmt.exec(ec, errstr);

        if (!res)
            return result_type();

        return res;
    }

    //
    // Used by debby::database to create statement
    //
    statement_type prepare (string_type const & sql
            , pfs::error_code & ec
            , string_type & errstr)
    {
        unsigned int prep_flags = 0;
        stmt_native_handle_type sth;
        std::string utf8_query(sql.utf8());

        int rc = sqlite3_prepare_v3(_pd.get() // Database handle
                , utf8_query.c_str()          // SQL statement, UTF-8 encoded
                , utf8_query.size()           // Maximum length of zSql in bytes
                , prep_flags                  // Zero or more SQLITE_PREPARE_ flags
                , & sth                       // OUT: Statement handle
                , NULL);                      // OUT: Pointer to unused portion of zSql (unsigned)

        if (rc != SQLITE_OK) {
            ec = pfs::make_error_code(pfs::sql_errc::query_fail);
            errstr = result_code<string_type>::errorstr(sqlite3_errmsg(_pd.get()), rc);
            return statement_type();
        }

        return statement_type(sth);
    }

    bool begin_transaction (pfs::error_code & ec, string_type & errstr)
    {
        return query("BEGIN TRANSACTION", ec, errstr);
    }

    bool commit (pfs::error_code & ec, string_type & errstr)
    {
        return query("COMMIT", ec, errstr);
    }

    bool rollback (pfs::error_code & ec, string_type & errstr)
    {
        return query("ROLLBACK", ec, errstr);
    }

    stringlist_type tables (string_type const & pattern
            , pfs::error_code & ec
            , string_type & errstr)
    {
        stringlist_type r;
        statement_type stmt = prepare(
                 "SELECT name FROM sqlite_master WHERE type='table' ORDER BY name"
                , ec
                , errstr);

        if (stmt) {
            result_type res = stmt.exec(ec, errstr);

            if (!ec) {
                if (pattern.empty()) {
                    while (res.has_more()) {
                        r.push_back(res.template get<string_type>(0));
                        ++res;
                    }
                } else {
                    basic_regex<string_type> rx(pattern);
                    typename basic_regex<string_type>::match_results m;

                    while (res.has_more()) {
                        string_type s = res.template get<string_type>(0);

                        if (pfs::regex_search(s, m, rx))
                            r.push_back(s);
                        ++res;
                    }
                }
            }
        }

        return r;
    }

    bool table_exists (string_type const & name, pfs::error_code & ec, string_type & errstr)
    {
        string_type sql("SELECT name FROM sqlite_master WHERE type='table' AND name='");
        sql += name;
        sql += "'";

        statement_type stmt = prepare(sql, ec, errstr);

        if (stmt) {
            result_type res = stmt.exec(ec, errstr);

            if (!ec) {
                if (res.has_more())
                    return true;
            }
        }

        return false;
    }
};

}}} // namespace pfs::sql::sqlite3
