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

error_code dynamic_library::open (filesystem::path const & p
        , filesystem::pathlist const & searchdirs)
{
	dynamic_library::native_handle_type h(0);

	_path = p;

	if (_path.empty()) {
        return pfs::make_error_code(errc::invalid_argument);
	}

	if (!filesystem::exists(_path)) {
		_path = filesystem::search_path(_path, searchdirs);

		if (_path.empty()) {
            return pfs::make_error_code(errc::no_such_file_or_directory);
		}
	}

	// clear error
	//
	::dlerror();

	bool global = false;
	bool resolve = true;

	h = ::dlopen(_path.native().c_str()
			, (global ? RTLD_GLOBAL : RTLD_LOCAL)
				| ( resolve ? RTLD_NOW : RTLD_LAZY));

	if (!h) {
        _path.clear();
        return pfs::make_error_code(errc::invalid_argument);
	}

	_handle = h;

	return error_code();
}

dynamic_library::symbol_type dynamic_library::resolve (char const * symbol_name)
{
	// clear error
	//
	::dlerror();

	dynamic_library::symbol_type r = ::dlsym(_handle, symbol_name);
	char * errstr = ::dlerror();

    // Failed to resolve symbol
	if (errstr) {
        throw filesystem::filesystem_error(std::string(errstr)
                , _path
                , pfs::make_error_code(errc::invalid_argument));
	}

	return r;
}

filesystem::path build_so_filename (filesystem::path const & name) noexcept
{
	filesystem::path p("lib");
	p += name;
	p += ".so";
	return p;
}

} //pfs
