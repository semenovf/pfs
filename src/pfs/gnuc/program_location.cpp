#ifndef _GNU_SOURCE
#   define _GNU_SOURCE // for extern char *program_invocation_name;
#endif
#include <errno.h>
#include "pfs/program_location.hpp"

namespace pfs {

inline filesystem::path program_location (error_code & ec)
{
    return filesystem::path(program_invocation_name);
}

filesystem::path program_location ()
{
    error_code ec;
    filesystem::path result = program_location(ec);
    return result;
}

} // namespace pfs
