#include "pfs/rpc.hpp"

namespace pfs {

namespace details {

char const * rpc_error_category::name () const pfs_noexcept
{
    return "rpc_error_category";
}

std::string rpc_error_category::message (int ev) const
{
    switch (ev) {
    case static_cast<int>(rpc_errc::success):
        return "no error";

    case static_cast<int>(rpc_errc::version_not_match):
        return "version does not match";

    case static_cast<int>(rpc_errc::id_not_match):
        return "ID does not match";

    case static_cast<int>(rpc_errc::bad_response):
        return "bad response";

    case static_cast<int>(rpc_errc::bad_request):
        return "bad request";

    default: return "unknown RPC error";
    }
}

} // details

pfs::error_category const & rpc_error_category ()
{
    static details::rpc_error_category instance;
    return instance;
}

} // pfs
