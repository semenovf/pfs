#include "pfs/json/exception.hpp"

namespace pfs {

namespace details {

char const * json_category::name () const pfs_noexcept
{
    return "json_category";
}

std::string json_category::message (int ev) const
{
    switch (ev) {
    case static_cast<int>(json_errc::success):
        return "No error";
        
    case static_cast<int>(json_errc::bad_number):
        return "Bad number";

    default: return "Unknown JSON error";
    }
}

} // details

pfs::error_category const & json_category ()
{
    static details::json_category instance;
    return instance;
}

} // pfs