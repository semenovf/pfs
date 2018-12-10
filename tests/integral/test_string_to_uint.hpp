#pragma once
#include <iostream>

template <typename UIntT>
static bool test_uint_helper (char const * s
    , int radix
    , UIntT result_sample
    , ptrdiff_t badpos_offset)
{
    typedef pfs::string::const_iterator char_iterator_type;

    bool r = true;
    char_iterator_type badpos;
    char_iterator_type badpos_sample;
    pfs::error_code ec;
    pfs::string str(s);

    UIntT result = pfs::string_to_uint<UIntT, char_iterator_type>(str.cbegin()
        , str.cend()
        , & badpos
        , radix
        , ec);

    if (result != result_sample) {
        std::cout << "***ERROR: result = " << result << ", but expected " << result_sample << std::endl;
        r = false;
    }

    badpos_sample = str.cbegin();
    pfs::advance(badpos_sample, badpos_offset);

    if (badpos != badpos_sample) {
        std::cout << "***ERROR: badpos does not match" << std::endl;
        r = false;
    }

    return r;
}

//
// TODO Add octet radix and some other "non-standard"
//
void test_string_to_uint ()
{
    ADD_TESTS(15);

    TEST_OK(test_uint_helper<uint32_t>("0", 10, 0, 1));
    TEST_OK(test_uint_helper<uint32_t>("+0", 10, 0, 2));
    TEST_OK(test_uint_helper<uint32_t>("-0", 10, 0, 0));
    TEST_OK(test_uint_helper<uint32_t>("1", 10, 1, 1));
    TEST_OK(test_uint_helper<uint32_t>("+1", 10, 1, 2));
    TEST_OK(test_uint_helper<uint32_t>("-1", 10, 0, 0));
    TEST_OK(test_uint_helper<uint32_t>("4294967295", 10, 4294967295UL, 10));
    TEST_OK(test_uint_helper<uint32_t>("+4294967295", 10, 4294967295UL, 11));
    TEST_OK(test_uint_helper<uint32_t>("  +4294967295", 10, 4294967295UL, 13));
    TEST_OK(test_uint_helper<uint32_t>("-4294967295", 10, 0, 0));
    TEST_OK(test_uint_helper<uint32_t>("4294967296" , 10, 429496729UL, 9));

    TEST_OK(test_uint_helper<uint32_t>( "FFFFFFFF", 16, 4294967295UL, 8));
    TEST_OK(test_uint_helper<uint32_t>( "FFFFFFFFF", 16, 4294967295UL, 8));
    TEST_OK(test_uint_helper<uint32_t>("-FFFFFFFF", 16, 0, 0));
    TEST_OK(test_uint_helper<uint32_t>( "10000000", 16, 268435456UL, 8));

#if PFS_HAVE_INT64
    ADD_TESTS(12);

    TEST_OK(test_uint_helper<uint64_t>("0", 10, 0, 1));
    TEST_OK(test_uint_helper<uint64_t>("+0", 10, 0, 2));
    TEST_OK(test_uint_helper<uint64_t>("-0", 10, 0, 0));
    TEST_OK(test_uint_helper<uint64_t>("1", 10, 1, 1));
    TEST_OK(test_uint_helper<uint64_t>("+1", 10, 1, 2));
    TEST_OK(test_uint_helper<uint64_t>("-1", 10, 0, 0));
    TEST_OK(test_uint_helper<uint64_t>("18446744073709551615", 10, 18446744073709551615ULL, 20));
    TEST_OK(test_uint_helper<uint64_t>("18446744073709551616", 10, 1844674407370955161ULL, 19));
    TEST_OK(test_uint_helper<uint64_t>("-18446744073709551615", 10, 0, 0));
    TEST_OK(test_uint_helper<uint64_t>("FFFFFFFFFFFFFFFF", 16, 18446744073709551615ULL, 16));
    TEST_OK(test_uint_helper<uint64_t>("FFFFFFFFFFFFFFFFF", 16, 18446744073709551615ULL, 16));
    TEST_OK(test_uint_helper<uint64_t>("1000000000000000", 16, 1152921504606846976ULL, 16));
#endif
}
