#include <fcntl.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "posix_utils.hpp"

namespace pfs {
namespace io {

bool is_nonblocking (int fd)
{
    int r = fcntl(fd, F_GETFL, 0);
    PFS_ASSERT(r >= 0);
    return (r & O_NONBLOCK);
}

bool set_nonblocking (int fd, bool on)
{
    int flags = ::fcntl(fd, F_GETFL, 0);

    if (on)
    	flags |= O_NONBLOCK;
    else
    	flags &= ~O_NONBLOCK;

    return ::fcntl(fd, F_SETFL, flags) >= 0;
}

int create_tcp_socket (bool non_blocking)
{
  	int socktype = SOCK_STREAM;

	if (non_blocking)
		socktype |= SOCK_NONBLOCK;

	return ::socket(PF_INET, socktype, IPPROTO_TCP);
}

int create_udp_socket (bool non_blocking_flag)
{
	int socktype = SOCK_DGRAM;

	if (non_blocking_flag)
		socktype |= SOCK_NONBLOCK;

	return ::socket(PF_INET, socktype, IPPROTO_UDP);
}

int close_socket (int fd)
{
    if (fd > 0) {
    	shutdown(fd, SHUT_RDWR);

        return ::close(fd);
    }

    return 0;
}

}} // pfs::io
