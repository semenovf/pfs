/**
 * @file   test.cpp
 * @author
 * @date
 *
 * @brief testing ...
 */

#include <pfs/test/test.hpp>
#include <pfs.hpp>

#include <pfs/foundation/stdcxx/list.hpp>

// Enabled by `qt_enable`
#ifdef QT_CORE_LIB
#   include <pfs/foundation/qt/list.hpp>
#endif

#include "test_iterator.hpp"
//#include "test_access.hpp"

int main (int argc, char *argv[])
{
 	PFS_UNUSED(argc);
	PFS_UNUSED(argv);
    
	BEGIN_TESTS(0);
    
    test_iterator<stdcxx::list>();
//    test_access<pfs::mpl::stdcxx::list>();
    
#ifdef QT_CORE_LIB
    test_iterator<qt::list>();
//    test_access<QList>();
#endif    

	return END_TESTS;
}
