#pragma once
#include <pfs/compiler.hpp>

#if 0 // PFS_CC_GCC_VERSION >= 50300 && HAVE_STDCXX_FS_EXPERIMENTAL

#include <experimental/filesystem>

namespace pfs {
namespace filesystem {

using filesystem_error = std::experimental::filesystem::filesystem_error;

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

}} // pfs::filesystem

#else
#   include <pfs/cxx/cxx98/filesystem/filesystem_error.hpp>
#endif
