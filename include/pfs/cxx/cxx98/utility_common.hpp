#pragma once

#if __cplusplus >= 201103L
#   include <utility>
#else
#   include <algorithm>
#endif

#include <pfs/types.hpp>

namespace pfs {

// Avoid overload ambiguity errors with std::swap (std::swap will take precedence)
// see [Question on boost::swap](https://stackoverflow.com/questions/6115204/question-on-boostswap)
// and [Argument-dependent name lookup](https://en.wikipedia.org/wiki/Argument-dependent_name_lookup)
namespace details {
    template <typename T>
    void swap_impl(T& left, T& right)
    {
        //using namespace std;//use std::swap if argument dependent lookup fails
        std::swap(left,right);
    }

    template <typename T, size_t N>
    void swap_impl(T (& left)[N], T (& right)[N])
    {
        for (std::size_t i = 0; i < N; ++i) {
            pfs::details::swap_impl(left[i], right[i]);
        }
    }
} // details

template <typename T1, typename T2>
inline void swap (T1 & a, T2 & b)
{
    ::pfs::details::swap_impl(a, b);
}

} // pfs
