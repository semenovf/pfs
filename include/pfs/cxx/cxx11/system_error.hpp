/* 
 * File:   system_error.hpp
 * Author: wladt
 *
 * Created on June 21, 2017, 3:19 PM
 */

#ifndef __PFS_CXX_CXX11_SYSTEM_ERROR_HPP__
#define __PFS_CXX_CXX11_SYSTEM_ERROR_HPP__

#include <system_error>

namespace pfs {

using error_code = std::error_code;
using error_category = std::error_category;
using errc = std::errc;

inline error_category const & system_category () noexcept
{
    return std::system_category();
}

inline error_category const & generic_category () noexcept
{
    return std::generic_category();
}

inline pfs::error_code make_error_code (pfs::errc e) noexcept
{
    return std::make_error_code(e);
}
  
} // pfs

#endif /* __PFS_CXX_CXX11_SYSTEM_ERROR_HPP__ */

