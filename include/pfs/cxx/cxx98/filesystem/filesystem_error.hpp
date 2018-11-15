#pragma once
#include <pfs/config.h>

#if HAVE_BOOST_FILESYSTEM
#   include "filesystem_error_boost.hpp"
#else
#   include "filesystem_error_custom.hpp"
#endif
