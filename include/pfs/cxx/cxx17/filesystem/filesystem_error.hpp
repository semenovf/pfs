#pragma once
#include <pfs/config.h>

#if HAVE_STDCXX_FILESYSTEM

#include <filesystem>

namespace pfs {
namespace filesystem {
namespace details {

    using filesystem_error = std::filesystem::filesystem_error;

/**
 * @fn filesystem_error::filesystem_error (std::string const & what_arg, pfs::error_code ec)
 */

/**
 * @fn filesystem_error::filesystem_error (std::string const & what_arg
 *          , pfs::filesystem::path const & p1
 *          , pfs::error_code ec)
 */

/**
 * @fn filesystem_error::filesystem_error (std::string const & what_arg
 *          , pfs::filesystem::path const & p1
 *          , pfs::filesystem::path const & p2
 *          , pfs::error_code ec)
 */

}}} // pfs::filesystem

#else

#include <pfs/cxx/cxx14/filesystem/filesystem_error.hpp>

#endif
