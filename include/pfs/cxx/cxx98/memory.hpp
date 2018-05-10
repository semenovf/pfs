#pragma once
#include <pfs/config.h>
#include <pfs/cxxlang.hpp>

#if defined(HAVE_BOOST_SMARTPTR)
#   include "shared_ptr_boost.hpp"
#   include "unique_ptr_boost.hpp"
#else
#   include "shared_ptr_custom.hpp"
#   include "unique_ptr_custom.hpp"
#endif

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
