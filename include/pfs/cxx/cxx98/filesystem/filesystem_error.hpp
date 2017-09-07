/* 
 * File:   filesystem_error.hpp
 * Author: wladt
 *
 * Created on Sep 07, 2017
 */

#ifndef __PFS_CXX_CXX98_FILESYSTEM_FILESYSTEM_ERROR_HPP__
#define __PFS_CXX_CXX98_FILESYSTEM_FILESYSTEM_ERROR_HPP__

#if HAVE_BOOST_FILESYSTEM

#include <boost/filesystem/operations.hpp>

namespace pfs {
namespace filesystem { 
namespace details {

typedef boost::filesystem::filesystem_error filesystem_error;


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

#   error "Need to implement"

#endif

#endif /* __PFS_CXX_CXX98_FILESYSTEM_FILESYSTEM_ERROR_HPP__ */

