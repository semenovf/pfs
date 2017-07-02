/* 
 * File:   test_string_to_uint.hpp
 * Author: wladt
 *
 * Created on March 19, 2017, 1:37 PM
 */

#ifndef TEST_STRING_TO_UINT_HPP
#define TEST_STRING_TO_UINT_HPP

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
    return strtoul(nptr, endptr, base);
}

#if PFS_HAVE_INT64

template <>
inline uint64_t std_string_to_uint<uint64_t> (char const * nptr
        , char ** endptr
        , int base)
{
    errno = 0;
    return strtoull(nptr, endptr, base);
}

#endif

template <typename UintT, typename StringT>
static bool __test_string_to_uint (char const * s, int radix
    , UintT result_sample
    , int overflow_sample
    , ptrdiff_t badpos_increment)
{
    typedef typename StringT::const_iterator char_iterator_type;
    
    bool r = true;
    char_iterator_type badpos;
    pfs::error_code ec;
    StringT str(s);
    
    UintT result = pfs::string_to_uint<UintT, char_iterator_type>(str.cbegin(), str.cend()
        , & badpos
        , radix
        , ec);
    
    UintT std_result = std_string_to_uint<UintT>(s, 0, radix);
    
    if (result != std_result) {
        // Ignore result with overflow
        if (!(errno == ERANGE && (std_result == 0 || std_result == pfs::numeric_limits<UintT>::max()))) {
            std::cout << "***ERROR: result = " << result << " does not math result of strtoul[l](): " << std_result << std::endl;
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
    
    if (badpos + badpos_increment != str.cend()) {
        std::cout << "***ERROR: badpos does not match" << std::endl;
        r = false;
    }
    
    return r;
}


template <typename StringT>
void test_string_to_uint ()
{
    ADD_TESTS(16);
    
    TEST_OK((__test_string_to_uint<uint32_t, StringT>("0", 10, 0, 0, 0)));

// TODO    
//    "0" , 0, 0);
//    "+0", 0, 0);
//    "-0", 0, 0);
//    "1" , 1, 0);
//    "+1", 1, 0);
//    "-1", static_cast<uintmax_t>(-1), 0);

    TEST_OK((__test_string_to_uint<uint32_t, StringT>( "4294967295", 10, 4294967295UL, 0, 0)));
    TEST_OK((__test_string_to_uint<uint32_t, StringT>("-4294967295", 10, 1, 0, 0)));
    TEST_OK((__test_string_to_uint<uint32_t, StringT>("-4294967294", 10, 2, 0, 0)));
    TEST_OK((__test_string_to_uint<uint32_t, StringT>("-429496729" , 10, 3865470567UL, 0, 0)));
    TEST_OK((__test_string_to_uint<uint32_t, StringT>("-42949672"  , 10, 4252017624UL, 0, 0)));

    // strtoul() returns 0 and do not set errno into ERANGE, see analogous test for uint64_t below (marked by [!])
    TEST_OK((__test_string_to_uint<uint32_t, StringT>( "4294967296" , 10, 429496729UL, 1, 1)));
    
    // strtoul() returns 1 and do not set errno into ERANGE, see analogous test for uint64_t below (marked by [!])
    TEST_OK((__test_string_to_uint<uint32_t, StringT>( "4294967297" , 10, 429496729UL, 1, 1)));
    
    // strtoul() returns 0 and do not set errno into ERANGE, see analogous test for uint64_t below (marked by [!])
    TEST_OK((__test_string_to_uint<uint32_t, StringT>("-4294967296" , 10, 3865470567UL, -1, 1)));
    
    // strtoul() returns 4294967295 and do not set errno into ERANGE, see analogous test for uint64_t below (marked by [!])  
    TEST_OK((__test_string_to_uint<uint32_t, StringT>("-4294967297" , 10, 3865470567UL, -1, 1)));
    
    TEST_OK((__test_string_to_uint<uint32_t, StringT>( "FFFFFFFF", 16, 4294967295UL, 0, 0)));
    TEST_OK((__test_string_to_uint<uint32_t, StringT>("-FFFFFFFF", 16, 1, 0, 0)));
    TEST_OK((__test_string_to_uint<uint32_t, StringT>("-FFFFFFFE", 16, 2, 0, 0)));
    TEST_OK((__test_string_to_uint<uint32_t, StringT>("-FFFFFFF" , 16, 4026531841UL, 0, 0)));
    TEST_OK((__test_string_to_uint<uint32_t, StringT>("-FFFFFF"  , 16, 4278190081UL, 0, 0)));
    
    // strtoul() returns 0 and do not set errno into ERANGE, see analogous test for uint64_t below (marked by [!!])  
    TEST_OK((__test_string_to_uint<uint32_t, StringT>( "100000000", 16, 268435456UL, 1, 1)));
    
#if PFS_HAVE_INT64
    ADD_TESTS(15);

    TEST_OK((__test_string_to_uint<uint64_t, StringT>( "18446744073709551615", 10, 18446744073709551615ULL, 0, 0)));
    TEST_OK((__test_string_to_uint<uint64_t, StringT>("-18446744073709551615", 10, 1, 0, 0)));
    TEST_OK((__test_string_to_uint<uint64_t, StringT>("-18446744073709551614", 10, 2, 0, 0)));
    TEST_OK((__test_string_to_uint<uint64_t, StringT>("-1844674407370955161" , 10, 16602069666338596455ULL, 0, 0)));
    TEST_OK((__test_string_to_uint<uint64_t, StringT>("-184467440737095516"  , 10, 18262276632972456100ULL, 0, 0)));

    TEST_OK((__test_string_to_uint<uint64_t, StringT>( "18446744073709551616", 10, 1844674407370955161ULL, 1, 1)));   // [!]
    TEST_OK((__test_string_to_uint<uint64_t, StringT>( "18446744073709551617", 10, 1844674407370955161ULL, 1, 1)));   // [!]
    TEST_OK((__test_string_to_uint<uint64_t, StringT>("-18446744073709551616", 10, 16602069666338596455ULL, -1, 1))); // [!]
    TEST_OK((__test_string_to_uint<uint64_t, StringT>("-18446744073709551617", 10, 16602069666338596455ULL, -1, 1))); // [!]
    
    TEST_OK((__test_string_to_uint<uint64_t, StringT>( "FFFFFFFFFFFFFFFF", 16, 18446744073709551615ULL, 0, 0)));
    TEST_OK((__test_string_to_uint<uint64_t, StringT>("-FFFFFFFFFFFFFFFF", 16, 1, 0, 0)));
    TEST_OK((__test_string_to_uint<uint64_t, StringT>("-FFFFFFFFFFFFFFFE", 16, 2, 0, 0)));
    TEST_OK((__test_string_to_uint<uint64_t, StringT>("-FFFFFFFFFFFFFFF" , 16, 17293822569102704641ULL, 0, 0)));
    TEST_OK((__test_string_to_uint<uint64_t, StringT>("-FFFFFFFFFFFFFF"  , 16, 18374686479671623681ULL, 0, 0)));
    
    TEST_OK((__test_string_to_uint<uint64_t, StringT>( "10000000000000000", 16, 1152921504606846976ULL, 1, 1))); // [!!]

#endif
}

#endif /* TEST_STRING_TO_UINT_HPP */

