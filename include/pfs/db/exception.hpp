/* 
 * File:   exception.hpp
 * Author: wladt
 *
 * Created on February 8, 2017, 4:23 PM
 */

#ifndef __PFS_DB_EXCEPTION_HPP__
#define __PFS_DB_EXCEPTION_HPP__

#include <pfs/cxxlang.hpp>
#include <pfs/system_error.hpp>
#include <pfs/exception.hpp>

namespace pfs {

#if __cplusplus >= 201103L
enum class db_errc
{
#else
struct db_errc
{
    enum value_enum {
#endif
          success = 0
        , bad_uri
        , open_fail
        , bad_alloc
        , query_fail
        , specific_error
#if __cplusplus < 201103L                  
    };
    
    value_enum v;
    
    db_errc (value_enum x)
        : v(x)
    {}

    db_errc & operator = (value_enum x)
    {
        v = x;
        return *this;
    }
    
    operator int () const
    {
        return static_cast<int>(v);
    }
#endif    
};

namespace details {
class db_category : public pfs::error_category 
{
public:
    virtual char const * name () const pfs_noexcept pfs_override;
    virtual std::string message (int ev) const pfs_override;
};
} // details

pfs::error_category const & db_category ();

inline pfs::error_code make_error_code (db_errc e)
{
    return pfs::error_code(static_cast<int>(e), db_category());
}

class db_exception : public logic_error
{
public:
    db_exception (pfs::error_code ec)
        : logic_error(db_category().message(ec.value()))
    {}

    db_exception (pfs::error_code ec, char const * what)
        : logic_error(db_category().message(ec.value())
            + ": " + what)
    {}

    db_exception (pfs::error_code ec, std::string const & what)
        : logic_error(db_category().message(ec.value())
            + ": " + what)
    {}

    virtual ~db_exception() throw() {}
};

} // pfs

namespace std {

// TODO implement for C++98
#if __cplusplus >= 201103L

template<>
struct is_error_code_enum<pfs::db_errc>
        : public std::true_type 
{};

#endif
} // std

#endif /* __PFS_DB_EXCEPTION_HPP__ */

