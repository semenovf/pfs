#include <pfs/system_error.hpp>
#include <pfs/exception.hpp>
#include <pfs/operationsystem.hpp>
#include PFS_OS_HEADER(getusername)

namespace pfs {

/**
 * @fn pfs::string getusername (error_code & ec)
 *
 * @return
 */

/**
 *
 */
inline pfs::string getusername ()
{
    error_code ec;
    string result = getusername(ec);

    if (ec)
        PFS_THROW(pfs::runtime_error(ec.message()));

    return result;
}

} // namespace pfs
