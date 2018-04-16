#pragma once
#include <pfs/bits/compiler.h>

#define PFS_CC_X(x) x
#define PFS_CC_HEADER(x) <pfs/compiler/__PFS_CC_HPREFIX__/PFS_CC_X(x).hpp>
