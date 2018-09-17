#include <pfs/test.hpp>
#include "test_path.hpp"
#include "test_directory_entry.hpp"

int main ()
{
    BEGIN_TESTS(0);

    test_path();
    test_directory_entry();

    return END_TESTS;
}

