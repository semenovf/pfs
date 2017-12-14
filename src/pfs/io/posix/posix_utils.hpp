#ifndef __PFS_IO_INET_POSIX_UTILS_HPP__
#define __PFS_IO_INET_POSIX_UTILS_HPP__

#include <netinet/in.h>
#include <pfs/compiler.hpp>
#include <pfs/lexical_cast.hpp>
#include <pfs/string.hpp>

#if PFS_CC_GCC
#   include <arpa/inet.h>
#endif

namespace pfs {
namespace io {

bool is_nonblocking (int fd);
bool set_nonblocking (int fd, bool on);

template <typename StringType>
StringType inet_socket_url (char const * proto, sockaddr_in const & sin)
{
#if PFS_CC_MSC
#   error "Implement inet_socket_url() for MSC"
#else
    char str[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, & sin.sin_addr, str, INET_ADDRSTRLEN);
    StringType r(proto);
    r.append("://");
    r.append(str);
    r.append(":");
    r.append(to_string<StringType>(ntohs(sin.sin_port), 10));
    return r;
#endif
}

int create_tcp_socket (bool non_blocking);
int create_udp_socket (bool non_blocking);
int close_socket (int fd);

}}

#endif /* __PFS_IO_INET_POSIX_UTILS_HPP__ */

