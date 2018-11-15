#pragma once
#include <pfs/config.h>

#if HAVE_BOOST_FILESYSTEM
#   include "directory_entry_boost.hpp"
#else
#   include "directory_entry_custom.hpp"
#endif

