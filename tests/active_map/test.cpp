#include <pfs/test.hpp>
#include "test_active_map.hpp"

int main ()
{
    BEGIN_TESTS(0);

    test::active_map::basic::test();

    return END_TESTS;
}

