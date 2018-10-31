#pragma once

void test_gcd_lcm ()
{
    std::cout << "///////////////////////////////////////////////////////////////////////////\n";
    std::cout << "//                     GCD - Greatest Common Divisor                     //\n";
    std::cout << "///////////////////////////////////////////////////////////////////////////\n";

    ADD_TESTS(18);

    TEST_OK(pfs::gcd<int>(0,1) == 1);
    TEST_OK(pfs::gcd<int>(1,0) == 1);
    TEST_OK(pfs::gcd<int>(0,0) == 0);
    TEST_OK(pfs::gcd<int>(1,1) == 1);
    TEST_OK(pfs::gcd<int>(1,3) == 1);
    TEST_OK(pfs::gcd<int>(2,3) == 1);
    TEST_OK(pfs::gcd<int>(2,4) == 2);
    TEST_OK(pfs::gcd<int>(3,9) == 3);
    TEST_OK(pfs::gcd<int>(9,3) == 3);

    std::cout << "///////////////////////////////////////////////////////////////////////////\n";
    std::cout << "//                  LCM - Lowest (Least) Common Multiple                 //\n";
    std::cout << "///////////////////////////////////////////////////////////////////////////\n";

    TEST_OK(pfs::lcm<int>(0,1) == 0);
    TEST_OK(pfs::lcm<int>(1,0) == 0);
    TEST_OK(pfs::lcm<int>(0,0) == 0);
    TEST_OK(pfs::lcm<int>(1,1) == 1);
    TEST_OK(pfs::lcm<int>(1,3) == 3);
    TEST_OK(pfs::lcm<int>(2,3) == 6);
    TEST_OK(pfs::lcm<int>(2,4) == 4);
    TEST_OK(pfs::lcm<int>(3,9) == 9);
    TEST_OK(pfs::lcm<int>(6,8) == 24);
}

