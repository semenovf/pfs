/**
 * @file   test.cpp
 * @author
 * @date
 *
 * @brief testing ...
 */

#include <pfs/test/test.hpp>
#include "test_path.hpp"
#include "test_directory_entry.hpp"

int main(int argc, char *argv[])
{
    PFS_UNUSED(argc);
    PFS_UNUSED(argv);

    BEGIN_TESTS(0);
    
    test_path();
    test_directory_entry();

    return END_TESTS;
}

