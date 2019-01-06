#pragma once
#include <pfs/config.h>

#if HAVE_STDCXX_FILESYSTEM

#include <filesystem>

namespace pfs {
namespace filesystem {
namespace details {

using perms = std::filesystem::perms;

}}} // pfs::filesystem::details

#else

#include <pfs/cxx/cxx14/filesystem/constants.hpp>

#endif

