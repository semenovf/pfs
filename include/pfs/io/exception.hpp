#pragma once
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
    enum value_enum {
#endif
          success = 0
        , operation_in_progress
        , connection_refused
        , bad_file_descriptor
        , stream
        , timeout
#if __cplusplus < 201103L
    };

    value_enum v;

    io_errc (value_enum x)
        : v(x)
    {}

    io_errc & operator = (value_enum x)
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
class io_category : public pfs::error_category
{
public:
    virtual char const * name () const noexcept override;
    virtual std::string message (int ev) const override;
};
} // details

pfs::error_category const & io_category ();

inline pfs::error_code make_error_code (io_errc e)
{
    return pfs::error_code(static_cast<int>(e), io_category());
}

class io_exception : public error_code_exception
{
public:
    io_exception (error_code const & ec) : error_code_exception(ec) {}
    explicit io_exception (error_code const & ec, char const * s) : error_code_exception(ec, s) {}
    explicit io_exception (error_code const & ec, std::string const & s) : error_code_exception(ec, s) {}
};

} // pfs
