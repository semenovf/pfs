#pragma once
#include <pfs/cxxlang.hpp>
#include <pfs/io/exception.hpp>
#include <pfs/io/bits/device.hpp>

namespace pfs {
namespace io {

enum server_type
{
      server_unknown = 0
    , server_tcp
    , server_udp
};

}} // pfs::io

namespace pfs {
namespace io {
namespace details {

struct server : public basic_device
{
    typedef device::native_handle_type native_handle_type;
    typedef device::string_type        string_type;

    server ()
        : basic_device()
    {}

    virtual ~server () {}

    virtual bool opened () const = 0;

    virtual error_code close () = 0;

    virtual bool set_nonblocking (bool on) = 0;

    virtual bool is_nonblocking () const = 0;

    virtual details::device * accept (bool non_blocking, error_code & ec) = 0;

    //virtual native_handle_type native_handle () const = 0;

    virtual server_type type () const = 0;

    virtual bool is_server () const pfs_override
    {
        return true;
    }
};

}}} // pfs::io::details
