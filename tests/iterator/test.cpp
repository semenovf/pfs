#include <pfs/test/test.hpp>
#include <pfs/iterator.hpp>

#include "test_input_iterator.hpp"
#include "test_output_iterator.hpp"
#include "test_forward_iterator.hpp"
#include "test_bidirectional_iterator.hpp"
#include "test_random_access_iterator.hpp"

int main (int argc, char *argv[])
{
	PFS_UNUSED(argc);
	PFS_UNUSED(argv);

	BEGIN_TESTS(0);

  	test_input_iterator();
    test_output_iterator();
    test_forward_iterator();
    test_bidirectional_iterator();
    test_random_access_iterator();
    
	return END_TESTS;
}
