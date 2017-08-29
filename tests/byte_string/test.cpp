/**
 * @file   test.cpp
 * @author
 * @date
 *
 * @brief Testing pfs::mpl::byte_string.hpp.
 */

#include <pfs/test/test.hpp>
#include <pfs/byte_string.hpp>
#include "test_byte_stream.hpp"

int main (int argc, char *argv[])
{
    PFS_UNUSED(argc);
    PFS_UNUSED(argv);

    BEGIN_TESTS(0);

    test_byte_stream();

    return END_TESTS;
}
