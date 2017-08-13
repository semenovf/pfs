#include "pfs/lexical_cast.hpp"

namespace pfs {

namespace details {

char const * lexical_cast_category::name () const pfs_noexcept
{
    return "lexical_cast_category";
}

std::string lexical_cast_category::message (int ev) const
{
    switch (ev) {
    case static_cast<int>(lexical_cast_errc::success):
        return "No error";

    case static_cast<int>(lexical_cast_errc::bad_radix):
        return "Bad radix";

    case static_cast<int>(lexical_cast_errc::underflow):
        return "Underflow";

    case static_cast<int>(lexical_cast_errc::overflow):
        return "Overflow";

    case static_cast<int>(lexical_cast_errc::invalid_string):
        return "Invalid representation of number by string";

    default: return "Unknown lexical_cast error";
    }
}

} // details

pfs::error_category const & lexical_cast_category ()
{
    static details::lexical_cast_category instance;
    return instance;
}

} // pfs