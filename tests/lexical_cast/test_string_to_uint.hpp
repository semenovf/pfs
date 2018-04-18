#pragma once
#include <iostream>
#include <cstdlib> // strtoul, strtoull
#include <cerrno>

template <typename UintmaxT>
UintmaxT std_string_to_uint (char const * nptr
        , char ** endptr
        , int base);

template <>
inline uint32_t std_string_to_uint<uint32_t> (char const * nptr
        , char ** endptr
        , int base)
{
    errno = 0;
    unsigned long int r = strtoul(nptr, endptr, base);
    while (r > pfs::numeric_limits<uint32_t>::max())
        r /= base;
    return static_cast<uint32_t>(r);
}

#if PFS_HAVE_LONG_LONG

template <>
inline uint64_t std_string_to_uint<uint64_t> (char const * nptr
        , char ** endptr
        , int base)
{
    errno = 0;
    unsigned long long int r = strtoull(nptr, endptr, base);
    while (r > pfs::numeric_limits<uint64_t>::max())
        r /= base;
    return static_cast<uint64_t>(r);
}

#endif

template <typename UintT>
static bool __test_string_to_uint (char const * s, int radix
    , UintT result_sample
    , int overflow_sample
    , ptrdiff_t badpos_increment)
{
    typedef pfs::string::const_iterator char_iterator_type;

    bool r = true;
    char_iterator_type badpos;
    pfs::error_code ec;
    pfs::string str(s);

    UintT result = pfs::string_to_uint<UintT, char_iterator_type>(str.cbegin(), str.cend()
        , & badpos
        , radix
        , ec);

    UintT std_result = std_string_to_uint<UintT>(s, 0, radix);

    if (result != std_result) {
        // Ignore result with overflow
        if (!(errno == ERANGE && (std_result == 0 || std_result == pfs::numeric_limits<UintT>::max()))) {
            std::cout << "***ERROR: result = " << result
                    << " does not math result of strtoul[l](): "
                    << std_result
                    << std::endl;
            r = false;
        }
    }

    if (result != result_sample) {
        std::cout << "***ERROR: result = " << result << ", but expected " << result_sample << std::endl;
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

void test_string_to_uint ()
{
    ADD_TESTS(16);

    TEST_OK((__test_string_to_uint<uint32_t>("0", 10, 0, 0, 0)));

// TODO
//    "0" , 0, 0);
//    "+0", 0, 0);
//    "-0", 0, 0);
//    "1" , 1, 0);
//    "+1", 1, 0);
//    "-1", static_cast<uintmax_t>(-1), 0);
    int32_t k = -429496729;
    //k *= -1;
    uint32_t k1 = k;

    TEST_OK((__test_string_to_uint<uint32_t>( "4294967295", 10, 4294967295UL, 0, 0)));
    TEST_OK((__test_string_to_uint<uint32_t>("-4294967295", 10, 1, 0, 0)));
    TEST_OK((__test_string_to_uint<uint32_t>("-4294967294", 10, 2, 0, 0)));
    TEST_OK((__test_string_to_uint<uint32_t>("-429496729" , 10, 3865470567UL, 0, 0)));
    TEST_OK((__test_string_to_uint<uint32_t>("-42949672"  , 10, 4252017624UL, 0, 0)));

    // strtoul() returns 0 and do not set errno into ERANGE, see analogous test for uint64_t below (marked by [!])
    TEST_OK((__test_string_to_uint<uint32_t>( "4294967296" , 10, 429496729UL, 1, 1)));

    // strtoul() returns 1 and do not set errno into ERANGE, see analogous test for uint64_t below (marked by [!])
    TEST_OK((__test_string_to_uint<uint32_t>( "4294967297" , 10, 429496729UL, 1, 1)));

    // strtoul() returns 0 and do not set errno into ERANGE, see analogous test for uint64_t below (marked by [!])
    TEST_OK((__test_string_to_uint<uint32_t>("-4294967296" , 10, 3865470567UL, -1, 1)));

    // strtoul() returns 4294967295 and do not set errno into ERANGE, see analogous test for uint64_t below (marked by [!])
    TEST_OK((__test_string_to_uint<uint32_t>("-4294967297" , 10, 3865470567UL, -1, 1)));

    TEST_OK((__test_string_to_uint<uint32_t>( "FFFFFFFF", 16, 4294967295UL, 0, 0)));
    TEST_OK((__test_string_to_uint<uint32_t>("-FFFFFFFF", 16, 1, 0, 0)));
    TEST_OK((__test_string_to_uint<uint32_t>("-FFFFFFFE", 16, 2, 0, 0)));
    TEST_OK((__test_string_to_uint<uint32_t>("-FFFFFFF" , 16, 4026531841UL, 0, 0)));
    TEST_OK((__test_string_to_uint<uint32_t>("-FFFFFF"  , 16, 4278190081UL, 0, 0)));

    // strtoul() returns 0 and do not set errno into ERANGE, see analogous test for uint64_t below (marked by [!!])
    TEST_OK((__test_string_to_uint<uint32_t>( "100000000", 16, 268435456UL, 1, 1)));

#if PFS_HAVE_INT64
    ADD_TESTS(15);

    TEST_OK((__test_string_to_uint<uint64_t>( "18446744073709551615", 10, 18446744073709551615ULL, 0, 0)));
    TEST_OK((__test_string_to_uint<uint64_t>("-18446744073709551615", 10, 1, 0, 0)));
    TEST_OK((__test_string_to_uint<uint64_t>("-18446744073709551614", 10, 2, 0, 0)));
    TEST_OK((__test_string_to_uint<uint64_t>("-1844674407370955161" , 10, 16602069666338596455ULL, 0, 0)));
    TEST_OK((__test_string_to_uint<uint64_t>("-184467440737095516"  , 10, 18262276632972456100ULL, 0, 0)));

    TEST_OK((__test_string_to_uint<uint64_t>( "18446744073709551616", 10, 1844674407370955161ULL, 1, 1)));   // [!]
    TEST_OK((__test_string_to_uint<uint64_t>( "18446744073709551617", 10, 1844674407370955161ULL, 1, 1)));   // [!]
    TEST_OK((__test_string_to_uint<uint64_t>("-18446744073709551616", 10, 16602069666338596455ULL, -1, 1))); // [!]
    TEST_OK((__test_string_to_uint<uint64_t>("-18446744073709551617", 10, 16602069666338596455ULL, -1, 1))); // [!]

    TEST_OK((__test_string_to_uint<uint64_t>( "FFFFFFFFFFFFFFFF", 16, 18446744073709551615ULL, 0, 0)));
    TEST_OK((__test_string_to_uint<uint64_t>("-FFFFFFFFFFFFFFFF", 16, 1, 0, 0)));
    TEST_OK((__test_string_to_uint<uint64_t>("-FFFFFFFFFFFFFFFE", 16, 2, 0, 0)));
    TEST_OK((__test_string_to_uint<uint64_t>("-FFFFFFFFFFFFFFF" , 16, 17293822569102704641ULL, 0, 0)));
    TEST_OK((__test_string_to_uint<uint64_t>("-FFFFFFFFFFFFFF"  , 16, 18374686479671623681ULL, 0, 0)));

    TEST_OK((__test_string_to_uint<uint64_t>( "10000000000000000", 16, 1152921504606846976ULL, 1, 1))); // [!!]

#endif
}
