/* 
 * File:   system_error.hpp
 * Author: wladt
 *
 * Created on June 21, 2017, 3:22 PM
 */

#ifndef __PFS_CXX_CXX98_SYSTEM_ERROR_HPP__
#define __PFS_CXX_CXX98_SYSTEM_ERROR_HPP__

#include <pfs/cxxlang.hpp>

#if HAVE_BOOST_SYSTEM

#include <boost/system/error_code.hpp>

namespace pfs {

typedef ::boost::system::error_code      error_code;
typedef ::boost::system::error_category  error_category;
typedef ::boost::system::error_condition error_condition;
typedef ::boost::system::errc::errc_t    errc;

inline error_category const & system_category () pfs_noexcept
{
    return ::boost::system::system_category();
}

inline error_category const & generic_category () pfs_noexcept
{
    return ::boost::system::generic_category();
}

inline error_code make_error_code (errc e) pfs_noexcept
{
    return error_code(e, system_category());
}

template <typename StringT>
inline StringT to_string (error_code const & ec)
{
    return StringT(ec.message());
}

// [Construct std::error_code from errno on POSIX and GetLastError() on Windows]
// (https://stackoverflow.com/questions/13950938/construct-stderror-code-from-errno-on-posix-and-getlasterror-on-windows)
//
inline error_code get_last_system_error ()
{
#if PFS_CC_MSC
    return error_code(::GetLastError(), system_category());
#else // POSIX
    return error_code(errno, system_category());
#endif // !PFS_CC_MSC
}

template <typename ErrorCode>
ErrorCode lexical_cast (pfs::error_code const & ec);

template <>
inline ::boost::system::error_code 
lexical_cast< ::boost::system::error_code> (::boost::system::error_code const & ec)
{
    return ec;
}

} // pfs

#else

#   error "Need to implement system_error for C++98"

#endif // HAVE_BOOST_SYSTEM

#endif /* __PFS_CXX_CXX98_SYSTEM_ERROR_HPP__ */
