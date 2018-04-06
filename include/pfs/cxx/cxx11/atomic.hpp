#ifndef __PFS_CXX11_ATOMIC_HPP__
#define __PFS_CXX11_ATOMIC_HPP__

#include <atomic>

namespace pfs {

template <typename T>
using atomic = std::atomic<T>;

using atimic_bool           = std::atomic_bool;
using atomic_char           = std::atomic_char;
using atomic_schar          = std::atomic_schar;
using atomic_uchar          = std::atomic_uchar;
using atomic_short          = std::atomic_short;
using atomic_ushort         = std::atomic_ushort;
using atomic_int            = std::atomic_int;
using atomic_uint           = std::atomic_uint;
using atomic_long           = std::atomic_long;
using atomic_ulong          = std::atomic_ulong;
using atomic_llong          = std::atomic_llong;
using atomic_ullong         = std::atomic_ullong;
using atomic_char16_t       = std::atomic_char16_t;
using atomic_char32_t       = std::atomic_char32_t;
using atomic_wchar_t        = std::atomic_wchar_t;
using atomic_int_least8_t   = std::atomic_int_least8_t;
using atomic_uint_least8_t  = std::atomic_uint_least8_t;
using atomic_int_least16_t  = std::atomic_int_least16_t;
using atomic_uint_least16_t = std::atomic_uint_least16_t;
using atomic_int_least32_t  = std::atomic_int_least32_t;
using atomic_uint_least32_t = std::atomic_uint_least32_t;
using atomic_int_least64_t  = std::atomic_int_least64_t;
using atomic_uint_least64_t = std::atomic_uint_least64_t;
using atomic_int_fast8_t    = std::atomic_int_fast8_t;
using atomic_uint_fast8_t   = std::atomic_uint_fast8_t;
using atomic_int_fast16_t   = std::atomic_int_fast16_t;
using atomic_uint_fast16_t  = std::atomic_uint_fast16_t;
using atomic_int_fast32_t   = std::atomic_int_fast32_t;
using atomic_uint_fast32_t  = std::atomic_uint_fast32_t;
using atomic_int_fast64_t   = std::atomic_int_fast64_t;
using atomic_uint_fast64_t  = std::atomic_uint_fast64_t;
using atomic_intptr_t       = std::atomic_intptr_t;
using atomic_uintptr_t      = std::atomic_uintptr_t;
using atomic_size_t         = std::atomic_size_t;
using atomic_ptrdiff_t      = std::atomic_ptrdiff_t;
using atomic_intmax_t       = std::atomic_intmax_t;
using atomic_uintmax_t      = std::atomic_uintmax_t;

using atomic_int8_t   = atomic<int8_t>;
using atomic_uint8_t  = atomic<uint8_t>;
using atomic_int16_t  = atomic<int16_t>;
using atomic_uint16_t = atomic<uint16_t>;
using atomic_int32_t  = atomic<int32_t>;
using atomic_uint32_t = atomic<uint32_t>;

#if PFS_HAVE_INT64
using atomic_int64_t  = atomic<int64_t>;
using atomic_uint64_t = atomic<uint64_t>;
#endif

} // pfs

#endif /* __PFS_CXX11_ATOMIC_HPP__ */
