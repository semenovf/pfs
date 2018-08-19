#include "pfs/sql/exception.hpp"

namespace pfs {

namespace details {

char const * sql_category::name () const pfs_noexcept
{
    return "sql_category";
}

std::string sql_category::message (int ev) const
{
    switch (ev) {
    case static_cast<int>(sql_errc::success):
        return "no error";
    case static_cast<int>(sql_errc::bad_uri):
        return "bad URI";
    case static_cast<int>(sql_errc::open_fail):
        return "database open failed";
    case static_cast<int>(sql_errc::bad_alloc):
        return "insufficient memory";
    case static_cast<int>(sql_errc::query_fail):
        return "query failed";
    case static_cast<int>(sql_errc::bind_fail):
        return "parameter binding failed";
    case static_cast<int>(sql_errc::specific_error):
        return "specific error";
    case static_cast<int>(sql_errc::bad_handle):
        return "handle is not valid";
    default: break;
    }
    return "unknown DB error";
}

} // details

pfs::error_category const & sql_category ()
{
    static details::sql_category instance;
    return instance;
}

} // pfs
