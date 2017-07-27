/* 
 * File:   dynamic_library.hpp
 * Author: wladt
 *
 * Created on June 21, 2017, 1:48 PM
 */

#ifndef __PFS_DYNAMIC_LIBRARY_HPP__
#define __PFS_DYNAMIC_LIBRARY_HPP__

/* see http://en.wikipedia.org/wiki/Dynamic_loading */

#include <pfs/cxxlang.hpp>
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
    typedef filesystem::path::string_type system_string;
    
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
  	bool open (filesystem::path const & p
            , filesystem::pathlist const & searchdirs);

	bool open (filesystem::path const & p
            , filesystem::pathlist const & searchdirs
            , error_code & ec) pfs_noexcept;

	symbol_type resolve (char const * symbol_name);
    symbol_type resolve (char const * symbol_name, error_code & ec) pfs_noexcept;
};

/**
 * @brief Builds dynamic library (shared object) file name according to platform.
 *
 * @param name Base name of dynamic library.
 */
filesystem::path build_so_filename (filesystem::path const & name) pfs_noexcept;

#if __cplusplus >= 201103L
enum class dynamic_library_errc 
{
#else
struct dynamic_library_errc 
{
    enum {
#endif
          success = 0
        , invalid_argument
        , file_not_found
        , open_failed
        , symbol_not_found
#if __cplusplus < 201103L                  
    };
#endif    
};

namespace details {
class dynamic_library_category : public pfs::error_category 
{
public:
    virtual char const * name () const pfs_noexcept pfs_override;
    virtual std::string message (int ev) const pfs_override;
};
} // details

pfs::error_category const & dynamic_library_category ();

inline pfs::error_code make_error_code (dynamic_library_errc e)
{
    return pfs::error_code(static_cast<int>(e), dynamic_library_category());
}

} // pfs

namespace std {

template<>
struct is_error_code_enum<pfs::dynamic_library_errc>
        : public std::true_type 
{};

} // std


#endif /* __PFS_DYNAMIC_LIBRARY_HPP__ */

