#include <pfs/test/test.hpp>
#include "test_duration.hpp"

int main(int argc, char *argv[])
{
    PFS_UNUSED(argc);
    PFS_UNUSED(argv);

    BEGIN_TESTS(0);
    
    test_duration();

    return END_TESTS;
}

