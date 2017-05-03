/**
 * @file   test.cpp
 * @author
 * @date
 *
 * @brief Testing numeric features
 */

#include <pfs/test/test.hpp>
#include "iota.hpp"

int main (int argc, char * argv[])
{
    PFS_UNUSED(argc);
    PFS_UNUSED(argv);

    BEGIN_TESTS(0);

    test_iota();

    return END_TESTS;
}

