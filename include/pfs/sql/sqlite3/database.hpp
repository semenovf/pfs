#pragma once
#include <pfs/sql/sqlite3/sqlite3.h>
#include <pfs/noncopyable.hpp>
#include <pfs/string.hpp>
#include <pfs/stringlist.hpp>
#include <pfs/system_error.hpp>
#include <pfs/net/uri.hpp>
#include <pfs/sql/exception.hpp>
#include <pfs/sql/sqlite3/statement.hpp>

namespace pfs {
namespace sql {
namespace sqlite3 {

template <typename StringListT = pfs::stringlist<>, typename StringT = pfs::string>
class database : noncopyable
{
    static int const MAX_BUSY_TIMEOUT = 1000; // 1 second

public:
    typedef StringT            string_type;
    typedef StringListT        stringlist_type;
    typedef statement<StringT> statement_type;

private:
    native_handle_type _h;

public:
    database () : _h(0) {}

    native_handle_type native_handle () const
    {
        return _h;
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

        int rc = sqlite3_open_v2(filename.to_string().utf8().c_str(), & _h, flags, NULL);

        if (rc != SQLITE_OK) {
            if (!_h) {
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

                sqlite3_close_v2(_h);
                _h = 0;
            }

            return false;
        } else {
            // TODO what for this call ?
            sqlite3_busy_timeout(_h, MAX_BUSY_TIMEOUT);

            // Enable extended result codes
            sqlite3_extended_result_codes(_h, 1);
        }

        return true;
    }

    bool close ()
    {
        int rc = sqlite3_close_v2(_h);
        _h = 0;
        return rc == SQLITE_OK;
    }

    bool opened () const
    {
        return _h != 0;
    }

    bool query (string_type const & sql, pfs::error_code & ec, string_type & errstr)
    {
        char * errmsg;
        int rc = sqlite3_exec(_h, sql.utf8().c_str(), NULL, NULL, & errmsg);

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

    bool begin ()
    {
        pfs::error_code ec;
        return query("BEGIN", ec, 0);
    }

    bool commit ()
    {
        pfs::error_code ec;
        return query("COMMIT", ec, 0);
    }

    bool rollback ()
    {
        pfs::error_code ec;
        return query("ROLLBACK", ec, 0);
    }

    stringlist_type tables (pfs::error_code & rc, string_type & errstr) const
    {
        stringlist_type result;

        statement_type stmt;

        if (stmt.prepare(_h
                , "SELECT anme FROM sqlite_master WHERE type='table' ORDER BY name"
                , ec
                , errstr)) {

            if (stmt.exec(ec, errstr)) {

            }
        }

        return result;
    }
};

}}} // namespace pfs::sql::sqlite3
