#pragma once
#include <pfs/config.h>

#if defined(PFS_CC_MSC)
#   include "chrono_win32.hpp"

#   error "Implement for Win32"

#elif HAVE_BOOST_CHRONO
#   include "chrono_boost.hpp"
#else
#   include "chrono_custom.hpp"
#endif
