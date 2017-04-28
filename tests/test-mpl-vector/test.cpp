/**
 * @file   test.cpp
 * @author
 * @date
 *
 * @brief testing ...
 */

#include <vector>
#include <pfs/test/test.hpp>
#include <pfs/traits/stdcxx/vector.hpp>

// Enabled by `qt_enable`
#ifdef QT_CORE_LIB
#   include <pfs/traits/qt/vector.hpp>
#endif

#include "test_iterator.hpp"
//#include "test_access.hpp"

int main (int argc, char *argv[])
{
 	PFS_UNUSED(argc);
	PFS_UNUSED(argv);
    
	BEGIN_TESTS(0);
    
    test_iterator<stdcxx::vector>();
//    test_access<stdcxx::vector>();
    
#ifdef QT_CORE_LIB
    test_iterator<QVector>();
//    test_access<qt::vector>();
#endif    

	return END_TESTS;
}
