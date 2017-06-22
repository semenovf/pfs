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

	if (!filesystem::exists(_path)) {
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
	if (! sym) {
        ec = pfs::make_error_code(dynamic_library_errc::symbol_not_found);
	}

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

filesystem::path build_so_filename (filesystem::path const & name) noexcept
{
	filesystem::path p("lib");
	p += name;
	p += ".so";
	return p;
}

} //pfs
