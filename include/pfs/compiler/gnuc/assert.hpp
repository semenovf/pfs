#ifndef __PFS_COMPILER_GNUC_ASSERT_HPP__
#define __PFS_COMPILER_GNUC_ASSERT_HPP__

#define __TFILE__ __FILE__

#ifndef PFS_GNUC_BACKTRACE_SIZE
#   define PFS_GNUC_BACKTRACE_SIZE 20
#endif

// Default is standard error
#ifndef PFS_GNUC_ASSERT_BACKTRACE_FD
#   define PFS_GNUC_ASSERT_BACKTRACE_FD 2
#endif

// Default is standard output
#ifndef PFS_GNUC_BACKTRACE_FD
#   define PFS_GNUC_BACKTRACE_FD    1 
#endif

#endif /* __PFS_COMPILER_GNUC_ASSERT_HPP__ */

