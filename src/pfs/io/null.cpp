#include "pfs/limits.hpp"
#include "pfs/io/null.hpp"


namespace pfs {
namespace io {
namespace details {

struct null : public details::device
{
    null () {}

    virtual error_code reopen () override
    {
        return error_code();
    }

    virtual open_mode_flags open_mode () const override
    {
        return read_write | non_blocking;
    }

    virtual ssize_t available () const override
    {
        return 0;
    }

    virtual ssize_t read (byte_t * /*bytes*/, size_t /*n*/, error_code &) noexcept override
    {
        return 0;
    }

    virtual ssize_t write (const byte_t * /*bytes*/, size_t n, error_code &) noexcept override
    {
        return integral_cast_check<ssize_t>(n);
    }

    virtual error_code close () override
    {
        return error_code();
    }

    virtual bool opened () const override
    {
        return true;
    }

    virtual void flush () override
    { }

    virtual bool is_nonblocking() const override
    {
        return true;
    }

    virtual bool set_nonblocking (bool) override
    {
        return true;
    }

    virtual native_handle_type native_handle () const override
    {
        return -1;
    }

    virtual device_type type () const override
    {
        return device_null;
    }

    virtual string url () const override
    {
        return string("null");
    }
};

}}} // pfs::io::details

namespace pfs { namespace io {

template <>
device_ptr open_device<null> (open_params<null> const &, error_code & ec)
{
    ec.clear();
    return device_ptr(new details::null);
}

}} // pfs::io
