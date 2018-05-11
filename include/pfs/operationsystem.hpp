#pragma once
#include <pfs/bits/operationsystem.h>

#define PFS_OS_X(x) x
#define PFS_OS_HEADER(x) <pfs/operationsystem/__PFS_OS_HPREFIX__/PFS_OS_X(x).hpp>
