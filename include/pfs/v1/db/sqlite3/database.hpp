#ifndef __PFS_DB_SQLITE3_DATABASE_HPP__
#define __PFS_DB_SQLITE3_DATABASE_HPP__

#include <pfs/system_error.hpp>
#include <pfs/net/uri.hpp>
#include <pfs/db/exception.hpp>
#include <pfs/db/sqlite3/sqlite3.h>

namespace pfs {
namespace db {
namespace sqlite3 {

template <typename StringType>
struct database
{
    static int const MAX_BUSY_TIMEOUT = 1000; // 1 second

    typedef StringType string_type;
    typedef struct sqlite3 *  native_handle;

private:
    native_handle _h;

public:
    database () : _h(0) {}

    bool open (string_type const & uri, error_code & ec, string_type * errstr);
    bool close ();

    bool opened () const
    {
        return _h != 0;
    }

    bool query (char const * sql, pfs::error_code & ec, string_type * errstr);

    bool query (string_type const & sql, pfs::error_code & ec, string_type * errstr)
    {
        return query(u8string<std::string>(sql).c_str(), ec, errstr);
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
};

/**
 * @brief Connects to sqlite3 databases.
 *
 * @param driver_dsn Details see SQLite docs on sqlite3_open_v2 function.
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
template <typename StringType>
bool database<StringType>::open (string_type const & uristr, error_code & ec, string_type * errstr)
{
    pfs::net::uri<StringType> uri;

    if (! uristr.starts_with(string_type("sqlite3:"))) {
        ec = make_error_code(db_errc::bad_uri);
        return false;
    }

    if (!uri.parse(uristr)) {
        ec = make_error_code(db_errc::bad_uri);
        return false;
    }

    int flags = SQLITE_OPEN_URI;

    //
    // It is an error to specify a value for the mode parameter 
    // that is less restrictive than that specified by the flags passed 
    // in the third parameter to sqlite3_open_v2().
    //
    flags |= SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE;

    pfs::net::uri<StringType> filename;
    filename.set_scheme("file");
    filename.set_path(uri.path());
    filename.set_query(uri.query());

    int rc = sqlite3_open_v2(u8string<std::string>(filename.to_string()).c_str(), & _h, flags, NULL);

    if (rc != SQLITE_OK) {
        if (!_h) {
            // Unable to allocate memory for database handler.
            // Internal error code.
            ec = make_error_code(db_errc::bad_alloc);
        } else {
            switch (rc) {
            case SQLITE_CANTOPEN:
                ec = make_error_code(db_errc::open_fail);
                if (errstr) {
                    *errstr = sqlite3_errstr(rc);
                }
                return false;
            default:
                ec = make_error_code(db_errc::specific_error);
                if (errstr) {
                    *errstr = sqlite3_errstr(rc);
                }
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

template <typename StringType>
inline bool database<StringType>::close ()
{
    int rc = sqlite3_close_v2(_h);
    _h = 0;
    return rc == SQLITE_OK;
}

template <typename StringType>
bool database<StringType>::query (char const * sql, pfs::error_code & ec, string_type * errstr)
{
    char * errmsg;
    int rc = sqlite3_exec(_h, sql, NULL, NULL, & errmsg);

    if (SQLITE_OK != rc) {
        if (errmsg && errstr) {
            *errstr = errmsg;
            sqlite3_free(errmsg);
        }
        return false;
    }

    return true;
}

}}} // pfs::db::sqlite3

#endif /* __PFS_DB_SQLITE3_DATABASE_HPP__ */
