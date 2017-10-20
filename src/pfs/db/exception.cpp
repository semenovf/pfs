#include "pfs/db/exception.hpp"

namespace pfs {

namespace details {

char const * db_category::name () const pfs_noexcept
{
    return "db_category";
}

std::string db_category::message (int ev) const
{
    switch (ev) {
    case static_cast<int>(db_errc::success):
        return "no error";
    case static_cast<int>(db_errc::bad_uri):
        return "bad URI";
    case static_cast<int>(db_errc::open_fail):
        return "database open failed";
    case static_cast<int>(db_errc::bad_alloc):
        return "insufficient memory";
    case static_cast<int>(db_errc::query_fail):
        return "query failed";
    default: break;
    }
    return "unknown DB error";
}

} // details

pfs::error_category const & db_category ()
{
    static details::db_category instance;
    return instance;
}

} // pfs