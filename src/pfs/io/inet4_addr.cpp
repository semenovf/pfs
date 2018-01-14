#include <netinet/in.h>
#include "pfs/utility.hpp"
#include "pfs/lexical_cast.hpp"
#include "pfs/net/inet4_addr.hpp"
#include "pfs/traits/stdcxx/list.hpp"

namespace pfs { namespace net {

inet4_addr::inet4_addr (uint8_t a, uint8_t b, uint8_t c, uint8_t d)
    : _addr(0)
{
    _addr |= (static_cast<uint32_t>(a) << 24);
    _addr |= (static_cast<uint32_t>(b) << 16);
    _addr |= (static_cast<uint32_t>(c) << 8);
    _addr |= static_cast<uint32_t>(d);
}

inet4_addr::inet4_addr (uint8_t a, uint8_t b, uint16_t c)
    : _addr(0)
{
    _addr |= (static_cast<uint32_t>(a) << 24);
    _addr |= (static_cast<uint32_t>(b) << 16);
    _addr |= static_cast<uint32_t>(c);
}

inet4_addr::inet4_addr (uint8_t a, uint32_t b)
    : _addr(0)
{
    _addr |= (static_cast<uint32_t>(a) << 24);
    _addr |= b;
}

inet4_addr::inet4_addr (uint32_t a)
    : _addr(a)
{}

}} // pfs::net


