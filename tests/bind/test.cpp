#include <iostream>
#include <pfs/test.hpp>
#include <pfs/functional.hpp>

int f (int n1, int n2 , int n3, int const & n4, int n5)
{
    std::cout << n1
            << ' ' << n2
            << ' ' << n3
            << ' ' << n4
            << ' ' << n5
            << std::endl;

    return n1 + n2 + n3 + n4 + n5;
}

// int g (int n1)
// {
//     return n1;
// }
//
// struct foo
// {
//     int sum (int n1, int n2)
//     {
//         std::cout << n1
//                 << ' ' << n2
//                 << std::endl;
//         return n1 + n2;
//     }
// };

int main ()
{
    BEGIN_TESTS(7);

    PFS_AUTO(f1, pfs::bind<int>(f, _1, _2, _3, _4, _5));
    PFS_AUTO(f2, pfs::bind<int>(f, 1, _1, _2, _3, _4));
    PFS_AUTO(f3, pfs::bind<int>(f, 1, 2, _1, _2, _3));
    PFS_AUTO(f4, pfs::bind<int>(f, 1, 2, 3, _1, _2));
    PFS_AUTO(f5, pfs::bind<int>(f, 1, 2, 3, 4, _1));
    PFS_AUTO(f6, pfs::bind<int>(f, 1, 2, 3, 4, 5));
    PFS_AUTO(f7, pfs::bind<int>(f, _5, _4, _3, _2, _1));

    TEST_OK(f1(1, 2, 3, 4, 5) == 15);
    TEST_OK(f2(2, 3, 4, 5) == 15);
    TEST_OK(f3(3, 4, 5) == 15);
    TEST_OK(f4(4, 5) == 15);
    TEST_OK(f5(5) == 15);
    TEST_OK(f6() == 15);
    TEST_OK(f7(1, 2, 3, 4, 5) == 15);

    return END_TESTS;
}
