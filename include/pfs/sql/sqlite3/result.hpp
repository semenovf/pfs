#pragma once
#include <pfs/sql/sqlite3/sqlite3.h>
#include <pfs/string.hpp>
#include <pfs/system_error.hpp>
#include <pfs/sql/sqlite3/id.hpp>
#include <pfs/sql/sqlite3/private_data.hpp>
#include <pfs/sql/cast.hpp>

namespace pfs {
namespace sql {
namespace sqlite3 {

template <typename StringT = pfs::string>
class result
{
    typedef StringT string_type;

    stmt_handle_shared _pd;
    int _rc;

private:
    template <typename T>
    struct cast_traits : public details::cast_traits<T, string_type>
    {};

public:
    result () : _rc(SQLITE_DONE) {}
    result (stmt_handle_shared const & stdata, int rc)
        : _pd(stdata)
        , _rc(rc)
    {}

    ~result () {}

    operator bool () const
    {
        return _pd.get() != 0;
    }

    bool done () const
    {
        return _rc == SQLITE_DONE;
    }

    bool fail () const
    {
        return _pd.get() == 0 || !(_rc == SQLITE_ROW || _rc == SQLITE_DONE || _rc == SQLITE_OK);
    }

    bool has_more () const
    {
        return _rc == SQLITE_ROW;
    }

    result & operator ++ ()
    {
        _rc = sqlite3_step(_pd.get());
        return *this;
    }

    result operator ++ (int) const
    {
        result r(*this);
        ++r;
        return r;
    }

    int column_count () const
    {
        return sqlite3_column_count(_pd.get());
    }

    template <typename T>
    T get (int column) const
    {
        switch (sqlite3_column_type(_pd.get(), column)) {
        case SQLITE_TEXT:
            return cast_traits<T>::cast(string_type(reinterpret_cast<char const *>(sqlite3_column_text(_pd.get(), column))));
        case SQLITE_INTEGER:
#if PFS_HAVE_INT64
            return cast_traits<T>::cast(static_cast<intmax_t>(sqlite3_column_int64(_pd.get(), column)));
#else
            return cast_traits<T>::cast(static_cast<intmax_t>(sqlite3_column_int(_pd.get(), column)));
#endif
        case SQLITE_FLOAT:
            return cast_traits<T>::cast(sqlite3_column_double(_pd.get(), column));

        case SQLITE_BLOB:
            // TODO Implement
//             const char * bytes = reinterpret_cast<const char*>(sqlite3_column_blob(s3_sth->_native_handler, i));
//             int nbytes = sqlite3_column_bytes(s3_sth->_native_handler, i);
//             PFS_ASSERT(nbytes >= 0);
//             row.append(pfs::unitype(pfs::bytearray(bytes, size_t(nbytes))));
            break;

        case SQLITE_NULL:
            return T();

        default:
            PFS_ASSERT_X(false, "Bad column type");
            break;
        }

        return T();
    }

    string_type column_name (int column)
    {
        string_type name(sqlite3_column_name(_pd.get(), column));
        return name;
    }
};

}}} // pfs::sql::sqlite3
