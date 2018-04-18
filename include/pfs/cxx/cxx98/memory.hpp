#pragma once
#include <pfs/cxxlang.hpp>
#include "shared_ptr.hpp"
#include "unique_ptr.hpp"

namespace pfs {

template <typename T>
struct allocator : public std::allocator<T>
{};

template <typename T>
pfs_constexpr T * addressof (T & arg)
{
    return reinterpret_cast<T *>(
        & const_cast<char &>(reinterpret_cast<const volatile char &>(arg)));
}

} // pfs
