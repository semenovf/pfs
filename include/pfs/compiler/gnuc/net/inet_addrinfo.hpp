#ifndef __PFS_COMPILER_GNUC_NET_INET_ADDRINFO_HPP__
#define __PFS_COMPILER_GNUC_NET_INET_ADDRINFO_HPP__

#include <sys/types.h>
#include <sys/socket.h>
//#include <netdb.h>  // getaddrinfo(), freeaddrinfo()
#include <pfs/iterator.hpp>

namespace pfs {
namespace net {


#if __COMMENT__

class inet_addrinfo
{
    struct addrinfo * _p;

public:
    inet_addrinfo ();
    ~inet_addrinfo ();
};

struct inet_addrinfo_iterator : public pfs::iterator_facade<
          pfs::forward_iterator_tag
        , forward_iterator
        , C, C *, C &>
{
    C * _p;

    inet_addrinfo_iterator (C * p)
        : _p(p)
    {}

    static reference ref (inet_addrinfo_iterator & it)
    {
        return *it._p;
    }

    static pointer ptr (inet_addrinfo_iterator & it)
    {
        return it._p;
    }

    static void increment (inet_addrinfo_iterator & it, difference_type)
    {
        ++it._p;
    }

    static bool equals (inet_addrinfo_iterator const & it1, inet_addrinfo_iterator const & it2)
    {
        return it1._p == it2._p;
    }

    /* Official name of the host */
    template <typename StringType>
    StringType canonical_name () const
    {
        // Only field of the first of the addrinfo structures in the returned list
        // is set to point to the official name of the host.
        if (_data->result)
            return StringType(_data->result->ai_canonname);
        return StringType();
    }

    template <typename StringType>
    StringType hostname () const
    {
        return canonical_name();
    }

    uint32_t ip4addr () const
    {
        if (_data->next) {
            struct sockaddr_in * saddr = reinterpret_cast<struct sockaddr_in *>(_data->next->ai_addr);
            return ntohl(saddr->sin_addr.s_addr);
        }
        return 0;
    }

    uint16_t port () const
    {
        return _data->next
                ? reinterpret_cast<struct sockaddr_in *>(_data->next->ai_addr)->sin_port
                : 0;
    }

};


class addrinfo_iterator
{
    pfs::shared_ptr<addrinfo_iterator_data> _data;

public:
    addrinfo_iterator () : _data(new addrinfo_iterator_data) {}
    addrinfo_iterator (const addrinfo_iterator & it) : _data(it._data) {}

    addrinfo_iterator & operator = (const addrinfo_iterator & it)
    {
        _data = it._data;
        return *this;
    }

    bool operator == (const addrinfo_iterator & it)
    {
        return _data->next == it._data->next;
    }

    bool operator != (const addrinfo_iterator & it) { return !(*this == it); }
    addrinfo_iterator & operator ++ ()
    {
        _data->next = _data->next->ai_next;
        return *this;
    }

    addrinfo_iterator operator ++ (int)
    {
        addrinfo_iterator r(*this);
        this->operator ++();
        return r;
    }



    static addrinfo_iterator begin (const pfs::string & hostname);
    static addrinfo_iterator end ()
    {
        return addrinfo_iterator();
    }
};

#endif // __COMMENT__

}} // pfs::net

#endif // __PFS_COMPILER_GNUC_NET_INET_ADDRINFO_HPP__
