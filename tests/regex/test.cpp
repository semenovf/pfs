/**
 * @file   test.cpp
 * @author
 * @date
 *
 * @brief testing ...
 */

#include <pfs/test/test.hpp>
#include "test_regex_match.hpp"

int main(int argc, char *argv[])
{
    PFS_UNUSED(argc);
    PFS_UNUSED(argv);

    BEGIN_TESTS(0);
    
    test_regex_match<char>();

    return END_TESTS;
}

