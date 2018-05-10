#pragma once
#include <pfs/cxx/cxx98/utility_common.hpp>

namespace pfs {

template <typename T1, typename T2>
using pair = std::pair<T1, T2>;

template <typename T1, typename T2>
inline pair<T1, T2> make_pair (T1 x, T2 y)
{
    return std::make_pair(x, y);
}

} // pfs
