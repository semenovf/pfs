#ifndef __PFS_CXX11_ATOMIC_HPP__
#define __PFS_CXX11_ATOMIC_HPP__

#include <atomic>

namespace pfs {

template <typename T>
using atomic = std::atomic<T>;

typedef std::atomic_char            atomic_char;
typedef std::atomic_schar           atomic_schar;
typedef std::atomic_uchar           atomic_uchar;
typedef std::atomic_short           atomic_short;
typedef std::atomic_ushort          atomic_ushort;
typedef std::atomic_int             atomic_int;
typedef std::atomic_uint            atomic_uint;
typedef std::atomic_long            atomic_long;
typedef std::atomic_ulong           atomic_ulong;
typedef std::atomic_llong           atomic_llong;
typedef std::atomic_ullong          atomic_ullong;
typedef std::atomic_char16_t        atomic_char16_t;
typedef std::atomic_char32_t        atomic_char32_t;
typedef std::atomic_wchar_t         atomic_wchar_t;
typedef std::atomic_int_least8_t    atomic_int_least8_t;
typedef std::atomic_uint_least8_t   atomic_uint_least8_t;
typedef std::atomic_int_least16_t   atomic_int_least16_t;
typedef std::atomic_uint_least16_t  atomic_uint_least16_t;
typedef std::atomic_int_least32_t   atomic_int_least32_t;
typedef std::atomic_uint_least32_t  atomic_uint_least32_t;
typedef std::atomic_int_least64_t   atomic_int_least64_t;
typedef std::atomic_uint_least64_t  atomic_uint_least64_t;
typedef std::atomic_int_fast8_t     atomic_int_fast8_t;
typedef std::atomic_uint_fast8_t    atomic_uint_fast8_t;
typedef std::atomic_int_fast16_t    atomic_int_fast16_t;
typedef std::atomic_uint_fast16_t   atomic_uint_fast16_t;
typedef std::atomic_int_fast32_t    atomic_int_fast32_t;
typedef std::atomic_uint_fast32_t   atomic_uint_fast32_t;
typedef std::atomic_int_fast64_t    atomic_int_fast64_t;
typedef std::atomic_uint_fast64_t   atomic_uint_fast64_t;
typedef std::atomic_intptr_t        atomic_intptr_t;
typedef std::atomic_uintptr_t       atomic_uintptr_t;
typedef std::atomic_size_t          atomic_size_t;
typedef std::atomic_ptrdiff_t       atomic_ptrdiff_t;
typedef std::atomic_intmax_t        atomic_intmax_t;
typedef std::atomic_uintmax_t       atomic_uintmax_t;

typedef atomic<int8_t>   atomic_int8_t;
typedef atomic<uint8_t>  atomic_uint8_t;
typedef atomic<int16_t>  atomic_int16_t;
typedef atomic<uint16_t> atomic_uint16_t;
typedef atomic<int32_t>  atomic_int32_t;
typedef atomic<uint32_t> atomic_uint32_t;

#if PFS_HAVE_INT64
typedef atomic<int64_t>  atomic_int64_t;
typedef atomic<uint64_t> atomic_uint64_t;
#endif

} // pfs

#endif /* __PFS_CXX11_ATOMIC_HPP__ */
