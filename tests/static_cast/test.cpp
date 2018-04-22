#include <pfs/test.hpp>
#include <pfs/types.hpp>

ssize_t available ()
{
    return 2614;
}

void read (size_t n)
{
    TEST_OK(n == 2614);
}

int main ()
{
    BEGIN_TESTS(2);

    ssize_t i1 = 2614;
    TEST_OK(static_cast<size_t>(i1) == 2614);

    read(available());

    return END_TESTS;
}
