#include <cerrno>
#include <sys/fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "inet_server_posix.hpp"
#include "inet_socket_posix.hpp"

namespace pfs {
namespace io {
namespace details {

error_code inet_server::bind (uint32_t addr, uint16_t port)
{
    memset(& _sockaddr, 0, sizeof (_sockaddr));

    _sockaddr.sin_family = PF_INET;
    _sockaddr.sin_port = htons(port);
    _sockaddr.sin_addr.s_addr = htonl(addr);

    int yes = 1;

    /* http://publib.boulder.ibm.com/infocenter/iseries/v5r3/topic/rzab6/rzab6xconoserver.htm
     *
     * The setsockopt() function is used to allow the local address to
     * be reused when the server is restarted before the required wait
     * time expires
     */
    int rc = ::setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR, & yes, sizeof (int));

    if (rc != 0)
        return get_last_system_error();

    rc = ::bind(_fd
            , reinterpret_cast<struct sockaddr *> (& _sockaddr)
            , sizeof (_sockaddr));

    if (rc != 0)
        return get_last_system_error();

    return error_code();
}

details::device * tcp_server::accept (bool non_blocking, error_code & ec)
{
    struct sockaddr_in peer_addr;
    socklen_t peer_addr_len = sizeof (peer_addr);

    int peer_sock = ::accept(_fd
            , reinterpret_cast<struct sockaddr *> (& peer_addr)
            , & peer_addr_len);

    PFS_ASSERT(sizeof (sockaddr_in) == peer_addr_len);

    if (peer_sock < 0) {
        ec = get_last_system_error();
        return 0;
    }

    details::tcp_socket_peer * peer = new details::tcp_socket_peer(peer_sock, peer_addr);

    PFS_ASSERT(peer->set_nonblocking(non_blocking));

    return static_cast<details::device *> (peer);
}

details::device * udp_server::accept (bool non_blocking, error_code & ec)
{
    struct sockaddr_in peer_addr;
    socklen_t peer_addr_len = sizeof (peer_addr);

    char buf[1];
    ssize_t n = recvfrom(_fd, buf, 0, MSG_PEEK
            , reinterpret_cast<sockaddr *> (& peer_addr), & peer_addr_len);

    if (n < 0) {
        ec = get_last_system_error();
        return 0;
    }

    details::udp_socket_peer * peer = new details::udp_socket_peer(_fd, peer_addr);

    PFS_ASSERT(peer->set_nonblocking(non_blocking));

    return static_cast<details::device *> (peer);
}

}}} // namespace

namespace pfs {
namespace io {

template <>
server_ptr open_server<tcp_server> (open_params<tcp_server> const & op, error_code & ec)
{
    bool non_blocking_flag = op.oflags & pfs::io::non_blocking;

    details::tcp_server * d = new details::tcp_server;

    ec = d->open(non_blocking_flag);

    if (!ec) ec = d->bind(op.addr.native(), op.port);
    if (!ec) ec = d->listen(op.npendingconn);

    if (ec) {
        delete d;
        return server_ptr();
    }

    return server_ptr(d);
}

template <>
server_ptr open_server<udp_server> (const open_params<udp_server> & op, error_code & ec)
{
    bool non_blocking_flag = op.oflags & pfs::io::non_blocking;

    details::udp_server * d = new details::udp_server;

    ec = d->open(non_blocking_flag);

    if (!ec) ec = d->bind(op.addr.native(), op.port);

    if (ec) {
        delete d;
        return server_ptr();
    }

    return server_ptr(d);
}

}} // pfs::io
