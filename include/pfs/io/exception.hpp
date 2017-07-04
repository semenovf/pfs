/* 
 * File:   exception.hpp
 * Author: wladt
 *
 * Created on July 3, 2017, 3:52 PM
 */

#ifndef __PFS_IO_EXCEPTION_HPP__
#define __PFS_IO_EXCEPTION_HPP__

#include <pfs/cxxlang.hpp>
#include <pfs/system_error.hpp>
#include <pfs/exception.hpp>

namespace pfs {

#if __cplusplus >= 201103L
enum class io_errc 
{
#else
struct io_errc 
{
    enum {
#endif
          success = 0
        , operation_in_progress
        , connection_refused
        , bas_file_descriptor
//        , bad_radix // bad radix
//        , underflow
//        , overflow
//        , invalid_string
#if __cplusplus < 201103L                  
    };
#endif    
};

namespace details {
class io_category : public pfs::error_category 
{
public:
    virtual char const * name () const pfs_noexcept pfs_override;
    virtual std::string message (int ev) const pfs_override;
};
} // details

pfs::error_category const & io_category ();

inline pfs::error_code make_error_code (io_errc e)
{
    return pfs::error_code(static_cast<int>(e), io_category());
}

class io_exception : public logic_error
{
public:
    io_exception (pfs::error_code ec)
        : logic_error(io_category().message(ec.value()))
    {}

    io_exception (pfs::error_code ec, char const * what)
        : logic_error(io_category().message(ec.value())
            + ": " + what)
    {}

    io_exception (pfs::error_code ec, std::string const & what)
        : logic_error(io_category().message(ec.value())
            + ": " + what)
    {}

    virtual ~io_exception() {}
};

} // pfs

namespace std {

template<>
struct is_error_code_enum<pfs::io_errc>
        : public std::true_type 
{};

} // std

#endif /* __PFS_IO_EXCEPTION_HPP__ */

