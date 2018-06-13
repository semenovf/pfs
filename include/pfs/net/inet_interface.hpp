#pragma once
#include <pfs/config.h>

#if HAVE_IF_NAMEINDEX
#   include <net/if.h> // if_nameindex(), if_freenameindex()
#endif

#if HAVE_GETIFADDRS
#   include <arpa/inet.h>
#   include <ifaddrs.h>
#endif

#include <pfs/iterator.hpp>
#include <pfs/stringlist.hpp>
#include <pfs/system_error.hpp>
#include <pfs/net/inet4_addr.hpp>

namespace pfs {
namespace net {

class inet_interface
{
public: // static
    static bool names (stringlist<string> & sl, error_code & ec);

    template <template <typename> class Container>
    static bool inet4_addrs (Container<inet4_addr> & c, error_code & ec);
};

bool inet_interface::names (stringlist<string> & sl, error_code & ec)
{
    typedef string string_type;

#if HAVE_IF_NAMEINDEX
    struct if_nameindex * first = if_nameindex();
    struct if_nameindex * p = first;

    if (! p) {
        ec = get_last_system_error();
        return false;
    }

    for (; ! (p->if_index == 0 && p->if_name == 0); p++)
        sl.push_back(string_type(p->if_name));

    if_freenameindex(first);
    return true;
#else
    #error "*** Can't implement pfs::net::inet_interface::names() ***"
#endif
}

template <template <typename> class Container>
bool inet_interface::inet4_addrs (Container<inet4_addr> & c, error_code & ec)
{
    typedef Container<inet4_addr> container_type;

    pfs::back_insert_iterator<container_type> inserter(c);

#if HAVE_GETIFADDRS
    struct ifaddrs * first;

    if (getifaddrs(& first)) {
        ec = get_last_system_error();
        return false;
    }

    for (struct ifaddrs * p = first; p != 0; p = p->ifa_next) {
        if (p->ifa_addr == 0)
            continue;

        int family = p->ifa_addr->sa_family;

        if (family != AF_INET)
            continue;

        struct sockaddr_in * saddr = reinterpret_cast<struct sockaddr_in *>(p->ifa_addr);

        inet4_addr a = ntohl(saddr->sin_addr.s_addr);

        inserter++ = a;
    }

    freeifaddrs(first);
    return true;

#else
    #error "*** Can't implement pfs::net::inet_interface::inet4_addrs() ***"
#endif
}

}} // pfs::net
