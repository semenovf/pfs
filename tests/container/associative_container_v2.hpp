#pragma once
#include "pfs/config.h"
#include "pfs/v2/container.hpp"
#include "pfs/v2/stdcxx/map.hpp"

#if HAVE_QT_CORE
#   include "pfs/v2/qt/map.hpp"
#endif

namespace v2 {

template <typename Key, typename T, typename AssociativeContainerTag>
void test1 ()
{
    ADD_TESTS(4);

    typedef pfs::associative_container::type_traits<Key, T, AssociativeContainerTag> type_traits;
    typedef typename type_traits::type map_type;
    typedef pfs::associative_container::iterators<Key, T, map_type> iterators;
    typedef pfs::associative_container::inserter<Key, T, map_type> inserter;
    typedef pfs::associative_container::finder<Key, T, map_type> finder;

    map_type m;
    inserter(m).insert(1, 1);
    inserter(m).insert(2, 2);
    inserter(m).insert(3, 4);

    typename map_type::iterator it1 = finder(m).find(1);
    typename map_type::iterator it2 = finder(m).find(2);
    typename map_type::iterator it3 = finder(m).find(3);
    typename map_type::iterator it4 = finder(m).find(4);

    TEST_OK(it1 != iterators(m).end());
    TEST_OK(it2 != iterators(m).end());
    TEST_OK(it3 != iterators(m).end());
    TEST_OK(it4 == iterators(m).end());
}

void test_associative_container ()
{
    test1<int, int, pfs::stdcxx::map>();

#if HAVE_QT_CORE
    test1<int, int, pfs::qt::map>();
#endif
}

} // v2
