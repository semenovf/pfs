/* 
 * File:   exception.hpp
 * Author: wladt
 *
 * Created on June 27, 2017, 3:45 PM
 */

#ifndef __PFS_JSON_EXCEPTION_HPP__
#define __PFS_JSON_EXCEPTION_HPP__

#include <pfs/cxxlang.hpp>
#include <pfs/system_error.hpp>
#include <pfs/exception.hpp>

namespace pfs {

#if __cplusplus >= 201103L
enum class json_errc 
{
#else
struct json_errc 
{
    enum value_enum {
#endif
          success = 0
        , bad_json
        , excess_source
        , bad_number
        , range
#if __cplusplus < 201103L                  
    };

    value_enum v;
    
    json_errc (value_enum x)
        : v(x)
    {}

    json_errc & operator = (value_enum x)
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

class json_category : public pfs::error_category 
{
public:
    virtual char const * name () const pfs_noexcept pfs_override;
    virtual std::string message (int ev) const pfs_override;
};

} // details

pfs::error_category const & json_category ();

inline pfs::error_code make_error_code (json_errc e)
{
    return pfs::error_code(static_cast<int>(e), json_category());
}

class json_exception : public logic_error
{
public:
    json_exception (pfs::error_code ec)
        : logic_error(json_category().message(ec.value()))
    {}

    json_exception (pfs::error_code ec, char const * what)
        : logic_error(json_category().message(ec.value())
            + ": " + what)
    {}

    json_exception (pfs::error_code ec, std::string const & what)
        : logic_error(json_category().message(ec.value())
            + ": " + what)
    {}

    virtual ~json_exception() throw() {}
};

} // pfs

namespace std {

// TODO implement for C++98
#if __cplusplus >= 201103L

template<>
struct is_error_code_enum<pfs::json_errc>
        : public std::true_type 
{};

#endif

} // std

#endif /* __PFS_JSON_EXCEPTION_HPP__ */

