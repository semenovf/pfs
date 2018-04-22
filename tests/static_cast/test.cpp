#include <pfs/test.hpp>
#include <pfs/types.hpp>

int main ()
{
    BEGIN_TESTS(1);

    ssize_t i1 = 1234;
    TEST_OK(static_cast<size_t>(i1) == 1234);

    return END_TESTS;
}
