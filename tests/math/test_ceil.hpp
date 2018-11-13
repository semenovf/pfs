#pragma once

template <typename T>
void test_ceil ()
{
    ADD_TESTS(6);

    TEST_OK(pfs::ceil(T(-3.1)) == T(-3.0));
    TEST_OK(pfs::ceil(T(-2.1)) == T(-2.0));
    TEST_OK(pfs::ceil(T(-1.1)) == T(-1.0));
    TEST_OK(pfs::ceil(T(-0.1)) == T(0.0));
    TEST_OK(pfs::ceil(T(1.1)) == T(2.0));
    TEST_OK(pfs::ceil(T(2.1)) == T(3.0));
}

void test_ceil ()
{
    std::cout << "///////////////////////////////////////////////////////////////////////////\n";
    std::cout << "//                               ceil                                    //\n";
    std::cout << "///////////////////////////////////////////////////////////////////////////\n";

    ADD_TESTS(0);

    test_ceil<float>();
    test_ceil<double>();

#if PFS_HAVE_LONG_DOUBLE
    test_ceil<long double>();
#endif
}
