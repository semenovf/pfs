#pragma once
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
        , array_expected
        , object_expected
        , ubjson_parse
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
    virtual char const * name () const noexcept override;
    virtual std::string message (int ev) const override;
};

} // details

pfs::error_category const & json_category ();

inline pfs::error_code make_error_code (json_errc e)
{
    return pfs::error_code(static_cast<int>(e), json_category());
}

class json_exception : public error_code_exception
{
public:
    json_exception (error_code const & ec) : error_code_exception(ec) {}
    explicit json_exception (error_code const & ec, char const * s) : error_code_exception(ec, s) {}
    explicit json_exception (error_code const & ec, std::string const & s) : error_code_exception(ec, s) {}
};

} // pfs
