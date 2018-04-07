#pragma once
#include <pfs/config.h>

#if defined(HAVE_BOOST_RATIO)
#   include "optional_boost.hpp"
#else
#   error "optional: no implementation yet"
#endif
