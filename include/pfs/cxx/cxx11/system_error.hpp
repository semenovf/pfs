#pragma once
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

inline string to_string (error_code const & ec)
{
    return string(ec.message());
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

template <>
inline error_code_converter_helper<::std::error_code, ::std::error_code>::~error_code_converter_helper ()
{
    targetref = origref;
}

} // pfs

//#if HAVE_BOOST_SYSTEM_ERROR
#if 0

//[Unifying error codes](http://breese.github.io/2016/06/18/unifying-error-codes.html)

#include <boost/system/error_code.hpp>
#include <pfs/assert.hpp>

namespace pfs {
namespace boost {

pfs::error_category const & generic_category ();
pfs::error_category const & system_category ();

} // boost

template <>
inline error_code error_code_cast<::boost::system::error_code
        , error_code> (::boost::system::error_code const & ec)
{
    if (ec.category() == ::boost::system::generic_category()) {
        return pfs::error_code(ec.value(), pfs::generic_category());
    } else if (ec.category() == ::boost::system::system_category()) {
        return pfs::error_code(ec.value(), pfs::system_category());
    } else {
        PFS_ASSERT_X(false, "Can't cast `::boost::system::error_code`"
                " to `pfs::error_code`");
    }
}

template <>
inline ::boost::system::error_code error_code_cast<error_code
        , ::boost::system::error_code> (error_code const & ec)
{
    if (ec.category() == pfs::generic_category()) {
        return ::boost::system::error_code(ec.value()
                , ::boost::system::generic_category());
    } else if (ec.category() == pfs::system_category()) {
        return ::boost::system::error_code(ec.value()
                , ::boost::system::system_category());
    } else {
        PFS_ASSERT_X(false, "Can't cast `pfs::error_code`"
                " to `::boost::system::error_code`");
    }

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

#endif // HAVE_BOOST_SYSTEM_ERROR
