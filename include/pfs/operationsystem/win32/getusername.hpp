#include <windows.h>
#include <lmcons.h> // UNLEN defined here
#include <pfs/string.hpp>
#include <pfs/system_error.hpp>

namespace pfs {

string getusername (error_code & ec)
{
    DWORD len = UNLEN + 1;
    char buffer[UNLEN + 1];

    if (!GetUserName(buffer, & len)) {
        ec = get_last_system_error();
        return string();
    }

    return string(buffer);
}

} // namespace pfs

