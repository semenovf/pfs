#include <pfs/test.hpp>
#include <pfs/types.hpp>
#include <pfs/limits.hpp>

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
    BEGIN_TESTS(3);

    ssize_t i1 = 2614;
    TEST_OK(static_cast<size_t>(i1) == 2614);
    read(available());

    size_t navailable = pfs::integral_cast_check<size_t>(available());
    std::cout << "navailable=" << navailable << std::endl;
    TEST_OK(navailable == 2614);

    return END_TESTS;
}
