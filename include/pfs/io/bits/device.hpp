#pragma once
#include <pfs/types.hpp>
#include <pfs/io/exception.hpp>
#include <pfs/byte_string.hpp>
#include <pfs/string.hpp>
#include <pfs/memory.hpp>

namespace pfs {
namespace io {

enum open_mode_enum
{
      not_open     = 0                       /**< Device is not opened */
    , read_only    = 0x0001                  /**< Open device for read only */
    , write_only   = 0x0002                  /**< Open device for write only */
    , read_write   = read_only | write_only  /**< Open device for read and write */
    , write_read   = read_write              /**< Synonym for read_write */
    , non_blocking = 0x0004                  /**< Open device in non-blocking mode */
    , truncate     = 0x0010                  /**< Create device (only for regular file device) */
};

enum device_type
{
      device_unknown = 0
    , device_null
    , device_buffer
    , device_stream
    , device_file
    , device_tcp_socket
    , device_tcp_peer
    , device_udp_socket
    , device_udp_peer
    , device_local_socket
    , device_local_peer
};

}} // pfs::io

namespace pfs {
namespace io {
namespace details {

// TODO Support other platform specific native file handle types.
//
typedef int native_handle_type;

class basic_device
{
public:
    typedef pfs::string string_type;

protected:
    shared_ptr<void> _ctx;

public:
    basic_device ()
        : _ctx(0)
    {}

    virtual ~basic_device () {}

    template <typename T>
    T * context ()
    {
        return static_cast<T *>(_ctx.get());
    }

    template <typename T>
    T const * context () const
    {
        return static_cast<T const *>(_ctx.get());
    }

    template <typename T>
    void set_context (T * context)
    {
        _ctx = pfs::static_pointer_cast<void>(shared_ptr<T>(context));
    }

    template <typename T, typename Deleter>
    void set_context (T * context, Deleter d)
    {
        _ctx = pfs::static_pointer_cast<void>(shared_ptr<T>(context, d));
    }

    virtual string_type url () const = 0;

    virtual native_handle_type native_handle () const = 0;

    virtual bool is_server () const
    {
        return false;
    }
};

class device : public basic_device
{
public:
    typedef details::native_handle_type  native_handle_type;
    typedef uint32_t                  open_mode_flags;
    typedef open_mode_enum            open_mode_type;
    typedef basic_device::string_type string_type;

public:
    device ()
        : basic_device()
    {}

    virtual ~device () {}

    virtual error_code reopen () = 0;

    virtual open_mode_flags open_mode () const = 0;

    virtual ssize_t available () const = 0;

    bool at_end () const
    {
        return this->available() == ssize_t(0);
    }

    /**
     * @note Implementation must read no more than @a n and return immediately.
     */
    virtual ssize_t read (byte_t * bytes, size_t n, error_code & ec) pfs_noexcept = 0;

    ssize_t read (byte_t * bytes, size_t n)
    {
        error_code ec;
        ssize_t r = read(bytes, n, ec);
        if (r < 0)
            PFS_THROW(io_exception(ec));
        return r;
    }

    ssize_t read (char * chars, size_t n, error_code & ec) pfs_noexcept
    {
        return this->read(reinterpret_cast<byte_t *>(chars), n, ec);
    }

    ssize_t read (char * chars, size_t n)
    {
        return this->read(reinterpret_cast<byte_t *>(chars), n);
    }

    ssize_t read (byte_string & bytes, size_t n, error_code & ec) pfs_noexcept
    {
        if (n == 0)
            return 0;

        size_t oldsize = bytes.size();

        bytes.resize(oldsize + n);

        ssize_t r = read(bytes.data() + oldsize, n, ec);

        if (r < 0) {
            // Restore previous state of buffer
            bytes.resize(oldsize);
            return r;
        }

        if (r > 0 && n != integral_cast_check<size_t>(r))
            bytes.resize(oldsize + r);

        return r;
    }

    ssize_t read (byte_string & bytes, size_t n)
    {
        error_code ec;
        ssize_t r = read(bytes, n, ec);
        if (r < 0)
            PFS_THROW(io_exception(ec));
        return r;
    }

    /**
     * @brief Read data from device and appends them
     */
    ssize_t read (byte_string & bytes, error_code & ec) pfs_noexcept
    {
        return this->read(bytes, available(), ec);
    }

    ssize_t read (byte_string & bytes)
    {
        return this->read(bytes, available());
    }

    /**
     * @param millis Timout in milliseconds for wait for reading @arg n bytes ().
     *               A negative value in timeout means an infinite timeout.
     *               Zero value means the behaviour as @c read method.
     */
    virtual ssize_t read_wait (byte_t * bytes, size_t n, error_code & ec, int millis) pfs_noexcept;

    ssize_t read_wait (byte_t * bytes, size_t n, int millis)
    {
        error_code ec;
        ssize_t r = read_wait(bytes, n, ec, millis);
        if (r < 0)
            PFS_THROW(io_exception(ec));
        return r;
    }

    inline ssize_t read_wait (char * chars, size_t n, error_code & ec, int millis) pfs_noexcept
    {
        return this->read_wait(reinterpret_cast<byte_t *>(chars), n, ec, millis);
    }

    inline ssize_t read_wait (char * chars, size_t n, int millis)
    {
        return this->read_wait(reinterpret_cast<byte_t *>(chars), n, millis);
    }

    /**
     * @brief Read @a n bytes from device.
     *
     * @param millis Timout in milliseconds for wait for reading @arg n bytes.
     *               A negative value in timeout means an infinite timeout.
     *               Zero value means the behaviour as @c read method.
     * @return The number of bytes received, or -1 if an error occurred.
     */
    ssize_t read_wait (byte_string & bytes, size_t n, error_code & ec, int millis) pfs_noexcept;

    ssize_t read_wait (byte_string & bytes, size_t n, int millis)
    {
        error_code ec;
        ssize_t r = read_wait(bytes, n, ec, millis);
        if (r < 0)
            PFS_THROW(io_exception(ec));
        return r;
    }

    /**
     * @brief Read data from device and appends them
     */
    ssize_t read_wait (byte_string & bytes, error_code & ec, int millis) pfs_noexcept
    {
        return this->read_wait(bytes, available(), ec, millis);
    }

    ssize_t read_wait (byte_string & bytes, int millis)
    {
        return this->read_wait(bytes, available(), millis);
    }

    virtual ssize_t write (byte_t const * bytes, size_t n, error_code & ec) pfs_noexcept = 0;

    ssize_t write (byte_t const * bytes, size_t n)
    {
        error_code ec;
        ssize_t r = write(bytes, n, ec);
        if (r < 0)
            PFS_THROW(io_exception(ec));
        return r;
    }

    ssize_t write (const char * chars, size_t n, error_code & ec) pfs_noexcept
    {
        return this->write(reinterpret_cast<const byte_t *>(chars), n, ec);
    }

    ssize_t write (const char * chars, size_t n)
    {
        return this->write(reinterpret_cast<const byte_t *>(chars), n);
    }

    ssize_t write (byte_string const & bytes, size_t n, error_code & ec) pfs_noexcept
    {
        return this->write(bytes.data(), pfs::min(n, bytes.size()), ec);
    }

    ssize_t write (byte_string const & bytes, size_t n)
    {
        return this->write(bytes.data(), pfs::min(n, bytes.size()));
    }

    ssize_t write (byte_string const & bytes, error_code & ec) pfs_noexcept
    {
        return this->write(bytes.data(), bytes.size(), ec);
    }

    ssize_t write (byte_string const & bytes)
    {
        return this->write(bytes.data(), bytes.size());
    }

    virtual error_code close () = 0;

    virtual bool opened () const = 0;

    virtual void flush () = 0;

    virtual bool set_nonblocking (bool on) = 0;

    virtual bool is_nonblocking () const = 0;

    bool is_readable () const
    {
        return this->open_mode() | read_only;
    }

    bool is_writable () const
    {
        return this->open_mode() | write_only;
    }

    //virtual native_handle_type native_handle () const = 0;

    virtual device_type type () const = 0;
};

}}} // pfs::io::details
