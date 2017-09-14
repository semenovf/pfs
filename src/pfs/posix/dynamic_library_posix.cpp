#include <sys/stat.h>
#include "pfs/dynamic_library.hpp"
#include "pfs/system_error.hpp"

namespace pfs {

dynamic_library::~dynamic_library ()
{
    if (_handle) {
        ::dlerror(); /*clear error*/
        ::dlclose(_handle);
        _handle = dynamic_library::native_handle_type(0);
    }
}

bool dynamic_library::open (filesystem::path const & p, error_code & ec)
{
    dynamic_library::native_handle_type h(0);

    if (p.empty()) {
        ec = pfs::make_error_code(dynamic_library_errc::invalid_argument);
        return false;
    }

    if (!filesystem::exists(p)) {
        ec = pfs::make_error_code(dynamic_library_errc::file_not_found);
        return false;
    }

    // clear error
    ::dlerror();

    bool global = false;
    bool resolve = true;

    h = ::dlopen(p.native().c_str()
                 , (global ? RTLD_GLOBAL : RTLD_LOCAL)
                   | ( resolve ? RTLD_NOW : RTLD_LAZY));

    if (!h) {
        ec = pfs::make_error_code(dynamic_library_errc::open_failed);
        return false;
    }

    _handle = h;
    _path = p;

    return true;
}

bool dynamic_library::open (filesystem::path const & p
        , filesystem::pathlist const & searchdirs
        , error_code & ec) pfs_noexcept
{
    dynamic_library::native_handle_type h(0);

    _path = p;

    if (_path.empty()) {
        ec = pfs::make_error_code(dynamic_library_errc::invalid_argument);
        return false;
    }

    if (!filesystem::exists(_path, ec)) {
        _path = filesystem::search_path(_path, searchdirs);

    if (_path.empty()) {
            ec = pfs::make_error_code(dynamic_library_errc::file_not_found);
            return false;
        }
    }

    // clear error
    ::dlerror();

    bool global = false;
    bool resolve = true;

    h = ::dlopen(_path.native().c_str()
                 , (global ? RTLD_GLOBAL : RTLD_LOCAL)
                   | ( resolve ? RTLD_NOW : RTLD_LAZY));

    if (!h) {
        _path.clear();
        ec = pfs::make_error_code(dynamic_library_errc::open_failed);
        return false;
    }

    _handle = h;

    return true;
}

bool dynamic_library::open (filesystem::path const & p
        , filesystem::pathlist const & searchdirs)
{
    error_code ec;

    if (! open(p, searchdirs, ec))
        throw filesystem::filesystem_error(std::string(::dlerror())
                , _path, ec);

    return true;
}


dynamic_library::symbol_type dynamic_library::resolve (char const * symbol_name
        , error_code & ec) pfs_noexcept
{
    // clear error
    ::dlerror();

    dynamic_library::symbol_type sym = ::dlsym(_handle, symbol_name);

    // Failed to resolve symbol
    if (! sym)
        ec = pfs::make_error_code(dynamic_library_errc::symbol_not_found);

    return sym;
}

dynamic_library::symbol_type dynamic_library::resolve (char const * symbol_name)
{
    error_code ec;
    symbol_type sym = resolve(symbol_name, ec);

    if (! sym)
        throw filesystem::filesystem_error(std::string(::dlerror())
                , _path, ec);

    return sym;
}

filesystem::path build_so_filename (filesystem::path const & name) pfs_noexcept
{
    filesystem::path p("lib");
    p += name;
    p += ".so";
    return p;
}

namespace details {

std::string dynamic_library_category::message (int ev) const
{
    switch (ev) {
    case static_cast<int>(dynamic_library_errc::success):
        return "no error";

    case static_cast<int>(dynamic_library_errc::invalid_argument):
        return "invalid argument";

    case static_cast<int>(dynamic_library_errc::file_not_found):
        return "shared object (dynamic library) not found";

    case static_cast<int>(dynamic_library_errc::open_failed):
        return std::string("failed to open shared object (dynamic library): ")
                + dlerror();

    case static_cast<int>(dynamic_library_errc::symbol_not_found):
        return std::string("symbol not found in shared object (dynamic library): ")
                + dlerror();

    default: return "unknown dynamic_library error";
    }
}

} // details

} //pfs
