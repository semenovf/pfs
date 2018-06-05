#pragma once
#include <pfs/config.h>

#if HAVE_BOOST_RATIO
#   include "ratio_boost.hpp"
#else
#   include "ratio_custom.hpp"
#endif

