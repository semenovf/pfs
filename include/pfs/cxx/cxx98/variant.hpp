#pragma once
#include <pfs/config.h>
#include <pfs/cxxlang.hpp>

#if HAVE_BOOST_VARIANT
#   include "variant_boost.hpp"
#else
#   include "variant_custom.hpp"
#endif
