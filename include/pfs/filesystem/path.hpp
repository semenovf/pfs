#pragma once
#include <pfs/cxxversion.hpp>
#include PFS_CXX_HEADER(filesystem/path)

namespace pfs {

template <typename StringType>
StringType to_string (filesystem::path const & p)
{
    return StringType(p.native());
}

} // pfs
