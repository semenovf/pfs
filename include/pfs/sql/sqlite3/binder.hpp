#pragma once
#include <string>
#include <pfs/sql/sqlite3/sqlite3.h>
#include <pfs/sql/sqlite3/private_data.hpp>

namespace pfs {
namespace sql {
namespace sqlite3 {

namespace details {

template <typename StringT>
inline bool binder_fail (stmt_native_handle_type sth
            , int rc
            , pfs::error_code & ec
            , StringT & errstr)
{
    db_native_handle_type dbh = sqlite3_db_handle(sth);
    ec = pfs::make_error_code(pfs::sql_errc::bind_fail);
    errstr = result_code<StringT>::errorstr(sqlite3_errmsg(dbh), rc);
    return false;
}

template <typename T, typename StringT = pfs::string>
struct binder
{
    bool operator () (stmt_native_handle_type sth
            , int index
            , int const & value
            , pfs::error_code & ec
            , StringT & errstr);
};

template <typename StringT>
struct binder<int, StringT>
{
    bool operator () (stmt_native_handle_type sth
            , int index
            , int const & value
            , pfs::error_code & ec
            , StringT & errstr)
    {
        int rc = sqlite3_bind_int(sth, index, value);
        return (rc != SQLITE_OK) ? binder_fail<StringT>(sth, rc, ec, errstr) : true;
    }
};

#if PFS_HAVE_INT64

template <typename StringT>
struct binder<sqlite3_int64, StringT>
{
    bool operator () (stmt_native_handle_type sth
            , int index
            , sqlite3_int64 const & value
            , pfs::error_code & ec
            , StringT & errstr)
    {
        int rc = sqlite3_bind_int64(sth, index, value);
        return (rc != SQLITE_OK) ? binder_fail<StringT>(sth, rc, ec, errstr) : true;
    }
};

template <typename StringT>
struct binder<long, StringT>
{
    bool operator () (stmt_native_handle_type sth
            , int index
            , sqlite3_int64 const & value
            , pfs::error_code & ec
            , StringT & errstr)
    {
        int rc = sqlite3_bind_int64(sth, index, value);
        return (rc != SQLITE_OK) ? binder_fail<StringT>(sth, rc, ec, errstr) : true;
    }
};
#else

template <typename StringT>
struct binder<long, StringT>
{
    bool operator () (stmt_native_handle_type sth
            , int index
            , long const & value
            , pfs::error_code & ec
            , StringT & errstr)
    {
        int rc = sqlite3_bind_int(sth, index, value);
        return (rc != SQLITE_OK) ? binder_fail<StringT>(sth, rc, ec, errstr) : true;
    }
};

#endif

template <typename StringT>
struct binder<StringT, StringT>
{
    bool operator () (stmt_native_handle_type sth
            , int index
            , StringT const & value
            , pfs::error_code & ec
            , StringT & errstr)
    {

        std::string utf8str = value.utf8();
        int rc = sqlite3_bind_text(sth, index, utf8str.c_str(), utf8str.size(), SQLITE_TRANSIENT);
        return (rc != SQLITE_OK) ? binder_fail<StringT>(sth, rc, ec, errstr) : true;
    }
};

template <typename StringT>
struct binder<std::string, StringT>
{
    bool operator () (stmt_native_handle_type sth
            , int index
            , std::string const & value
            , pfs::error_code & ec
            , StringT & errstr)
    {

        int rc = sqlite3_bind_text(sth, index, value.c_str(), value.size(), SQLITE_TRANSIENT);
        return (rc != SQLITE_OK) ? binder_fail<StringT>(sth, rc, ec, errstr) : true;
    }
};

}  //namespace details

}}} // pfs::sql::sqlite3

