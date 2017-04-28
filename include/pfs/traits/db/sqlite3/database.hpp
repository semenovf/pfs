/* 
 * File:   database.hpp
 * Author: wladt
 *
 * Created on Apr 27, 2017
 */

#ifndef __PFS_TRAITS_DB_SQLITE3_DATABASE_HPP__
#define __PFS_TRAITS_DB_SQLITE3_DATABASE_HPP__

#include <sqlite3.h>
#include <pfs/net/uri.hpp>
#include <pfs/traits/db/sqlite3/tag.hpp>
#include <pfs/traits/db/database.hpp>

namespace pfs {
namespace db {

template <typename StringT>
struct database_rep<StringT, sqlite3_tag>
{
    typedef traits::string<StringT>   string_type;
    typedef traits::c_str<StringT>    c_str;
    typedef db::exception<StringT>    exception;

    database_rep ()
        : _handle(0)
    {}
    
    bool open (string_type const & uri);
    void close ();
    
private:
    sqlite3 * _handle;
};

/**
 * @brief Connects to sqlite3 databases.
 *
 * @param driver_dsn Details see SQLite docs on sqlite3_open_v2 function.
 *				Examples:
 *              	file:data.db
 *                 		Open the file "data.db" in the current directory.
 *
 *                  sqlite3:/home/fred/data.db
 *                  sqlite3:///home/fred/data.db
 *                  sqlite3://localhost/home/fred/data.db
 *                  	Open the database file "/home/fred/data.db".
 *
 *                  sqlite3:///C:/Documents%20and%20Settings/fred/Desktop/data.db
 *                  	Windows only: Open the file "data.db" on fred's desktop on drive C:.
 *                  	Note that the %20 escaping in this example is not
 *                  	strictly necessary - space characters can be used literally
 *                  	in URI filenames.
 *
 *                  sqlite3:data.db?mode=ro&cache=private
 *                  	Open file "data.db" in the current directory for read-only access.
 *                  	Regardless of whether or not shared-cache mode is enabled
 *                  	by default, use a private cache.
 *
 *                  sqlite3:/home/fred/data.db?vfs=unix-nolock
 *                  	Open file "/home/fred/data.db". Use the special VFS "unix-nolock".
 *
 *                  sqlite3:data.db?mode=readonly
 *                  	An error. "readonly" is not a valid option for the "mode" parameter.
 *
 *				Mode values:
 *					mode=ro (default)
 *				    mode=rw
 *                  mode=rwc
 *                  mode=memory
 *
 *              Cache values:
 *              	cache=shared
 *              	cache=private
 *
 * @return DBI connection to specified sqlite3 databases.
 *
 * @note  Autocommit mode is on by default.
 */
template <typename StringT>
bool
database_traits<StringT, sqlite3_tag>::open (string_type const & uristr)
{
    pfs::net::uri<StringT> uri
    
    if (! uri.starts_with(string_type("sqlite3:"))) {
        throw exception_type(string_type("bad database URI"));
    }

	int rc        = SQLITE_OK;
	int flags     = SQLITE_OPEN_URI;
	int flag_mode = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE;

	pfs::map<pfs::string, pfs::string>::const_iterator mode = params.find(string_type("mode"));

	if (mode != params.cend()) {
		if (mode->second == "ro")
			flag_mode = SQLITE_OPEN_READONLY;
		else if (mode->second == "rw")
			flag_mode = SQLITE_OPEN_READWRITE;
		else if (mode->second == "rwc")
			flag_mode = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE;
		else if (mode->second == "memory")
			flag_mode = SQLITE_OPEN_MEMORY;
	}

	flags |= flag_mode;

    rc = sqlite3_open_v2(path.c_str(), & dbh_native, s3_flags, NULL);

	if (rc != SQLITE_OK) {
		if (!dbh_native) {
			errstr = _u8(_Tr("Unable to allocate memory for database handler."));
			errstr << _Tr("Internal error code: ") << pfs::string::number(rc);
		} else {
			switch( rc ) {
				case SQLITE_CANTOPEN:
					errstr << _Tr("Unable to open the database file. Try to check path ") << path;
					break;
				default: break;
			}
			sqlite3_close(dbh_native);
			dbh_native = nullptr;
		}
	} else {
		PFS_ASSERT(__dbd);
		dbh = new Sqlite3DbHandler;

		// TODO what for this call ?
		sqlite3_busy_timeout(dbh_native, MaxSqlTimeout);

		// Enable extended result codes
		sqlite3_extended_result_codes(dbh_native, 1);

		dbh->_driver = __dbd;
		dbh->_dbh_native = dbh_native;
	}

	if (dbh) {
		pfs::auto_lock<> lock(& __mutex);
		++__refs;
	}

    return true;
}

//template <typename StringT>
//void database_traits<StringT, sqlite3_rep>::xclose (data_type & d)
//{
//    
//}

}} // pfs::db

#endif /* __PFS_TRAITS_DB_SQLITE3_DATABASE_HPP__ */

