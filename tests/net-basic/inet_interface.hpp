#include <iostream>
#include "pfs/net/inet_interface.hpp"
#include "pfs/traits/stdcxx/string.hpp"
#include "pfs/traits/stdcxx/list.hpp"

#ifdef HAVE_QT_CORE
    #include "pfs/traits/qt/string.hpp"
    #include "pfs/traits/qt/list.hpp"
#endif

template <typename StringImplType>
void test_inet_interface_names ()
{
    ADD_TESTS(1);

    typedef pfs::string<StringImplType> string_t;
    typedef pfs::stringlist<string_t>   stringlist_t;

    stringlist_t names;
    pfs::error_code ec;

    TEST_OK(pfs::net::inet_interface::names(names, ec));

    if (ec) {
        std::cerr << "***ERROR: pfs::net::inet_interface::names(): " << pfs::to_string<string_t>(ec) << std::endl;
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

template <typename StringImplType, template <typename> class Container>
void test_inet_interface_addrs ()
{
    typedef pfs::string<StringImplType> string_t;
    typedef Container<pfs::net::inet4_addr> addr_container_t;

    ADD_TESTS(1);

    addr_container_t addrs;
    pfs::error_code ec;

    TEST_OK(pfs::net::inet_interface::inet4_addrs(addrs, ec));

    if (ec) {
        std::cerr << "***ERROR: pfs::net::inet_interface::addrs(): " << pfs::to_string<string_t>(ec) << std::endl;
    } else {
        typename addr_container_t::const_iterator it = addrs.cbegin();
        typename addr_container_t::const_iterator last = addrs.cend();

        std::cout << "Addresses:" << std::endl;
        int i = 1;
        while (it != last) {
            std::cout << '\t' << i << ": " << pfs::to_string<string_t>(*it) << std::endl;
            ++it;
            ++i;
        }
    }
}

template <typename T>
struct stdcxx_list : public pfs::traits::sequence_container<T, pfs::stdcxx::list>
{};

#ifdef HAVE_QT_CORE
template <typename T>
struct qt_list : public pfs::traits::sequence_container<T, pfs::qt::list>
{};
#endif

void test_inet_interface ()
{
    test_inet_interface_names<pfs::stdcxx::string>();
    test_inet_interface_addrs<pfs::stdcxx::string, stdcxx_list>();

#ifdef HAVE_QT_CORE
    test_inet_interface_names<pfs::qt::string>();
    test_inet_interface_addrs<pfs::qt::string, qt_list>();
#endif
}
