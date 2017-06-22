/* 
 * File:   filesystem_error.hpp
 * Author: wladt
 *
 * Created on June 21, 2017, 11:57 AM
 */

#ifndef __PFS_CXX_CXX11_FILESYSTEM_FILESYSTEM_ERROR_HPP__
#define __PFS_CXX_CXX11_FILESYSTEM_FILESYSTEM_ERROR_HPP__

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

#endif /* __PFS_CXX_CXX11_FILESYSTEM_FILESYSTEM_ERROR_HPP__ */
