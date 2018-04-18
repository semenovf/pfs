#pragma once
#include <iostream>
#include <cstdlib> // strtol, strtoll
#include <cerrno>

template <typename IntT>
IntT std_string_to_int (char const * nptr
        , char ** endptr
        , int base);

template <>
inline int32_t std_string_to_int<int32_t> (char const * nptr
        , char ** endptr
        , int base)
{
    errno = 0;
    long r = strtol(nptr, endptr, base);
    return static_cast<int32_t>(r);
}

#if PFS_HAVE_INT64

template <>
inline int64_t std_string_to_int<int64_t> (char const * nptr
        , char ** endptr
        , int base)
{
    errno = 0;
    long long int r = strtoll(nptr, endptr, base);
    return static_cast<int64_t>(r);
}

#endif

template <typename IntT>
static bool __test_string_to_int (char const * s, int radix
    , IntT result_sample
    , int overflow_sample
    , ptrdiff_t badpos_increment)
{
    typedef pfs::string::const_iterator char_iterator_type;

    bool r = true;
    char_iterator_type badpos;
    pfs::error_code ec;
    pfs::string str(s);

    IntT result = pfs::string_to_int<IntT, char_iterator_type>(str.cbegin(), str.cend()
        , & badpos
        , radix
        , ec);

    IntT std_result = std_string_to_int<IntT>(s, 0, radix);

    if (result != std_result) {
        // Ignore result with overflow
        if (!(errno == ERANGE /*&& (std_result == 0
                || std_result == pfs::numeric_limits<IntmaxT>::max())*/)) {
            std::cout << "***ERROR: result = " << result
                    << " does not math result of strtol[l](): " << std_result << std::endl;
            r = false;
        }
    }

    if (result != result_sample) {
        std::cout << "***ERROR: result = " << result
                << ", but expected " << result_sample << std::endl;
        r = false;
    }

    if (ec.value() == static_cast<int>(pfs::lexical_cast_errc::underflow) && overflow_sample >= 0) {
        std::cout << "***ERROR: underflow (-1), but expected " << overflow_sample << std::endl;
        r = false;
    }

    if (ec.value() == static_cast<int>(pfs::lexical_cast_errc::overflow) && overflow_sample <= 0) {
        std::cout << "***ERROR: overflow (1), but expected " << overflow_sample << std::endl;
        r = false;
    }

    pfs::advance(badpos, badpos_increment);

    if (badpos != str.cend()) {
        std::cout << "***ERROR: badpos does not match" << std::endl;
        r = false;
    }

    return r;
}

void test_string_to_int ()
{
    ADD_TESTS(8);

    TEST_OK((__test_string_to_int<int32_t>("0", 10, 0, 0, 0)));

    // TODO
    // "0" , 0, 0);
    // "+0", 0, 0);
    // "-0", 0, 0);
    // "1" , 1, 0);
    // "+1", 1, 0);
    // "-1", static_cast<uintmax_t>(-1), 0);

    TEST_OK((__test_string_to_int<int32_t>( "2147483647", 10,  2147483647L, 0, 0)));
    TEST_OK((__test_string_to_int<int32_t>("-2147483648", 10, -2147483647L - 1, 0, 0)));

    TEST_OK((__test_string_to_int<int32_t>( "2147483648", 10,  214748364L, 1, 1)));
    TEST_OK((__test_string_to_int<int32_t>("-2147483649", 10, -214748364L, -1, 1)));

    TEST_OK((__test_string_to_int<int32_t>( "7FFFFFFF", 16,  2147483647L, 0, 0)));
    TEST_OK((__test_string_to_int<int32_t>( "80000000", 16, 0x80000000/16, 1, 1)));
    TEST_OK((__test_string_to_int<int32_t>( "FFFFFFFF", 16, 0xFFFFFFFF/16, 1, 1)));

#if PFS_HAVE_INT64
    ADD_TESTS(4);

    TEST_OK((__test_string_to_int<int64_t>( "9223372036854775807", 10,  9223372036854775807LL, 0, 0)));
    TEST_OK((__test_string_to_int<int64_t>("-9223372036854775808", 10, -9223372036854775807LL - 1LL, 0, 0)));

    TEST_OK((__test_string_to_int<int64_t>( "9223372036854775808", 10,  922337203685477580LL, 1, 1)));
    TEST_OK((__test_string_to_int<int64_t>("-9223372036854775809", 10, -922337203685477580LL, -1, 1)));
#endif
}
