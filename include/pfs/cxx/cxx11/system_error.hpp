/* 
 * File:   system_error.hpp
 * Author: wladt
 *
 * Created on June 21, 2017, 3:19 PM
 */

#ifndef __PFS_CXX_CXX11_SYSTEM_ERROR_HPP__
#define __PFS_CXX_CXX11_SYSTEM_ERROR_HPP__

#include <system_error>
#include <pfs/compiler.hpp>
#include <pfs/string.hpp>

// Creating custom error category:
// [std::error_category](http://www.cplusplus.com/reference/system_error/error_category/)
// [Unifying error codes](http://breese.github.io/2016/06/18/unifying-error-codes.html)
// [Your own error condition](https://akrzemi1.wordpress.com/2017/08/12/your-own-error-condition/)
// [System error support in C++0x - part 1](http://blog.think-async.com/2010/04/system-error-support-in-c0x-part-1.html)
//     Translation: [Поддержка системных ошибок в C++](https://habrahabr.ru/post/336012/)

namespace pfs {

using system_error    = std::system_error;
using error_code      = std::error_code;
using error_category  = std::error_category;
using error_condition = std::error_condition;
using errc            = std::errc;

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

template <typename StringType>
inline StringType to_string (error_code const & ec)
{
    return StringType(ec.message());
}

// [Construct std::error_code from errno on POSIX and GetLastError() on Windows](https://stackoverflow.com/questions/13950938/construct-stderror-code-from-errno-on-posix-and-getlasterror-on-windows)
//
inline pfs::error_code get_last_system_error ()
{
#if PFS_CC_MSC
    return error_code(::GetLastError(), system_category());
#else // POSIX
    return error_code(errno, generic_category());
#endif // !PFS_CC_MSC
}

template <typename ErrorCode>
ErrorCode lexical_cast (pfs::error_code const & ec);

template <>
inline std::error_code 
lexical_cast<std::error_code> (pfs::error_code const & ec)
{
    return ec;
}

} // pfs

#if HAVE_BOOST_SYSTEM

//[Unifying error codes](http://breese.github.io/2016/06/18/unifying-error-codes.html)

#include <boost/system/error_code.hpp>
#include <pfs/assert.hpp>

namespace pfs {
namespace boost {

pfs::error_category const & generic_category ();

//struct ec_convert_wrapper
//{
//    pfs::error_code & result;
//    ::boost::system::error_code ec;
//    
//    ec_convert_wrapper (pfs::error_code & r)
//        : result(r)
//    {}
//
//    ~ec_convert_wrapper ()
//    {
//        result = pfs::lexical_cast(ec);
//    }
//};

} // boost

//template <typename Result>
//Result lexical_cast (::boost::system::error_code const & ec);
//
//template <>
//inline pfs::error_code
//lexical_cast<pfs::error_code> (::boost::system::error_code const & ec)
//{
//    if (ec.category() == ::boost::system::generic_category())
//        return pfs::error_code(ec.value(), generic_category());
//    
//    PFS_ASSERT(false);
//    return pfs::error_code();
//}

template <>
inline ::boost::system::error_code
lexical_cast<::boost::system::error_code> (pfs::error_code const & ec)
{
    if (ec.category() == std::generic_category())
        return ::boost::system::error_code(ec.value(), ::boost::system::generic_category());
    
    PFS_ASSERT(false);
    return ::boost::system::error_code();
}

inline pfs::error_code make_error_code (::boost::system::errc::errc_t e)
{
    return pfs::error_code(static_cast<int>(e), boost::generic_category());
}

} // pfs

namespace std {

template <>
struct is_error_code_enum<boost::system::errc::errc_t>
    : public std::true_type
{};

} // std

#endif // HAVE_BOOST_SYSTEM

#endif /* __PFS_CXX_CXX11_SYSTEM_ERROR_HPP__ */

