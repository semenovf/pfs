#include "pfs/limits.hpp"
#include "pfs/io/null.hpp"


namespace pfs {
namespace io {
namespace details {

struct null : public details::device
{
    null () {}

    virtual error_code reopen () pfs_override
    {
        return error_code();
    }

    virtual open_mode_flags open_mode () const pfs_override
    {
        return read_write | non_blocking;
    }

    virtual ssize_t available () const pfs_override
    {
        return 0;
    }

    virtual ssize_t read (byte_t * /*bytes*/, size_t /*n*/, error_code &) pfs_noexcept pfs_override
    {
        return 0;
    }

    virtual ssize_t write (const byte_t * /*bytes*/, size_t n, error_code &) pfs_noexcept pfs_override
    {
        return integral_cast_check<ssize_t>(n);
    }

    virtual error_code close () pfs_override
    {
        return error_code();
    }

    virtual bool opened () const pfs_override
    {
        return true;
    }

    virtual void flush () pfs_override
    { }

    virtual bool is_nonblocking() const pfs_override
    {
        return true;
    }

    virtual bool set_nonblocking (bool) pfs_override
    {
        return true;
    }

    virtual native_handle_type native_handle () const pfs_override
    {
        return -1;
    }

    virtual device_type type () const pfs_override
    {
        return device_null;
    }

    virtual string url () const pfs_override
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
