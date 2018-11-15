#pragma once
#include <pfs/config.h>

#if HAVE_BOOST_FILESYSTEM
#   include "path_boost.hpp"
#else
#   include "path_custom.hpp"
#endif

