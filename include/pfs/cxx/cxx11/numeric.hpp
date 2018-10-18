#pragma once
#include <pfs/cxx/cxx98/numeric.hpp>

namespace pfs {

template <typename ForwardIt, typename T>
inline void iota (ForwardIt first, ForwardIt last, T value)
{
    std::iota(first, last, value);
}

} // pfs
