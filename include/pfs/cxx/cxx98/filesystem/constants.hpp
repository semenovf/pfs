#pragma once
#include <pfs/config.h>

#if HAVE_BOOST_FILESYSTEM
#   include "constants_boost.hpp"
#else
#   include "constants_custom.hpp"
#endif

