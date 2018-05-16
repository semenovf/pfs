#pragma once
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>
#include "pfs/io/inet_socket.hpp"
#include "posix_utils.hpp"

namespace pfs {
namespace io {
namespace details {

class inet_socket : public details::device
{
public:
    typedef details::device::native_handle_type native_handle_type;
    typedef details::device::string_type        string_type;

protected:
    native_handle_type _fd;
    sockaddr_in  _sockaddr;

private:
    inet_socket (const inet_socket & other);
    inet_socket & operator = (const inet_socket & other);

public:
    virtual error_code open (bool non_blocking) = 0;

public:
    inet_socket ()
        : details::device()
        , _fd(-1)
    {}

    virtual ~inet_socket ()
    {
        close();
    }

    error_code connect (uint32_t addr, uint16_t port);

    virtual bool reopen () pfs_override;

    virtual open_mode_flags open_mode () const pfs_override;

    virtual ssize_t available () const pfs_override;

    virtual ssize_t read (byte_t * bytes, size_t n) pfs_override;

    virtual ssize_t write (const byte_t * bytes, size_t n) pfs_override;

    virtual bool close () pfs_override
    {
        bool r = true;
        if (close_socket(_fd) != 0) {
            this->_ec = get_last_system_error();
            r = false;
        }
        _fd = -1;
        return r;
    }

    virtual bool opened () const pfs_override
    {
        return _fd >= 0;
    }

    virtual void flush () pfs_override
    {}

    virtual bool set_nonblocking (bool on) pfs_override
    {
        return pfs::io::set_nonblocking(_fd, on);
    }

    virtual bool is_nonblocking () const pfs_override
    {
        return pfs::io::is_nonblocking(_fd);
    }

    virtual native_handle_type native_handle () const pfs_override
    {
        return _fd;
    }

    virtual device_type type () const = 0;

    virtual string_type url () const = 0;

    error_code set_socket_options (uint32_t sso);
};

class tcp_socket : public inet_socket
{
public:
    typedef inet_socket::native_handle_type native_handle_type;
    typedef inet_socket::string_type        string_type;

public:
    virtual error_code open (bool non_blocking) pfs_override
    {
        _fd = create_tcp_socket(non_blocking);
        return _fd < 0
                ? error_code(errno, pfs::generic_category())
                : error_code();
    }

public:
    tcp_socket ()
        : inet_socket()
    {}

    virtual device_type type () const pfs_override
    {
        return device_tcp_socket;
    }

    virtual string_type url () const pfs_override
    {
        return inet_socket_url("tcp", _sockaddr);
    }
};

class tcp_socket_peer : public tcp_socket
{
public:
    typedef tcp_socket::native_handle_type native_handle_type;

public:
    tcp_socket_peer (native_handle_type fd, const sockaddr_in & sockaddr)
        : tcp_socket()
    {
        _fd = fd;
        ::memcpy(& _sockaddr, & sockaddr, sizeof(_sockaddr));
    }

    virtual device_type type () const pfs_override
    {
        return device_tcp_peer;
    }
};

class udp_socket : public inet_socket
{
public:
    typedef inet_socket::native_handle_type native_handle_type;
    typedef inet_socket::string_type        string_type;

public:
    virtual error_code open (bool non_blocking) pfs_override
    {
        _fd = create_udp_socket(non_blocking);
        return _fd < 0
                ? error_code(errno, pfs::generic_category())
                : error_code();
    }

public:
    udp_socket ()
        : inet_socket()
    {}

    virtual device_type type () const pfs_override
    {
        return device_udp_socket;
    }

    virtual string_type url () const pfs_override
    {
        return inet_socket_url("udp", _sockaddr);
    }
};

class udp_socket_peer : public udp_socket
{
public:
    typedef udp_socket::native_handle_type native_handle_type;

public:
    udp_socket_peer (native_handle_type fd, const sockaddr_in & sockaddr)
        : udp_socket()
    {
        _fd = fd;
        ::memcpy(& _sockaddr, & sockaddr, sizeof(_sockaddr));
    }

    virtual ~udp_socket_peer ()
    {
        close();
    }

    virtual device_type type () const pfs_override
    {
        return device_udp_peer;
    }

    // Reimplemented to avoid descriptor closing
    //
    virtual bool close () pfs_override
    {
        // Really descriptor cannot be closed,
        // it still used by server
        //
        _fd = -1;
        return true;
    }
};

}}}
