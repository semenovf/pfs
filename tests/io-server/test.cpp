#include <pfs/test.hpp>

extern void test_pool_dispatcher ();

int main ()
{
    BEGIN_TESTS(0);

    test_pool_dispatcher();

    return END_TESTS;
}

