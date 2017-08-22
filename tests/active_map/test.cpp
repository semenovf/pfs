#include <pfs/test/test.hpp>
#include "test_active_map.hpp"

int main(int argc, char *argv[])
{
    PFS_UNUSED(argc);
    PFS_UNUSED(argv);

    BEGIN_TESTS(0);

    test::active_map::basic::test();

    return END_TESTS;
}

