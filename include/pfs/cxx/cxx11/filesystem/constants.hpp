/* 
 * File:   constants.hpp
 * Author: wladt
 *
 * Created on July 4, 2017, 9:01 AM
 */

#ifndef __PFS_CXX_CXX11_FILESYSTEM_CONSTANTS_HPP__
#define __PFS_CXX_CXX11_FILESYSTEM_CONSTANTS_HPP__

#include <pfs/compiler.hpp>

#if PFS_CC_GCC_VERSION >= 50300

#include <experimental/filesystem>

namespace pfs {
namespace filesystem { 
namespace details {

using perms = std::experimental::filesystem::perms;

}}} // pfs::filesystem::details

#else

#include <pfs/cxx/cxx98/filesystem/constants.hpp>

#endif

#endif /* __PFS_CXX_CXX11_FILESYSTEM_CONSTANTS_HPP__ */

