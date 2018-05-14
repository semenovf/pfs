#pragma once
#include <cstring>
#include <sys/un.h>
#include "pfs/io/local_socket.hpp"
#include "posix_utils.hpp"

namespace pfs {
namespace io {
namespace details {

inline string local_socket_url (char const * proto, sockaddr_un const & sun)
{
#if PFS_CC_MSC
#   error "Implement local_socket_url() for MSC"
#else
    string r(proto);
    r.append("://");
    r.append(sun.sun_path);
    return r;
#endif
}

class local_socket : public bits::device
{
public:
    typedef bits::device::native_handle_type native_handle_type;
    typedef bits::device::string_type        string_type;

protected:
    native_handle_type _fd;
    sockaddr_un  _sockaddr;

private:
    local_socket (local_socket const & other);
    local_socket & operator = (local_socket const & other);

public:
    virtual error_code open (bool non_blocking)
    {
        _fd = create_local_socket(non_blocking);
        return _fd < 0
                ? error_code(errno, pfs::generic_category())
                : error_code();
    }

public:
    local_socket ()
        : bits::device()
        , _fd(-1)
    {}

    virtual ~local_socket ()
    {
        close();
    }

    error_code connect (filesystem::path const & filename);

    virtual bool reopen () pfs_override;

    virtual open_mode_flags open_mode () const pfs_override;

    virtual ssize_t available () const pfs_override;

    virtual ssize_t read (byte_t * bytes, size_t n) pfs_override;

    virtual ssize_t write (const byte_t * bytes, size_t n) pfs_override;

    virtual bool close () pfs_override
    {
        bool r = true;
        if (close_socket(_fd) != 0) {
            this->_ec = error_code(errno, pfs::generic_category());
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

    virtual device_type type () const pfs_override
    {
        return device_local_socket;
    }

    virtual string_type url () const pfs_override
    {
        return local_socket_url("local", _sockaddr);
    }

    error_code set_socket_options (uint32_t sso);
};

class local_socket_peer : public local_socket
{
public:
    typedef local_socket::native_handle_type native_handle_type;

public:
    local_socket_peer (native_handle_type fd, sockaddr_un const & sockaddr)
        : local_socket()
    {
        _fd = fd;
        ::memcpy(& _sockaddr, & sockaddr, sizeof(_sockaddr));
    }

    virtual device_type type () const pfs_override
    {
        return device_local_peer;
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

