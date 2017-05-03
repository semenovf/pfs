/**
 * @file   test.cpp
 * @author
 * @date
 *
 * @brief testing ...
 */

#include <pfs/test/test.hpp>
#include <pfs.hpp>

#include "pfs/traits/kmvcontainer.hpp"
//#include <pfs/traits/stdcxx/map.hpp>
//#include <pfs/traits/stdcxx/multimap.hpp>
//
//// Enabled by `qt_enable`
//#ifdef QT_CORE_LIB
//#   include <pfs/traits/qt/map.hpp>
//#   include <pfs/traits/qt/multimap.hpp>
//#endif
//
//#include "test_access.hpp"
//#include "test_compare_swap.hpp"

int main (int argc, char *argv[])
{
 	PFS_UNUSED(argc);
	PFS_UNUSED(argv);
    
	BEGIN_TESTS(0);
    
    
    
//    test_access<pfs::traits::map, stdcxx::map>();
//    test_compare<pfs::traits::map, stdcxx::map>();
//    test_swap<pfs::traits::map, stdcxx::map>();
    
    
#ifdef QT_CORE_LIB
//    test_access<pfs::traits::map, QMap>();
//    test_compare<pfs::traits::map, QMap>();
//    test_swap<pfs::traits::map, QMap>();
#endif    

	return END_TESTS;
}
