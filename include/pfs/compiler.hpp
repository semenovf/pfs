/**
 * @file   compiler.hpp
 * @author wladt
 * @date   Sep 8, 2016 11:45:00
 *
 * @brief
 */

#ifndef __PFS_COMPILER_HPP__
#define __PFS_COMPILER_HPP__

#include <pfs/bits/compiler.h>

#define PFS_CC_X(x) x
#define PFS_CC_HEADER(x) <pfs/compiler/__PFS_CC_HPREFIX__/PFS_CC_X(x).hpp>

#endif /* __PFS_COMPILER_HPP__ */
