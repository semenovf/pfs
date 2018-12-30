#include "pfs/json/exception.hpp"

namespace pfs {

namespace details {

char const * json_category::name () const noexcept
{
    return "json_category";
}

std::string json_category::message (int ev) const
{
    switch (ev) {
    case static_cast<int>(json_errc::success):
        return "no error";

    case static_cast<int>(json_errc::bad_json):
        return "bad JSON";

    case static_cast<int>(json_errc::excess_source):
        return "excess source for JSON";

    case static_cast<int>(json_errc::bad_number):
        return "bad number";

    case static_cast<int>(json_errc::range):
        return "out of bounds";

    case static_cast<int>(json_errc::array_expected):
        return "array expected";

    case static_cast<int>(json_errc::object_expected):
        return "object expected";

    case static_cast<int>(json_errc::ubjson_parse):
        return "UBJSON parse error";

    default: return "unknown JSON error";
    }
}

} // details

pfs::error_category const & json_category ()
{
    static details::json_category instance;
    return instance;
}

} // pfs
