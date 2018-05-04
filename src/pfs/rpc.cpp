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
    case static_cast<int>(rpc_errc::no_error):
        return "no error";

    case static_cast<int>(rpc_errc::bad_version):
        return "bad version";

    case static_cast<int>(rpc_errc::id_not_match):
        return "ID does not match";

    case static_cast<int>(rpc_errc::parse_error):
        return "parse error";

    case static_cast<int>(rpc_errc::invalid_request):
        return "invalid request";

    case static_cast<int>(rpc_errc::method_not_found):
        return "method not found";

    case static_cast<int>(rpc_errc::invalid_params):
        return "invalid method parameters";

    case static_cast<int>(rpc_errc::invalid_response):
        return "invalid response";

    case static_cast<int>(rpc_errc::internal_error):
        return "internal error";

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
