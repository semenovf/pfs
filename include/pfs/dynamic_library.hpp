/* 
 * File:   dynamic_library.hpp
 * Author: wladt
 *
 * Created on June 21, 2017, 1:48 PM
 */

#ifndef __PFS_DYNAMIC_LIBRARY_HPP__
#define __PFS_DYNAMIC_LIBRARY_HPP__

/* see http://en.wikipedia.org/wiki/Dynamic_loading */
//#include <pfs/string.hpp>
//#include <pfs/shared_ptr.hpp>
//#include <pfs/map.hpp>
//#include <pfs/vector.hpp>
//#include <pfs/stringlist.hpp>
//#include <pfs/pluggable.hpp>
//#include <pfs/error_code.hpp>
#include <pfs/compiler.hpp>
#include <pfs/memory.hpp>
#include <pfs/filesystem.hpp>
#include <pfs/system_error.hpp>

#ifdef PFS_CC_MSVC
#	include <windows.h>
#	include <windef.h>
#else
#	include <dlfcn.h>
#endif

namespace pfs {

class dynamic_library
{
public:
    typedef filesystem::path::string_type string_type;
    
#ifdef PFS_CC_MSC
	typedef HMODULE native_handle_type;
	typedef FARPROC symbol_type;
#else
	typedef void * native_handle_type;
	typedef void * symbol_type;
#endif

private:
    native_handle_type _handle;
    filesystem::path   _path;   // contains path to dynamic library

private:
#if __cplusplus >= 201103L    
    dynamic_library (dynamic_library const &) = delete;
    dynamic_library & operator = (dynamic_library const &) = delete;
#else
    dynamic_library (dynamic_library const &);
    dynamic_library & operator = (dynamic_library const &);
#endif
    
public:
	dynamic_library ()
		: _handle(0)
	{};

	~dynamic_library ();

	native_handle_type native_handle () const
	{
		return _handle;
	}

	/**
	 * @brief Open (load) dynamic library (shared object).
	 *
	 * @param p path to dynamic library file (relative or absolute).
	 *
	 * @return @c invalid_argument if path @a p is empty,
	 */
	error_code open (filesystem::path const & p, filesystem::pathlist const & searchdirs);

	symbol_type resolve (char const * symbol_name);
};

/**
 * @brief Builds dynamic library (shared object) file name according to platform.
 *
 * @param name Base name of dynamic library.
 */
filesystem::path build_so_filename (filesystem::path const & name) noexcept;

} // pfs

#endif /* __PFS_DYNAMIC_LIBRARY_HPP__ */

