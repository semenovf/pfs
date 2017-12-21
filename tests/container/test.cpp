#include "pfs/test/test.hpp"
#include "pfs/utility.hpp"

#include "pfs/traits/stdcxx/vector.hpp"
#include "pfs/traits/stdcxx/deque.hpp"
#include "pfs/traits/stdcxx/list.hpp"
#include "pfs/traits/stdcxx/set.hpp"
#include "pfs/traits/stdcxx/multiset.hpp"
#include <pfs/traits/stdcxx/map.hpp>
#include <pfs/traits/stdcxx/multimap.hpp>
#include <pfs/traits/stdcxx/stack.hpp>

// Enabled by `qt_enable`
#ifdef QT_CORE_LIB
#   include <pfs/traits/qt/vector.hpp>
#   include <pfs/traits/qt/list.hpp>
#   include <pfs/traits/qt/linked_list.hpp>
//#   include <pfs/traits/qt/set.hpp>       FIXME
//#   include <pfs/traits/qt/map.hpp>       FIXME
//#   include <pfs/traits/qt/multimap.hpp>  FIXME
//#   include <pfs/traits/qt/hash.hpp>      FIXME
//#   include <pfs/traits/qt/multihash.hpp> FIXME
#   include <pfs/traits/qt/stack.hpp>
#endif

#include "container.hpp"
#include "sequence_container.hpp"
#include "associative_container.hpp"
#include "stack.hpp"

int main ()
{
    BEGIN_TESTS(0);

    test_container_basic<int, pfs::stdcxx::vector>();
    test_container_basic<int, pfs::stdcxx::vector_reference>();
    test_container_basic<int, pfs::stdcxx::deque>();
    test_container_basic<int, pfs::stdcxx::deque_reference>();
    test_container_basic<int, pfs::stdcxx::list>();
    test_container_basic<int, pfs::stdcxx::list_reference>();
    test_container_basic<int, pfs::stdcxx::set>();
    test_container_basic<int, pfs::stdcxx::set_reference>();
    test_container_basic<int, pfs::stdcxx::multiset>();
    test_container_basic<int, pfs::stdcxx::multiset_reference>();
    test_container_basic<pfs::traits::kv<int, int>, pfs::stdcxx::map>();
    test_container_basic<pfs::traits::kv<int, int>, pfs::stdcxx::map_reference>();
    test_container_basic<pfs::traits::kv<int, int>, pfs::stdcxx::multimap>();
    test_container_basic<pfs::traits::kv<int, int>, pfs::stdcxx::multimap_reference>();

    test_associative_container_coverage<pfs::traits::kv<int, int>, pfs::stdcxx::map>();
    test_associative_container_coverage<pfs::traits::kv<int, int>, pfs::stdcxx::map_reference>();
    test_associative_container_coverage<pfs::traits::kv<int, int>, pfs::stdcxx::multimap>();
    test_associative_container_coverage<pfs::traits::kv<int, int>, pfs::stdcxx::multimap_reference>();

    test_stack<int, pfs::stdcxx::stack>();
    test_stack<int, pfs::stdcxx::stack_reference>();

#if __cplusplus >= 201103L
//    test_container_basic<int, pfs::stdcxx::array>();
//    test_container_basic<pfs::stdcxx::forward_list>();
//    test_container_basic<pfs::stdcxx::unordered_set>();
//    test_container_basic<pfs::stdcxx::unordered_multiset>();
//    test_container_basic<pfs::stdcxx::unordered_map>();
//    test_container_basic<pfs::stdcxx::unordered_multimap>();
#endif

#ifdef QT_CORE_LIB
    // TODO Implement
//    test_container_basic<int, pfs::qt::vector>();                   // QVector<T>
//    test_container_basic<int, pfs::qt::list>();                     // QList<T>
//    test_container_basic<int, pfs::qt::linked_list>();              // QLinkedList<T>
//    test_container_basic<int, pfs::qt::set>();                      // QSet<T>
//    test_container_basic<pfs::traits::kv_type<int, int>, pfs::qt::map>();      // QMap<Key, T>
//    test_container_basic<pfs::traits::kv_type<int, int>, pfs::qt::multimap>(); // QMultiMap<Key, T>
//    test_container_basic<pfs::traits::kv_type<int, int>, pfs::qt::hash>();     // QHash<Key, T>
//    test_container_basic<pfs::traits::kv_type<int, int>, pfs::qt::multihash>();// QMultiHash<Key, T>
//    test_stack<int, pfs::qt::stack>();
//    test_stack<int, pfs::qt::stack_reference>();
#endif

    return END_TESTS;
}
