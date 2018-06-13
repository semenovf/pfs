#include <iostream>
#include "pfs/net/inet_interface.hpp"
#include "pfs/string.hpp"
#include "pfs/deque.hpp"
#include "pfs/list.hpp"
#include "pfs/vector.hpp"

void test_inet_interface_names ()
{
    ADD_TESTS(1);

    typedef pfs::string               string_t;
    typedef pfs::stringlist<string_t> stringlist_t;

    stringlist_t names;
    pfs::error_code ec;

    TEST_OK(pfs::net::inet_interface::names(names, ec));

    if (ec) {
        std::cerr << "***ERROR: pfs::net::inet_interface::names(): " << pfs::to_string(ec) << std::endl;
    } else {
        typename stringlist_t::const_iterator it = names.cbegin();
        typename stringlist_t::const_iterator last = names.cend();

        std::cout << "Interfaces:" << std::endl;
        int i = 1;
        while (it != last) {
            std::cout << '\t' << i << ": " << (*it) << std::endl;
            ++it;
            ++i;
        }
    }
}

template <template <typename> class Container>
void test_inet_interface_addrs ()
{
    typedef pfs::string string_t;
    typedef Container<pfs::net::inet4_addr> addr_container_t;

    ADD_TESTS(1);

    addr_container_t addrs;
    pfs::error_code ec;

    TEST_OK(pfs::net::inet_interface::inet4_addrs(addrs, ec));

    if (ec) {
        std::cerr << "***ERROR: pfs::net::inet_interface::addrs(): " << pfs::to_string(ec) << std::endl;
    } else {
        typename addr_container_t::const_iterator it = addrs.cbegin();
        typename addr_container_t::const_iterator last = addrs.cend();

        std::cout << "Addresses:" << std::endl;
        int i = 1;
        while (it != last) {
            std::cout << '\t' << i << ": " << pfs::to_string(*it) << std::endl;
            ++it;
            ++i;
        }
    }
}

void test_inet_interface ()
{
    test_inet_interface_names();
    test_inet_interface_addrs<pfs::deque>();
    test_inet_interface_addrs<pfs::list>();
    test_inet_interface_addrs<pfs::vector>();
}
