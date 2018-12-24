#pragma once
#include <pfs/config.h>

#if HAVE_BOOST
#   include <boost/version.hpp>
#endif

// // TODO DEPRECATED
// #ifdef pfs_constexpr
// #   undef pfs_constexpr
// #endif
// 
// // TODO DEPRECATED
// #ifdef pfs_noexcept
// #   undef pfs_noexcept
// #endif
// 
// // TODO DEPRECATED
// #ifdef pfs_override
// #   undef pfs_override
// #endif
// 
// // TODO DEPRECATED
// #if __cplusplus >= 201103L
// #   define pfs_constexpr           constexpr
// #   define pfs_noexcept            noexcept
// #   define pfs_noexcept_expr(expr) noexcept(expr)
// #   define pfs_override            override
// #else
// #   define pfs_constexpr
// #   define pfs_noexcept
// #   define pfs_noexcept_expr(expr)
// #   define pfs_override
// #endif

#ifdef constexpr
#   undef constexpr
#endif

#ifdef noexcept
#   undef noexcept
#endif

#ifdef override
#   undef override
#endif

#if __cplusplus < 201103L
#   define constexpr
#   define noexcept
#   define noexcept_expr(expr)
#   define override
#endif
//
// https://infektor.net/posts/2017-01-19-using-cpp17-attributes-today.html
// http://clang.llvm.org/docs/LanguageExtensions.html#has-cpp-attribute
//
// Bellow are C++17 three additional attributes were added to the standard which
// enable additional compiler warnings when used in general code.
//
// nodiscard
//          specifies that a return value should not be discarded
// fallthrough
//          indicates to the compiler that you intended a case in a switch
//          statement to fall-through, requires an optional warning to be enabled
// maybe_unused
//          silences warnings generated when a variable is only used in an assertion
//
#ifndef __has_cpp_attribute
#   define __has_cpp_attribute(name) 0
#endif

//
// nodiscard
//
#if __has_cpp_attribute(nodiscard)
#   define pfs_nodiscard [[nodiscard]]
#elif __has_cpp_attribute(gnu::warn_unused_result)
#   define pfs_nodiscard [[gnu::warn_unused_result]]
#else
#   define pfs_nodiscard
#endif

//
// fallthrough
//
#if __has_cpp_attribute(fallthrough)
#   define pfs_fallthrough [[fallthrough]]
#elif __has_cpp_attribute(clang::fallthrough)
#   define pfs_fallthrough [[clang::fallthrough]]
#else
#   define pfs_fallthrough
#endif

//
// maybe_unused
//
#if __has_cpp_attribute(maybe_unused)
#   define pfs_maybe_unused [[maybe_unused]]
#elif __has_cpp_attribute(gnu::unused)
#   define pfs_maybe_unused [[gnu::unused]]
#else
#   define pfs_maybe_unused
#endif


// static_assert (expression, message)
//
#if __cplusplus < 201103L

// [STATIC_ASSERT](http://www.pixelbeat.org/programming/gcc/static_assert.html)

#   define PFS_ASSERT_CONCAT_P(a, b) a##b
#   define PFS_ASSERT_CONCAT(a, b) PFS_ASSERT_CONCAT_P(a, b)
/* These can't be used after statements in c89. */
#   ifdef __COUNTER__
#       define static_assert(e,m) \
            ;enum { PFS_ASSERT_CONCAT(static_assert_, __COUNTER__) = 1/static_cast<int>(!!(e)) }
#   else
  /* This can't be used twice on the same line so ensure if using in headers
   * that the headers are not included twice (by wrapping in #ifndef...#endif)
   * Note it doesn't cause an issue when used on same line of separate modules
   * compiled with gcc -combine -fwhole-program.  */
#       define static_assert(e,m) \
            ;enum { PFS_ASSERT_CONCAT(assert_line_, __LINE__) = 1/static_cast<int>(!!(e)) }
#   endif

#endif

#ifndef PFS_AUTO
#   if __cplusplus >= 201103L
#       define PFS_AUTO(var,expr) auto var = expr
#   elif defined(BOOST_VERSION) && BOOST_VERSION >= 103400 // BOOST_AUTO since 1.34
#       include <boost/typeof/typeof.hpp>
#       define PFS_TYPEOF(expr) BOOST_TYPEOF(expr)
#       define PFS_AUTO(var,expr) BOOST_AUTO(var, expr)
#   else
#       include <3rdparty/boost/typeof/typeof.hpp>
#       define PFS_TYPEOF(expr) BOOST_TYPEOF(expr)
#       define PFS_AUTO(var,expr) BOOST_AUTO(var, expr)
#   endif
#endif
