#pragma once
#include <pfs/types.hpp>
#include <pfs/io/exception.hpp>
#include <pfs/string.hpp>

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
namespace bits {

// TODO Support other platform specific native file handle types.
//
typedef int native_handle_type;

struct device_context
{
    virtual ~device_context () {}
};

class basic_device
{
public:
    typedef device_context context_type;
    typedef pfs::string    string_type;

protected:
    context_type * _ctx;
    error_code     _ec;

public:
    basic_device ()
        : _ctx(0)
    {}

    virtual ~basic_device ()
    {
        if (_ctx)
            delete _ctx;
    }

    context_type * context ()
    {
        return _ctx;
    }

    context_type const * context () const
    {
        return _ctx;
    }

    void set_context (context_type * context)
    {
        if (_ctx)
            delete _ctx;
        _ctx = context;
    }

    error_code errorcode () const
    {
        return _ec;
    }

    bool is_error () const
    {
        return _ec != error_code();
    }

    virtual string_type url () const = 0;
};

class device : public basic_device
{
public:
    typedef bits::native_handle_type  native_handle_type;
    typedef uint32_t                  open_mode_flags;
    typedef open_mode_enum            open_mode_type;
    typedef device_context            info_type;
    typedef basic_device::string_type string_type;

public:
    device ()
        : basic_device()
    {}

    virtual ~device () {}

    virtual bool reopen () = 0;

    virtual open_mode_flags open_mode () const = 0;

    virtual ssize_t bytes_available () const = 0;

    virtual ssize_t read (byte_t * bytes, size_t n) = 0;

    virtual ssize_t write (const byte_t * bytes, size_t n) = 0;

    virtual bool close () = 0;

    virtual bool opened () const = 0;

    virtual void flush () = 0;

    virtual bool set_nonblocking (bool on) = 0;

    virtual bool is_nonblocking () const = 0;

    virtual native_handle_type native_handle () const = 0;

    virtual device_type type () const = 0;
};

}}} // pfs::io::bits
