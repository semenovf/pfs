/**
 * @file   test.cpp
 * @author
 * @date
 *
 * @brief testing ...
 */

#include <pfs/test/test.hpp>
#include <pfs.hpp>

#include "test_input_iterator.hpp"
#include "test_output_iterator.hpp"

int main (int argc, char *argv[])
{
	PFS_UNUSED(argc);
	PFS_UNUSED(argv);

	BEGIN_TESTS(0);

  	test_input_iterator();
    test_output_iterator();
    
	return END_TESTS;
}
