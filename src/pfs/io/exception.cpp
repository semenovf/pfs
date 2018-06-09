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
        return "no error";

    case static_cast<int>(io_errc::operation_in_progress):
        return "operation in progress";

    case static_cast<int>(io_errc::connection_refused):
        return "connection refused";

    case static_cast<int>(io_errc::bad_file_descriptor):
        return "bad file descriptor";

    case static_cast<int>(io_errc::stream):
        return "stream error";

    case static_cast<int>(io_errc::timeout):
        return "timed out";

    default: return "unknown I/O error";
    }
}

} // details

pfs::error_category const & io_category ()
{
    static details::io_category instance;
    return instance;
}

} // pfs
