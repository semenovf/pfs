/*
 * tcp_socket_posix.cpp
 *
 *  Created on: Jan 11, 2016
 *      Author: wladt
 *
 * Sources:
 * ---
 * Multicast over TCP/IP HOWTO
 *      http://tldp.org/HOWTO/Multicast-HOWTO.html
 * Socket programming:
 *      http://publib.boulder.ibm.com/infocenter/iseries/v5r3/topic/rzab6/rzab6soxoverview.htm
 * Socket application design recommendations:
 *      http://publib.boulder.ibm.com/infocenter/iseries/v5r3/topic/rzab6/rzab6designrec.htm
 * A connection-oriented server example:
 *      http://publib.boulder.ibm.com/infocenter/iseries/v5r3/topic/rzab6/rzab6xconoserver.htm
 * A connectionless server example:
 *      http://publib.boulder.ibm.com/infocenter/iseries/v5r3/topic/rzab6/rzab6xconlessserver.htm
 * A connection-oriented client example:
 *      http://publib.boulder.ibm.com/infocenter/iseries/v5r3/topic/rzab6/rzab6xconoclient.htm
 * A connectionless client example:
 *      http://publib.boulder.ibm.com/infocenter/iseries/v5r3/topic/rzab6/rzab6xconlessclient.htm
 * Introduction to non-blocking I/O:
 *      http://www.kegel.com/dkftpbench/nonblocking.html
 */

#include <cerrno>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "inet_socket_posix.hpp"

namespace pfs {
namespace io {
namespace details {

error_code inet_socket::connect (uint32_t addr, uint16_t port)
{
	memset(& _sockaddr, 0, sizeof(_sockaddr));

	_sockaddr.sin_family      = PF_INET;
	_sockaddr.sin_port        = htons(port);
	_sockaddr.sin_addr.s_addr = htonl(addr);

	int rc = ::connect(_fd
			, reinterpret_cast<struct sockaddr *>(& _sockaddr)
			, sizeof(_sockaddr));

	error_code ex;

    if (rc < 0) {
        switch (errno) {
        case EISCONN:
//        	sock._state = bits::connected_state;
            break;

        case ECONNREFUSED:
        case EINVAL:
            ex = make_error_code(io_errc::connection_refused);
//            sock._state = bits::unconnected_state;
            break;

        case ETIMEDOUT:
        	ex = error_code(errno, pfs::generic_category());
            break;

        case EHOSTUNREACH:
        	ex = error_code(errno, pfs::generic_category());
//        	sock._state = bits::unconnected_state;
            break;

        case ENETUNREACH:
        	ex = error_code(errno, pfs::generic_category());
//        	sock._state = bits::unconnected_state;
            break;

        case EADDRINUSE:
        	ex = error_code(errno, pfs::generic_category());
            break;

		/* TODO
			errno == EINPROGRESS:
			The socket is nonblocking and the connection cannot be completed immediately.
			It is possible to select(2) or poll(2) for completion by selecting
			the socket for writing. After select(2) indicates writability,
			use getsockopt(2) to read the SO_ERROR option at level SOL_SOCKET
			to determine whether connect() completed successfully (SO_ERROR is zero) or
			unsuccessfully (SO_ERROR is one of the usual error codes listed here,
			explaining the reason for the failure).
		 */
        case EINPROGRESS:
        case EALREADY:
        	ex = error_code(errno, pfs::generic_category());
//        	sock._state = bits::connecting_state;
            break;

        case EAGAIN:
        	ex = error_code(errno, pfs::generic_category());
            break;

        case EACCES:
        case EPERM:
            ex = error_code(errno, pfs::generic_category());
//            sock._state = bits::unconnected_state;
            break;

        case EAFNOSUPPORT:
        case EBADF:
        case EFAULT:
        case ENOTSOCK:
//        	sock._state = bits::unconnected_state;
        	break;

        default:
            break;
        }
    }

    return ex;
}

// FIXME Return in non-blocking mode (need to change this behavior)
//
error_code inet_socket::reopen ()
{
	close();
	error_code ex = open(true);
    
    // Need conversion ntoh() because connect() will convert again (see connect() above)
    //
	if (!ex) ex = connect(ntohl(_sockaddr.sin_addr.s_addr), ntohs(_sockaddr.sin_port));
	return ex;
}

bits::device::open_mode_flags inet_socket::open_mode () const
{
	// TODO Inherited from file (check if this apply to real socket)

	bits::device::open_mode_flags r = 0;
	char buf[1] = { 0 };

	if (::read(_fd, buf, 0) >= 0 && errno != EBADF)
		r |= read_only;

	if (::write(_fd, buf, 0) >= 0 && errno != EBADF)
		r |= write_only;

	return r;
}

size_t inet_socket::bytes_available () const
{
	PFS_ASSERT(_fd >= 0 );
	int n = 0;
	int rc = 0;
	rc = ioctl(_fd, FIONREAD, & n);

	PFS_ASSERT_X(rc == 0, error_code(errno, pfs::generic_category()).message().c_str());
	PFS_ASSERT(n >= 0);

	return static_cast<size_t>(n);
}

error_code inet_socket::set_socket_options (uint32_t sso)
{
	if (sso) {
		int optval = 1;
		socklen_t optlen = sizeof(optval);

		if (sso & sso_keep_alive) {
			int r = setsockopt(_fd, SOL_SOCKET, SO_KEEPALIVE, & optval, optlen);
			if (r < 0)
				return error_code(errno, pfs::generic_category());
		}
	}

	return error_code();
}

ssize_t inet_socket::read (byte_t * bytes, size_t n, error_code * pex)
{
	PFS_ASSERT(_fd >= 0 );

	ssize_t r = 0;

//	do {
		r = recv(_fd, bytes, n, 0);

		if (r < 0 && (errno == EAGAIN || errno == EWOULDBLOCK)) {
			r = 0;
//			continue;
		}

		if (r < 0) {
			if (pex)
				*pex = error_code(errno, pfs::generic_category());
		}

//		break;
//	} while (true);

	return r;
}

ssize_t inet_socket::write (const byte_t * bytes, size_t nbytes, error_code * ex)
{
	PFS_ASSERT(_fd >= 0);

	int r = 0; // total sent

	while (nbytes) {
		// MSG_NOSIGNAL flag means:
		// requests not to send SIGPIPE on errors on stream oriented sockets
		// when the other end breaks the connection.
		// The EPIPE error is still returned.
		//
		ssize_t n = send(_fd, bytes + r, nbytes, MSG_NOSIGNAL);

		if (n < 0) {
			if (errno == EAGAIN || errno == EWOULDBLOCK)
				continue;

			r = -1;
			break;
		}

		r += n;
		nbytes -= n;
	}

	if (r < 0) {
		if (ex)
			*ex = error_code(errno, pfs::generic_category());
	}

	return r;
}

}}} // pfs::io::details

namespace pfs {
namespace io {

template <>
device open_device<tcp_socket> (const open_params<tcp_socket> & op, error_code & ec)
{
    bool non_blocking_flag = op.oflags & pfs::io::non_blocking;

    details::tcp_socket * d = new details::tcp_socket;

    ec = d->open(non_blocking_flag);

    if (!ec)
        ec = d->set_socket_options(op.socketopts);

    if (!ec)
        ec = d->connect(op.addr.native(), op.port);

    if (ec && ec != make_error_code(io_errc::operation_in_progress)) {
    	delete d;
    	return device();
    }

    return device(d);
}

template <>
device open_device<udp_socket> (const open_params<udp_socket> & op, error_code & ec)
{
    bool non_blocking_flag = op.oflags & pfs::io::non_blocking;

    details::udp_socket * d = new details::udp_socket;

    ec = d->open(non_blocking_flag);

    if (!ec)
        ec = d->set_socket_options(op.socketopts);

    if (!ec)
        ec = d->connect(op.addr.native(), op.port);

    if (ec && ec != make_error_code(io_errc::operation_in_progress)) {
    	delete d;
    	return device();
    }

    return device(d);
}

}} // pfs::io
