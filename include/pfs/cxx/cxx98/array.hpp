#pragma once
#include <pfs/config.h>

#if HAVE_BOOST_ARRAY
#   include "array_boost.hpp"
#else
#   include "array_custom.hpp"
#endif

