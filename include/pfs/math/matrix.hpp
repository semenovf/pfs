#pragma once
#include <pfs/types.hpp>
#include <pfs/array.hpp>

namespace pfs {

template <typename T, size_t R, size_t C>
class matrix
{
    array<T, R * C> _m;
};

} // namespace pfs
