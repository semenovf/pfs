#pragma once

#define __TFILE__ __FILE__

#ifndef PFS_GNUC_BACKTRACE_MAXSIZE
#   define PFS_GNUC_BACKTRACE_MAXSIZE 1024
#endif

// Default is standard error
#ifndef PFS_GNUC_ASSERT_BACKTRACE_FD
#   define PFS_GNUC_ASSERT_BACKTRACE_FD 2
#endif

// Default is standard output
#ifndef PFS_GNUC_BACKTRACE_FD
#   define PFS_GNUC_BACKTRACE_FD    1
#endif
