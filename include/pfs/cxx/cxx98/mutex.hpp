#pragma once
#include <pfs/config.h>
#include <pfs/cxxlang.hpp>

#if HAVE_BOOST_THREAD
#   include "mutex_boost.hpp"
#else
#   include "mutex_custom.hpp"
#endif
