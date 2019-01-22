#pragma once
#include <pfs/assert.hpp>
#include <pfs/types.hpp>
#include <pfs/cxxversion.hpp>
#include PFS_CXX_HEADER(limits)

namespace pfs {

// see byte_string for usage
template <int N>
struct size_type_n;

template <>
struct size_type_n<1> 
{ 
    typedef uint8_t unsigned_type; 
    typedef int8_t signed_type;
};

template <>
struct size_type_n<2> 
{
    typedef uint16_t unsigned_type;
    typedef int16_t signed_type;
};

template <>
struct size_type_n<4> 
{
    typedef uint32_t unsigned_type;
    typedef int32_t signed_type;
};

#if PFS_HAVE_INT64
template <>
struct size_type_n<8> 
{ 
    typedef uint64_t unsigned_type; 
    typedef int64_t signed_type;
};
#endif

template <typename T1, typename T2>
inline T1 integral_cast_check (T2 x)
{
    PFS_ASSERT(static_cast<T1>(x) >= numeric_limits<T1>::min()
            && static_cast<T1>(x) <= numeric_limits<T1>::max());
    return static_cast<T1>(x);
}

} // pfs
