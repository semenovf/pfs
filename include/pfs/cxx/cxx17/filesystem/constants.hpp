#ifndef __PFS_CXX_CXX17_FILESYSTEM_CONSTANTS_HPP__
#define __PFS_CXX_CXX17_FILESYSTEM_CONSTANTS_HPP__

#include <pfs/compiler.hpp>

#if PFS_CC_GCC_VERSION >= 80000

#include <filesystem>

namespace pfs {
namespace filesystem {
namespace details {

    using perms = std::filesystem::perms;

}}} // pfs::filesystem::details

#else

#include <pfs/cxx/cxx14/filesystem/constants.hpp>

#endif


#endif /* __PFS_CXX_CXX17_FILESYSTEM_CONSTANTS_HPP__ */

