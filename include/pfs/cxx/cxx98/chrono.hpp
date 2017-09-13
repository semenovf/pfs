/* 
 * File:   chrono.hpp
 * Author: wladt
 *
 * Created on June 19, 2017, 9:05 AM
 */

#ifndef __PFS_CXX_CXX98_CHRONO_HPP__
#define __PFS_CXX_CXX98_CHRONO_HPP__

#if defined(PFS_CC_MSC)
//#	include <windows.h>
#   include "chrono_win32.hpp"

#   error "Implement for Win32"

#elif defined(HAVE_BOOST_CHRONO)
#   include "chrono_boost.hpp"
#else
#	include "chrono_custom.hpp"
#endif

#endif /* __PFS_CXX_CXX98_CHRONO_HPP__ */

