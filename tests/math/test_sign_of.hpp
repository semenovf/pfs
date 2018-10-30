#pragma once
#include <pfs/limits.hpp>

template <typename T>
void test_sign_of_signed_integral ()
{
    ADD_TESTS(7);

    TEST_OK(pfs::sign_of<T>(0) == 0);
    TEST_OK(pfs::sign_of<T>(-0) == 0);
    TEST_OK(pfs::sign_of<T>(+0) == 0);
    TEST_OK(pfs::sign_of<T>(1) == 1);
    TEST_OK(pfs::sign_of<T>(-1) == -1);
    TEST_OK(pfs::sign_of<T>(pfs::numeric_limits<T>::min()) == -1);
    TEST_OK(pfs::sign_of<T>(pfs::numeric_limits<T>::max()) == 1);
}

template <typename T>
void test_sign_of_unsigned_integral ()
{
    ADD_TESTS(7);

    TEST_OK(pfs::sign_of<T>(0) == 0);
    TEST_OK(pfs::sign_of<T>(-0) == 0);
    TEST_OK(pfs::sign_of<T>(+0) == 0);
    TEST_OK(pfs::sign_of<T>(1) == 1);
    TEST_OK(pfs::sign_of<T>(-1) == 1);
    TEST_OK(pfs::sign_of<T>(pfs::numeric_limits<T>::min()) == 0);
    TEST_OK(pfs::sign_of<T>(pfs::numeric_limits<T>::max()) == 1);
}

template <typename T>
void test_sign_of_fp ()
{
    ADD_TESTS(7);

    TEST_OK(pfs::sign_of<T>(0) == 0);
    TEST_OK(pfs::sign_of<T>(-0) == 0);
    TEST_OK(pfs::sign_of<T>(+0) == 0);
    TEST_OK(pfs::sign_of<T>(1) == 1);
    TEST_OK(pfs::sign_of<T>(-1) == -1);
    TEST_OK(pfs::sign_of<T>(pfs::numeric_limits<T>::min()) == 1);
    TEST_OK(pfs::sign_of<T>(pfs::numeric_limits<T>::max()) == 1);
    TEST_OK(pfs::sign_of<T>(-pfs::numeric_limits<T>::min()) == -1);
    TEST_OK(pfs::sign_of<T>(-pfs::numeric_limits<T>::max()) == -1);
}

void test_sign_of ()
{
    std::cout << "///////////////////////////////////////////////////////////////////////////\n";
    std::cout << "//                             sign_of                                   //\n";
    std::cout << "///////////////////////////////////////////////////////////////////////////\n";

    ADD_TESTS(0);

    test_sign_of_signed_integral<char>();
    test_sign_of_signed_integral<short>();
    test_sign_of_signed_integral<int>();
    test_sign_of_signed_integral<long int>();
#if PFS_HAVE_LONG_LONG
    test_sign_of_signed_integral<long long int>();
#endif

    test_sign_of_unsigned_integral<unsigned char>();
    test_sign_of_unsigned_integral<unsigned short>();
    test_sign_of_unsigned_integral<unsigned int>();
    test_sign_of_unsigned_integral<unsigned long int>();
#if PFS_HAVE_LONG_LONG
    test_sign_of_unsigned_integral<unsigned long long int>();
#endif

    test_sign_of_fp<float>();
    test_sign_of_fp<double>();

#if PFS_HAVE_LONG_DOUBLE
    test_sign_of_fp<long double>();
#endif
}
