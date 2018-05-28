#pragma once
#include <pfs/cxxlang.hpp>
#include <pfs/system_error.hpp>
#include <pfs/string.hpp>
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

class db_exception : public error_code_exception
{
public:
    db_exception (error_code const & ec) : error_code_exception(ec) {}
    explicit db_exception (error_code const & ec, char const * s) : error_code_exception(ec, s) {}
    explicit db_exception (error_code const & ec, std::string const & s) : error_code_exception(ec, s) {}
};

} // pfs
