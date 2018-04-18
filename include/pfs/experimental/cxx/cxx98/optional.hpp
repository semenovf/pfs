#pragma once
#include <pfs/config.h>

#if defined(HAVE_BOOST_OPTIONAL)
#   include "optional_boost.hpp"
#else
#   error "optional: no default yet"
#endif
