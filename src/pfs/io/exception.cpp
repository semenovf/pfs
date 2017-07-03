#include "pfs/io/exception.hpp"

namespace pfs {

namespace details {

char const * io_category::name () const pfs_noexcept
{
    return "io_category";
}

std::string io_category::message (int ev) const
{
    switch (ev) {
    case static_cast<int>(io_errc::success):
        return "No error";
        
    default: return "Unknown I/O error";
    }
}

} // details

pfs::error_category const & io_category ()
{
    static details::io_category instance;
    return instance;
}

} // pfs