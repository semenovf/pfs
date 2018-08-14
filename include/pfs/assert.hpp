#pragma once
#include <string>

void pfs_backtrace (char const * file, int line, char const * text);
void pfs_assert (char const * file, int line, char const * text);
void pfs_check_warn (char const * file, int line, char const * text);
void pfs_check_error (char const * file, int line, char const * text);

#include <pfs/compiler.hpp>
#include PFS_CC_HEADER(assert)

#ifndef __TFILE__
#   error "__TFILE__ must be defined"
#endif

#ifndef NDEBUG

#   define PFS_ASSERT(expr)        if (!(expr)) { pfs_assert(__TFILE__, __LINE__, #expr); }
#   define PFS_ASSERT_X(expr,text) if (!(expr)) { pfs_assert(__TFILE__, __LINE__, text); }
#   define PFS_BACKTRACE(text)     pfs_backtrace(__TFILE__, __LINE__, text)

#else // NDEBUG

#   define PFS_ASSERT(x)
#   define PFS_ASSERT_X(x,y)
#   define PFS_BACKTRACE(text)

#endif /* !NDEBUG */

#define PFS_ASSERT_UNEXPECTED() PFS_ASSERT(false)
#define PFS_ASSERT_NULLPTR(x)   PFS_ASSERT((x) != 0)

#define PFS_WARN(x) if (!(x)) { pfs_check_warn(__TFILE__, __LINE__, #x); }
#define PFS_ERROR(x) if (!(x)) { pfs_check_error(__TFILE__, __LINE__, #x); }
#define PFS_WARN_X(x,text) if (!(x)) { pfs_check_warn(__TFILE__, __LINE__, text); }
#define PFS_ERROR_X(x,text) if (!(x)) { pfs_check_error(__TFILE__, __LINE__, text); }

/*
 * Special case of assert.
 * Used when need to implement some code.
 */
#define PFS_ASSERT_TODO() PFS_ASSERT_X(false, "Need to implement")
