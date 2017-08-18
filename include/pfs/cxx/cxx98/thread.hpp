/* 
 * File:   thread.hpp
 * Author: wladt
 *
 * Created on June 16, 2017, 2:25 PM
 */

#ifndef __PFS_CXX_CXX98_THREAD_HPP__
#define __PFS_CXX_CXX98_THREAD_HPP__

// Programmatically find the number of cores on a machine
// =============================================================================
// C++11
// may return 0 when not able to detect
// unsigned concurentThreadsSupported = std::thread::hardware_concurrency();
//
// Prior to C++11 see below link.
// [Programmatically find the number of cores on a machine](https://stackoverflow.com/questions/150355/programmatically-find-the-number-of-cores-on-a-machine)
//

#if defined(PFS_CC_MSC)
//#	include <windows.h>
#   include "thread_win32.hpp"

#   error "Implement for Win32"

#elif defined(HAVE_PTHREAD)
#   include <pthread.h>
#   include "thread_posix.hpp"
#else
#	error "No native support for threads"
#endif

#endif /* __PFS_CXX_CXX98_THREAD_HPP__ */

