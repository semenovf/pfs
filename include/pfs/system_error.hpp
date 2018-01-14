#ifndef __PFS_SYSTEM_ERROR_HPP__
#define __PFS_SYSTEM_ERROR_HPP__

#include <pfs/cxxversion.hpp>
#include PFS_CXX_HEADER(system_error)

template <typename ErrorCode>
ErrorCode lexical_cast (pfs::error_code const & ec);

#endif /* __PFS_SYSTEM_ERROR_HPP__ */
