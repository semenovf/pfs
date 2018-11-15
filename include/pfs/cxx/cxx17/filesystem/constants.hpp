#pragma once

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

