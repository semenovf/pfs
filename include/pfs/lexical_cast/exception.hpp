/* 
 * File:   exception.hpp
 * Author: wladt
 *
 * Created on June 28, 2017, 8:28 AM
 */

#ifndef __PFS_LEXICAL_CAST_EXCEPTION_HPP__
#define __PFS_LEXICAL_CAST_EXCEPTION_HPP__

#include <pfs/cxxlang.hpp>
#include <pfs/system_error.hpp>
#include <pfs/exception.hpp>

namespace pfs {

#if __cplusplus >= 201103L
enum class lexical_cast_errc 
{
#else
struct lexical_cast_errc
{
    enum value_enum {
#endif
          success = 0
        , bad_radix // bad radix
        , underflow
        , overflow
        , invalid_string
#if __cplusplus < 201103L                  
    };
    
    value_enum v;
    
    lexical_cast_errc (value_enum x)
        : v(x)
    {}

    lexical_cast_errc & operator = (value_enum x)
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
class lexical_cast_category : public pfs::error_category 
{
public:
    virtual char const * name () const pfs_noexcept pfs_override;
    virtual std::string message (int ev) const pfs_override;
};
} // details

pfs::error_category const & lexical_cast_category ();

inline pfs::error_code make_error_code (lexical_cast_errc e)
{
    return pfs::error_code(static_cast<int>(e), lexical_cast_category());
}

class bad_lexical_cast : public logic_error
{
public:
    bad_lexical_cast (pfs::error_code ec)
        : logic_error(lexical_cast_category().message(ec.value()))
    {}

    bad_lexical_cast (pfs::error_code ec, char const * what)
        : logic_error(lexical_cast_category().message(ec.value())
            + ": " + what)
    {}

    bad_lexical_cast (pfs::error_code ec, std::string const & what)
        : logic_error(lexical_cast_category().message(ec.value())
            + ": " + what)
    {}

    virtual ~bad_lexical_cast() throw() {}
};

} // pfs

namespace std {

// TODO implement for C++98
#if __cplusplus >= 201103L

template<>
struct is_error_code_enum<pfs::lexical_cast_errc>
        : public std::true_type 
{};

#endif

} // std

#endif /* __PFS_LEXICAL_CAST_EXCEPTION_HPP__ */

