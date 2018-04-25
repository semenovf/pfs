#pragma once
#include <pfs/algorithm.hpp>
#include <pfs/memory.hpp>
#include <pfs/utility.hpp>
#include <pfs/io/bits/device.hpp>
#include <pfs/byte_string.hpp>
#include <pfs/compiler.hpp>

// TODO Apply support of device types: RandomAccessDevice and StreamDevice

namespace pfs { namespace io {

template <typename DeviceImpl>
struct open_params;

class server;
class device;

class device
{
    friend class server;

public:
    typedef bits::device::native_handle_type native_handle_type;
    typedef bits::device::open_mode_flags    open_mode_flags;
    typedef bits::device::open_mode_type     open_mode_type;
    typedef bits::device::context_type       context_type;
    typedef bits::device::string_type        string_type;

protected:
    shared_ptr<bits::device> _d;
    //unique_ptr<bits::device> _d;

protected:
    device (bits::device * p)
        : _d(p)
    {}

public:
    device () {}

    ~device ()
    {}

#if __cplusplus >= 201103L
    device (device const & rhs) = default;
#endif

    native_handle_type native_handle () const
    {
        return _d->native_handle();
    }

    bool reopen ()
    {
        return _d->reopen();
    }

    operator bool () const
    {
        return opened();
    }

    bool is_error () const
    {
        return _d->is_error();
    }

    error_code errorcode () const
    {
        return _d->errorcode();
    }

    bool is_null () const
    {
        return !_d;
    }

    bool is_readable () const
    {
        return _d->open_mode() | read_only;
    }

    bool is_writable () const
    {
        return _d->open_mode() | write_only;
    }

    bool is_nonblocking () const
    {
        return _d->is_nonblocking();
    }

    bool opened () const
    {
        return _d ? _d->opened() : false;
    }

    void flush ()
    {
        if (_d) _d->flush();
    }

    bool set_nonblocking (bool on)
    {
    	return _d->set_nonblocking(on);
    }

    /**
     * @brief Close device.
     *
     * @return error code value.
     *
     * @see    device::notification()
     */
    bool close ();

    ssize_t available () const
    {
        return _d->bytes_available();
    }

    bool at_end () const
    {
        return _d->bytes_available() == ssize_t(0);
    }

    /**
        * @brief Read bytes from the device.
        */
    ssize_t read (byte_t * bytes, size_t n)
    {
        return _d->read(bytes, n);
    }

    ssize_t read (char * chars, size_t n)
    {
        return read(reinterpret_cast<byte_t *>(chars), n);
    }

    /**
     * @brief Read data from device and appends them
     */
    bool read (byte_string & bytes)
    {
    	return read(bytes, available());
    }

    bool read (byte_string & bytes, ssize_t n);

    /**
     * @brief Write bytes to the device.
     */
    ssize_t write (const byte_t * bytes, size_t n)
    {
        return _d->write(bytes, n);
    }

    ssize_t write (const char * chars, size_t n)
    {
        return write(reinterpret_cast<const byte_t *>(chars), n);
    }

    ssize_t write (byte_string const & bytes, size_t n)
    {
        return this->write(bytes.data(), pfs::min(n, bytes.size()));
    }

    ssize_t write (byte_string const & bytes)
    {
        return this->write(bytes.data(), bytes.size());
    }

    device_type type () const
    {
        return _d->type();
    }

    void set_context (context_type * ctx)
    {
        _d->set_context(ctx);
    }

    context_type const * context () const
    {
        return _d->context();
    }

    context_type * context ()
    {
        return _d->context();
    }

    string_type url () const
    {
        return _d->url();
    }

    void swap (device & other)
    {
        _d.swap(other._d);
    }

    bool operator == (device const & other)
    {
        return _d == other._d;
    }

    bool operator != (device const & other)
    {
        return ! operator == (other);
    }

    template <typename DeviceTag>
    friend device open_device (open_params<DeviceTag> const &, error_code & ec);

//    friend bool compress (device & dest, device & src, zlib::compression_level level, size_t chunkSize, error_code * ex = 0);
//
//    friend bool uncompress (device & dest, device & src, size_t chunkSize, error_code * ex = 0);
};

ssize_t copy (device & dest, device & src, size_t chunkSize, error_code * ec = 0);

//inline bool compress (device & src, device & dest, error_code * ex = 0)
//{
//  return compress(src, dest, zlib::DefaultCompression, 0x4000, ex);
//}
//
//inline bool uncompress (device & src, device & dest, error_code * ex = 0)
//{
//	return uncompress(src, dest, 0x4000, ex);
//}

template <typename DeviceTag>
device open_device (open_params<DeviceTag> const &, error_code & ec);

template <typename DeviceTag>
inline device open_device (open_params<DeviceTag> const & op)
{
    error_code ec;
    return open_device<DeviceTag>(op, ec);
}

template <typename DeviceTag>
inline device open_device ()
{
    error_code ec;
    open_params<DeviceTag> op;
    return open_device<DeviceTag>(op, ec);
}

}} // pfs::io
