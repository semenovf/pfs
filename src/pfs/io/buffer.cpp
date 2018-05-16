#include "pfs/algorithm.hpp"
#include "pfs/limits.hpp"
#include "pfs/byte_string.hpp"
#include "pfs/io/buffer.hpp"

namespace pfs {
namespace io {
namespace details {

struct buffer : public details::device
{
    static const size_t default_buffer_size = 256;

    typedef byte_string buffer_type;

    buffer_type & _buffer;
    size_t _pos;

    buffer (buffer_type & buffer)
        : _buffer(buffer)
        , _pos(0)
    {}

    virtual bool reopen ()
    {
        return true;
    }

    virtual open_mode_flags open_mode () const
    {
        return read_write | non_blocking;
    }

    virtual ssize_t available () const
    {
        return _buffer.size() - _pos;
    }

    virtual ssize_t read (byte_t * bytes, size_t n) pfs_override;

    virtual ssize_t write (const byte_t * bytes, size_t n) pfs_override;

    virtual bool close ()
    {
        return true;
    }

    virtual bool opened () const
    {
        return true;
    }

    virtual void flush ()
    {}

    virtual bool set_nonblocking (bool)
    {
        return true;
    }

    virtual bool is_nonblocking () const
    {
        return true;
    }

    virtual native_handle_type native_handle () const
    {
        return -1;
    }

    virtual device_type type () const
    {
        return device_buffer;
    }

    virtual string url () const
    {
        return string("buffer");
    }
};

ssize_t buffer::read (byte_t * bytes, size_t n)
{
    if (_pos >= _buffer.size())
        return 0;

    n = pfs::min(n, _buffer.size() - _pos);
    _buffer.copy(bytes, n, _pos);
    _pos += n;

    return integral_cast_check<ssize_t>(n);
}

ssize_t buffer::write (byte_t const * bytes, size_t n)
{
    PFS_ASSERT(numeric_limits<size_t>::max() - _pos >= n);

    size_t size = _buffer.size() - _pos;

    if (size < n)
        _buffer.reserve(_buffer.size() + n - size);

    _buffer.append(bytes, n);

    return integral_cast_check<ssize_t>(n);
}

}}} // pfs::io::details

namespace pfs {
namespace io {

template <>
device_ptr open_device<buffer> (open_params<buffer> const & op, error_code & ec)
{
    device_ptr result(new details::buffer(op._bs));
    ec.clear();
    return result;
}

}} // pfs::io
