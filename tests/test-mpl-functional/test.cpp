/**
 * @file   test.cpp
 * @author
 * @date
 *
 * @brief testing ...
 */

#include <pfs/test/test.hpp>
#include "reference_wrapper.hpp"

int main(int argc, char *argv[])
{
    PFS_UNUSED(argc);
    PFS_UNUSED(argv);

    BEGIN_TESTS(0);

    test_reference_wrapper();

    return END_TESTS;
}

