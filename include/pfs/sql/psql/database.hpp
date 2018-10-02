#pragma once
#include <pfs/atomic.hpp>
#include <pfs/string.hpp>
#include <pfs/stringlist.hpp>
#include <pfs/system_error.hpp>
#include <pfs/net/uri.hpp>
#include <pfs/sql/exception.hpp>
#include <pfs/sql/psql/private_data.hpp>
#include <pfs/sql/psql/statement.hpp>
#include <pfs/sql/psql/result_code.hpp>
#include <pfs/sql/psql/result.hpp>

namespace pfs {
namespace sql {
namespace psql {

//
// Useful functions:
//
// 1. database name for live connection
//      char * PQdb (PGconn const * conn);
//
template <typename StringListT = pfs::stringlist<>, typename StringT = pfs::string>
class database
{
public:
    typedef StringT               string_type;
    typedef StringListT           stringlist_type;
    typedef statement<StringT>    statement_type;
    typedef result<StringT>       result_type;
    typedef db_native_handle_type native_handle_type;

private:
    db_handle_shared _pd;

private:
    static string_type make_unique_statement_name ()
    {
        static pfs::atomic_int32_t psql_statement_count(0);
        string_type r("pfs_psql_stmt_");
        r += pfs::to_string(++psql_statement_count, 16, true);
        return r;
    }

public:
    database () {}

    native_handle_type native_handle () const
    {
        return _pd.get();
    }

    /**
    * @brief Connects to postgresql database.
    *
    * @param db_uri Connection URI.
    *       The general form for a connection URI is:
    *       @code
    *           postgresql://[user[:password]@][netloc][:port][/dbname][?param1=value1&...]
    *       @endcode
    *       The URI scheme designator can be either @c postgresql:// or @c postgres://
    *       Examples:
    *       @code
    *           postgresql://
    *           postgresql://localhost
    *           postgresql://localhost:5433
    *           postgresql://localhost/mydb
    *           postgresql://user@localhost
    *           postgresql://user:secret@localhost
    *           postgresql://other@localhost/otherdb?connect_timeout=10&application_name=myapp
    *       @endcode
    *       Components of the hierarchical part of the URI can also be given as parameters. For example:
    *       @code
    *           postgresql:///mydb?host=localhost&port=5433
    *       @endcode
    *       Details see PostgresSQL documentation (section 'Connection Strings',
    *       section number 31.1.1 for PostgresSQL 9.3.9).
    *
    * @return DBI connection to specified sqlite3 databases.
    *
    * @note  Autocommit mode is on by default.
    */
    bool open (string_type const & db_uri, error_code & ec, string_type & errstr)
    {
        pfs::net::uri<string_type> uri;

        if (! (db_uri.starts_with("postgresql:")
                    || db_uri.starts_with("postgres:"))) {
            ec = make_error_code(sql_errc::bad_uri);
            return false;
        }

        if (!uri.parse(db_uri)) {
            ec = make_error_code(sql_errc::bad_uri);
            return false;
        }

        //
        // This function will always return a non-null object pointer,
        // unless perhaps there is too little memory even
        // to allocate the PGconn object. The PQstatus function should be
        // called to check the return value for a successful connection before
        // queries are sent via the connection object.
        //
        native_handle_type dbh = PQconnectdb(db_uri.c_str());

        if (!dbh) {
            ec = make_error_code(sql_errc::bad_alloc);
            return false;
        } else if (PQstatus(dbh) != CONNECTION_OK) {
            ec = make_error_code(sql_errc::open_fail);
            errstr = result_code<string_type>::to_string(dbh);

            PQfinish(dbh);
            dbh = 0;
            return false;
        }

        // CONNECTION_OK

        db_handle_shared pd(dbh, db_handle_deleter());
        _pd.swap(pd);
        return true;
    }

    bool opened () const
    {
        return _pd.get() != 0;
    }

    void close ()
    {
        db_handle_shared pd;
        _pd.swap(pd);
    }

    bool clear (pfs::error_code & ec, string_type & errstr)
    {
        stringlist_type tlist = this->tables(ec, errstr);

        if (ec)
            return false;

        typename stringlist_type::const_iterator first = tlist.cbegin();
        typename stringlist_type::const_iterator last = tlist.cend();

        for (; first != last; ++first) {
            string_type sql("DROP TABLE IF EXISTS \"");
            sql += *first;
            sql += "\" CASCADE";
            result_type res = this->exec(sql, ec, errstr);

            if (!res) return false;
        }

        return true;
    }

    result_type exec (string_type const & sql, pfs::error_code & ec, string_type & errstr)
    {
        PGresult * res = PQexec(_pd.get(), sql.utf8().c_str());

        ExecStatusType status = PQresultStatus(res);
        bool r = (status == PGRES_COMMAND_OK || status == PGRES_TUPLES_OK);

        if (!r) {
            errstr = result_code<string_type>::to_string(res);
            ec = make_error_code(sql_errc::query_fail);
            if (res)
                PQclear(res);
            return result_type();
        }

        return result_type(res);
    }

    //
    // Used by debby::database to create statement
    //
    statement_type prepare (string_type const & sql
            , pfs::error_code & ec
            , string_type & errstr)
    {
        string_type stmt_name = make_unique_statement_name();
        std::string utf8_query(sql.utf8());

        stmt_native_handle_type sth = PQprepare(_pd.get()
                , stmt_name.utf8().c_str()
                , utf8_query.c_str()
                , 0
                , 0);

        ExecStatusType status = PQresultStatus(sth);

        if (!(status == PGRES_COMMAND_OK || status == PGRES_TUPLES_OK)) {
            ec = pfs::make_error_code(pfs::sql_errc::query_fail);
            errstr = result_code<string_type>::to_string(sth);

            if (sth)
                PQclear(sth);

            return statement_type();
        }

        return statement_type(sth, _pd, stmt_name);
    }

    bool begin_transaction (pfs::error_code & ec, string_type & errstr)
    {
        return exec("BEGIN TRANSACTION", ec, errstr).done();
    }

    bool commit (pfs::error_code & ec, string_type & errstr)
    {
        return exec("COMMIT TRANSACTION", ec, errstr).done();
    }

    bool rollback (pfs::error_code & ec, string_type & errstr)
    {
        return exec("ROLLBACK TRANSACTION", ec, errstr).done();
    }

    stringlist_type tables (pfs::error_code & ec, string_type & errstr)
    {
        stringlist_type r;

        // SQL statement borrowed from Qt5 project
        statement_type stmt = prepare(
                  "select pg_class.relname, pg_namespace.nspname from pg_class"
                  " left join pg_namespace on (pg_class.relnamespace = pg_namespace.oid)"
                  " where (pg_class.relkind = 'r') and (pg_class.relname !~ '^Inv')"
                  " and (pg_class.relname !~ '^pg_')"
                  " and (pg_namespace.nspname != 'information_schema');"
                , ec
                , errstr);

        if (stmt) {
            result_type res = stmt.exec(ec, errstr);

            if (!ec) {
                while (res.has_more()) {
                    r.push_back(res.template get<string_type>(0));
                    ++res;
                }
            }
        }

        return r;
    }

    bool table_exists (string_type const & name, pfs::error_code & ec, string_type & errstr)
    {
        string_type sql("SELECT relname FROM pg_class WHERE relname='");
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

}}} // namespace pfs::sql::psql
