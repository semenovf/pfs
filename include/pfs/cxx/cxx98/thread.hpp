#pragma once
#include <pfs/config.h>
#include <pfs/cxxlang.hpp>

// Programmatically find the number of cores on a machine
// =============================================================================
// C++11
// may return 0 when not able to detect
// unsigned concurentThreadsSupported = std::thread::hardware_concurrency();
//
// Prior to C++11 see below link.
// [Programmatically find the number of cores on a machine](https://stackoverflow.com/questions/150355/programmatically-find-the-number-of-cores-on-a-machine)
//

#if HAVE_BOOST_THREAD
#   include "thread_boost.hpp"
#else
#   include "thread_custom.hpp"
#endif

