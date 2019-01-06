#pragma once
#include <pfs/config.h>
#include <pfs/cxxlang.hpp>

#if HAVE_BOOST_SMART_PTR
#   include "shared_ptr_boost.hpp"
// #   include "unique_ptr_boost.hpp" // old libraries has no unique_ptr 
#   include "unique_ptr_custom.hpp"
#else
#   include "shared_ptr_custom.hpp"
#   include "unique_ptr_custom.hpp"
#endif

namespace pfs {

template <typename T>
struct allocator : public std::allocator<T>
{};

template <typename T>
constexpr T * addressof (T & arg)
{
    return reinterpret_cast<T *>(
        & const_cast<char &>(reinterpret_cast<const volatile char &>(arg)));
}

} // pfs
