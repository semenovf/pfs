#pragma once
#include <pfs/string.hpp>
#include <pfs/cxxversion.hpp>
#include PFS_CXX_HEADER(filesystem/path)

namespace pfs {

inline string to_string (filesystem::path const & p)
{
    return string(p.native().begin(), p.native().end());
}

} // pfs
