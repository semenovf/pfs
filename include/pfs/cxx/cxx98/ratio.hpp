#ifndef __PFS_CXX_CXX98_RATIO_HPP__
#define __PFS_CXX_CXX98_RATIO_HPP__

#include <pfs/config.h>

#if defined(PFS_CC_MSC)
#   error "Implement for Win32"

#elif defined(HAVE_BOOST_RATIO)
#   include "ratio_boost.hpp"
#else
#   include "ratio_custom.hpp"
#endif


#endif /* __PFS_CXX_CXX98_RATIO_HPP__ */

