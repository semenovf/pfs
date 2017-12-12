#ifndef __PFS_DEBUG_HPP__
#define __PFS_DEBUG_HPP__

#include <cstdio>
#include <iomanip>
#include <iostream>

#ifndef NDEBUG
#	define PFS_DEBUG(x) x
#else
#	define PFS_DEBUG(x)
#endif

#endif /* __PFS_DEBUG_HPP__ */
