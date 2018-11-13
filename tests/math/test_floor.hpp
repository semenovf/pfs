#pragma once

template <typename T>
void test_floor ()
{
    ADD_TESTS(6);

    TEST_OK(pfs::floor(T(-3.1)) == T(-4.0));
    TEST_OK(pfs::floor(T(-2.1)) == T(-3.0));
    TEST_OK(pfs::floor(T(-1.1)) == T(-2.0));
    TEST_OK(pfs::floor(T(-0.1)) == T(-1.0));
    TEST_OK(pfs::floor(T(0.1)) == T(0.0));
    TEST_OK(pfs::floor(T(1.1)) == T(1.0));
}

void test_floor ()
{
    std::cout << "///////////////////////////////////////////////////////////////////////////\n";
    std::cout << "//                               floor                                   //\n";
    std::cout << "///////////////////////////////////////////////////////////////////////////\n";

    ADD_TESTS(0);

    test_floor<float>();
    test_floor<double>();

#if PFS_HAVE_LONG_DOUBLE
    test_floor<long double>();
#endif
}
