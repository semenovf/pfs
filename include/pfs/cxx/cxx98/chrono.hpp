#ifndef __PFS_CXX_CXX98_CHRONO_HPP__
#define __PFS_CXX_CXX98_CHRONO_HPP__

#include <pfs/config.h>

#if defined(PFS_CC_MSC)
#   include "chrono_win32.hpp"

#   error "Implement for Win32"

#elif defined(HAVE_BOOST_CHRONO)
#   include "chrono_boost.hpp"
#else
#   include "chrono_custom.hpp"
#endif

#endif /* __PFS_CXX_CXX98_CHRONO_HPP__ */

