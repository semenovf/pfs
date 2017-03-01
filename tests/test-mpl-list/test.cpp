/**
 * @file   test.cpp
 * @author
 * @date
 *
 * @brief testing ...
 */

#include <pfs/test/test.hpp>
#include <pfs.hpp>

#include <pfs/foundation/cxx/stdcxx/list.hpp>

// Enabled by `qt_enable`
#ifdef QT_CORE_LIB
#   include <pfs/foundation/cxx/qt/list.hpp>
#endif

#include "test_iterator.hpp"
//#include "test_access.hpp"

int main (int argc, char *argv[])
{
 	PFS_UNUSED(argc);
	PFS_UNUSED(argv);
    
	BEGIN_TESTS(0);
    
    test_iterator<foundation::stdcxx>();
//    test_access<foundation::stdcxx>();
    
#ifdef QT_CORE_LIB
    test_iterator<foundation::qt>();
//    test_access<foundation::qt>();
#endif    

	return END_TESTS;
}
