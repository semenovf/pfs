#include "pfs/net/inet_addrinfo.hpp"

namespace pfs {
namespace net {

#if __COMMENT__
inet_addrinfo::inet_addrinfo ()
    : _p(0)
{
    addrinfo_iterator it;

    const char * node = hostname.c_str();
    const char * service = nullptr; // service is no matter
    struct addrinfo hints;
    struct addrinfo * result;

    memset(& hints, 0, sizeof(struct addrinfo));
    hints.ai_flags     = (hostname.isEmpty() ? AI_PASSIVE : 0) | AI_CANONNAME;
    hints.ai_family    = AF_UNSPEC;    /* Allow IPv4 or IPv6, i.e. any address family */
    hints.ai_socktype  = 0;            /* Any type */
    hints.ai_protocol  = 0;            /* Any protocol */
    hints.ai_canonname = nullptr;
    hints.ai_addr      = nullptr;
    hints.ai_next      = nullptr;

    int rc = getaddrinfo(node, service, & hints, & result);
    if (!PFS_VERIFY_X(rc == 0
            , (pfs::string() << hostname
              << _u8(": get address info failure: ")
              << gai_strerror(rc)).c_str())) {
        return end();
    }

    pfs::shared_ptr<addrinfo_iterator_data> d(new addrinfo_iterator_data);
    d->result = result;
    d->next = result;
    it._data.swap(d);

    return it;

}

inet_addrinfo::~inet_addrinfo ()
{
    if (_p)
        freeaddrinfo(_p);
}

#endif

}} // pfs::net
