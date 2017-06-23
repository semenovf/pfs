/**
 * @file   test.cpp
 * @author
 * @date
 *
 * @brief Testing container traits
 */

#include "pfs/test/test.hpp"
#include "pfs/utility.hpp"

#include "pfs/traits/stdcxx/vector.hpp"
#include "pfs/traits/stdcxx/deque.hpp"
#include "pfs/traits/stdcxx/list.hpp"
#include "pfs/traits/stdcxx/set.hpp"
#include "pfs/traits/stdcxx/multiset.hpp"
#include <pfs/traits/stdcxx/map.hpp>
#include <pfs/traits/stdcxx/multimap.hpp>
//
// Enabled by `qt_enable`
#ifdef QT_CORE_LIB
#   include <pfs/traits/qt/vector.hpp>
#   include <pfs/traits/qt/list.hpp>
#   include <pfs/traits/qt/linked_list.hpp>
#   include <pfs/traits/qt/set.hpp>
#   include <pfs/traits/qt/map.hpp>
#   include <pfs/traits/qt/multimap.hpp>
#   include <pfs/traits/qt/hash.hpp>
#   include <pfs/traits/qt/multihash.hpp>
#endif

#include "container.hpp"
#include "sequence_container.hpp"
#include "associative_container.hpp"

int main (int argc, char *argv[])
{
 	PFS_UNUSED(argc);
	PFS_UNUSED(argv);
    
	BEGIN_TESTS(0);

    test_container_basic<int, pfs::traits::stdcxx::vector>();
    test_container_basic<int, pfs::traits::stdcxx::vector_reference>();
    test_container_basic<int, pfs::traits::stdcxx::deque>();
    test_container_basic<int, pfs::traits::stdcxx::deque_reference>();
    test_container_basic<int, pfs::traits::stdcxx::list>();
    test_container_basic<int, pfs::traits::stdcxx::list_reference>();
    test_container_basic<int, pfs::traits::stdcxx::set>();
    test_container_basic<int, pfs::traits::stdcxx::set_reference>();
    test_container_basic<int, pfs::traits::stdcxx::multiset>();
    test_container_basic<int, pfs::traits::stdcxx::multiset_reference>();
    test_container_basic<pfs::traits::kv<int, int>, pfs::traits::stdcxx::map>();
    test_container_basic<pfs::traits::kv<int, int>, pfs::traits::stdcxx::map_reference>();
    test_container_basic<pfs::traits::kv<int, int>, pfs::traits::stdcxx::multimap>();
    test_container_basic<pfs::traits::kv<int, int>, pfs::traits::stdcxx::multimap_reference>();
    
    test_associative_container_coverage<pfs::traits::kv<int, int>, pfs::traits::stdcxx::map>();
    test_associative_container_coverage<pfs::traits::kv<int, int>, pfs::traits::stdcxx::map_reference>();
    test_associative_container_coverage<pfs::traits::kv<int, int>, pfs::traits::stdcxx::multimap>();
    test_associative_container_coverage<pfs::traits::kv<int, int>, pfs::traits::stdcxx::multimap_reference>();

#if __cplusplus >= 201103L    
//    test_container_basic<int, pfs::traits::stdcxx::array>();
//    test_container_basic<pfs::traits::stdcxx::forward_list>();
//    test_container_basic<pfs::traits::stdcxx::unordered_set>();
//    test_container_basic<pfs::traits::stdcxx::unordered_multiset>();
//    test_container_basic<pfs::traits::stdcxx::unordered_map>();
//    test_container_basic<pfs::traits::stdcxx::unordered_multimap>();
#endif    

#ifdef QT_CORE_LIB
    // TODO Implement
//    test_container_basic<int, pfs::traits::qt::vector>();                   // QVector<T>
//    test_container_basic<int, pfs::traits::qt::list>();                     // QList<T>
//    test_container_basic<int, pfs::traits::qt::linked_list>();              // QLinkedList<T>
//    test_container_basic<int, pfs::traits::qt::set>();                      // QSet<T>
//    test_container_basic<pfs::traits::kv_type<int, int>, pfs::traits::qt::map>();      // QMap<Key, T>
//    test_container_basic<pfs::traits::kv_type<int, int>, pfs::traits::qt::multimap>(); // QMultiMap<Key, T>
//    test_container_basic<pfs::traits::kv_type<int, int>, pfs::traits::qt::hash>();     // QHash<Key, T>
//    test_container_basic<pfs::traits::kv_type<int, int>, pfs::traits::qt::multihash>();// QMultiHash<Key, T>
#endif    

	return END_TESTS;
}
