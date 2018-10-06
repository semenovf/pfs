#pragma once

// Programmatically find the number of cores on a machine
// =============================================================================
// C++11
// may return 0 when not able to detect
// unsigned concurentThreadsSupported = std::thread::hardware_concurrency();
//
// Prior to C++11 see below link.
// [Programmatically find the number of cores on a machine](https://stackoverflow.com/questions/150355/programmatically-find-the-number-of-cores-on-a-machine)
//

#include <pfs/config.h>

#if defined(PFS_CC_MSC)
#   include "thread_win32.hpp"

#   error "Implement for Win32"

#elif HAVE_BOOST_THREAD
#   include "thread_boost.hpp"
#elif HAVE_PTHREAD
#   include "thread_posix.hpp"
#else
#   error "No native support for threads"
#endif

