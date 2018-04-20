#pragma once
#include <pfs/config.h>

#if defined(HAVE_BOOST_REGEX)
#   include "regex_boost.hpp"
#elif defined(PFS_CC_MSC)
#   error "Implement for Win32"
#else
#   include "regex_custom.hpp"
#endif
