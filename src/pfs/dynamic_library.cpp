#include "pfs/dynamic_library.hpp"

namespace pfs {

namespace details {
char const * dynamic_library_category::name () const pfs_noexcept
{
    return "dynamic_library_category";
}

std::string dynamic_library_category::message (int ev) const
{
    switch (ev) {
    case static_cast<int>(dynamic_library_errc::success):
        return "No error";
        
    case static_cast<int>(dynamic_library_errc::invalid_argument):
        return "Invalid argument";
        
    case static_cast<int>(dynamic_library_errc::file_not_found):
        return "Shared object (dynamic library) not found";
    
    case static_cast<int>(dynamic_library_errc::open_failed):
        return "Failed to open shared object (dynamic library)";
        
    case static_cast<int>(dynamic_library_errc::symbol_not_found):
        return "Symbol not found in shared object (dynamic library)";
        
    default: return "Unknown dynamic_library error";
    }
}
}

pfs::error_category const & dynamic_library_category ()
{
    static details::dynamic_library_category instance;
    return instance;
}

} // pfs