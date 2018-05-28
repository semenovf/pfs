#pragma once
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

class bad_lexical_cast : public error_code_exception
{
public:
    bad_lexical_cast (error_code const & ec) : error_code_exception(ec) {}
    explicit bad_lexical_cast (error_code const & ec, char const * s) : error_code_exception(ec, s) {}
    explicit bad_lexical_cast (error_code const & ec, std::string const & s) : error_code_exception(ec, s) {}
};

} // pfs
