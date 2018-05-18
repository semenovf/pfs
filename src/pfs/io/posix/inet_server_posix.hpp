#ifndef __PFS_INET_SERVER_POSIX_HPP__
#define __PFS_INET_SERVER_POSIX_HPP__

#include "pfs/io/inet_server.hpp"
#include "posix_utils.hpp"

namespace pfs {
namespace io {
namespace details {

class inet_server : public details::server
{
public:
    typedef details::server::native_handle_type native_handle_type;

protected:
	native_handle_type _fd;
	sockaddr_in  _sockaddr;

public:
    inet_server ()
        : details::server()
        , _fd(-1)
    {}

  	virtual ~inet_server ()
	{
		close();
	}

    virtual bool close () pfs_override
    {
        bool r = true;
        if (pfs::io::close_socket(_fd) != 0) {
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

public:
    error_code bind (uint32_t addr, uint16_t port);
};

class tcp_server : public inet_server
{
public:
	typedef inet_server::native_handle_type native_handle_type;

public:
    error_code open (bool non_blocking)
    {
        _fd = pfs::io::create_tcp_socket(non_blocking);
        return _fd < 0
                ? get_last_system_error()
                : error_code();
    }

    error_code listen (int backlog)
    {
        return (::listen(_fd, backlog) != 0)
                ? get_last_system_error()
                : error_code();
    }

public:
    tcp_server ()
        : inet_server()
    {}

    virtual server_type type () const pfs_override
    {
    	return server_tcp;
    }

    virtual string url () const pfs_override
    {
        return pfs::io::inet_socket_url("tcp", _sockaddr);
    }

    virtual details::device * accept (bool non_blocking, error_code & ec) pfs_override;
};

class udp_server : public inet_server
{
public:
    typedef inet_server::native_handle_type native_handle_type;

public:
    error_code open (bool non_blocking)
    {
        _fd = pfs::io::create_udp_socket(non_blocking);
        return _fd < 0
                ? get_last_system_error()
                : error_code();
    }

    error_code listen (int/* backlog */)
    {
        return error_code();
    }

public:
    udp_server ()
        : inet_server()
    {}

    virtual server_type type () const pfs_override
    {
        return server_udp;
    }

    virtual string url () const pfs_override
    {
        return pfs::io::inet_socket_url("udp", _sockaddr);
    }

    virtual details::device * accept (bool non_blocking, error_code & ec) pfs_override;
};


}}}

#endif /* __PFS_INET_SERVER_POSIX_HPP__ */

