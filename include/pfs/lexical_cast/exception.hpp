/* 
 * File:   bad_lexical_cast.hpp
 * Author: wladt
 *
 * Created on June 28, 2017, 8:28 AM
 */

#ifndef __PFS_LEXICAL_CAST_BAD_LEXICAL_CAST_HPP__
#define __PFS_LEXICAL_CAST_BAD_LEXICAL_CAST_HPP__

#include <pfs/exception.hpp>

namespace pfs {

class bad_lexical_cast : public logic_error
{
public:
    explicit bad_lexical_cast ()
        : logic_error("")
    {}

    explicit bad_lexical_cast (char const * what)
        : logic_error(what)
    {}

    explicit bad_lexical_cast (std::string const & what)
        : logic_error(what)
    {}

    virtual ~bad_lexical_cast() {}
};


#if __cplusplus >= 201103L
enum class lexical_cast_errc 
{
#else
struct lexical_cast_errc 
{
    enum {
#endif
          success = 0
        , invalid_argument
        , underflow
        , overflow
#if __cplusplus < 201103L                  
    };
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

} // pfs

namespace std {

template<>
struct is_error_code_enum<pfs::lexical_cast_errc>
        : public std::true_type 
{};

} // std

#endif /* __PFS_LEXICAL_CAST_BAD_LEXICAL_CAST_HPP__ */

