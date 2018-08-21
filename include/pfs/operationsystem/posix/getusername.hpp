#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <pfs/string.hpp>
#include <pfs/system_error.hpp>

namespace pfs {

string getusername (error_code & ec)
{
    errno = 0;
    uid_t uid = getuid();
    passwd * pw = getpwuid(uid);

    if (!pw) {
        if (errno)
            ec = get_last_system_error();
        return string();
    }

    return string(pw->pw_name);
}

} // namespace pfs
