/* 
 * File:   test_string_to_intmax.hpp
 * Author: wladt
 *
 * Created on Jun 28, 2017
 */

#ifndef TEST_STRING_TO_INTMAX_HPP
#define TEST_STRING_TO_INTMAX_HPP

#include <iostream>
#include <cstdlib> // strtol, strtoll
#include <cerrno>

template <typename IntmaxT>
IntmaxT std_string_to_intmax (char const * nptr
        , char ** endptr
        , int base);

template <>
inline int32_t std_string_to_intmax<int32_t> (char const * nptr
        , char ** endptr
        , int base)
{
    errno = 0;
    return strtol(nptr, endptr, base);
}

#if PFS_HAVE_INT64

template <>
inline int64_t std_string_to_intmax<int64_t> (char const * nptr
        , char ** endptr
        , int base)
{
    errno = 0;
    return strtoll(nptr, endptr, base);
}

#endif

template <typename IntmaxT, typename StringT>
static bool __test_string_to_intmax (char const * s, int radix
    , IntmaxT result_sample
    , int overflow_sample
    , ptrdiff_t badpos_increment)
{
    typedef typename StringT::const_iterator char_iterator_type;
    
    bool r = true;
    char_iterator_type badpos;
    int overflow = 0;
    StringT str(s);
    
    IntmaxT result = pfs::string_to_intmax<IntmaxT, char_iterator_type>(str.cbegin(), str.cend()
        , & badpos
        , radix
        , & overflow);
    
    IntmaxT std_result = std_string_to_intmax<IntmaxT>(s, 0, radix);
    
    if (result != std_result) {
        // Ignore result with overflow
        if (!(errno == ERANGE /*&& (std_result == 0 
                || std_result == pfs::numeric_limits<IntmaxT>::max())*/)) {
            std::cout << "***ERROR: result = " << result << " does not math result of strtoul[l](): " << std_result << std::endl;
            r = false;
        }
    }
    
    if (result != result_sample) {
        std::cout << "***ERROR: result = " << result << ", but expected " << result_sample << std::endl;
        r = false;
    }
    
    if (overflow != overflow_sample) {
        std::cout << "***ERROR: overflow = " << overflow << ", but expected " << overflow_sample << std::endl;
        r = false;
    }
    
    if (badpos + badpos_increment != str.cend()) {
        std::cout << "***ERROR: badpos does not match" << std::endl;
        r = false;
    }
    
    return r;
}

template <typename StringT>
void test_string_to_intmax ()
{
    ADD_TESTS(3);
    
    
    
    TEST_OK((__test_string_to_intmax<int32_t, StringT>("0", 10, 0, 0, 0)));
    
    TEST_OK((__test_string_to_intmax<int32_t, StringT>( "2147483647", 10,  2147483647L, 0, 0)));
    TEST_OK((__test_string_to_intmax<int32_t, StringT>("-2147483648", 10, -2147483648L, 0, 0)));
    
    TEST_OK((__test_string_to_intmax<int32_t, StringT>( "2147483648", 10,  214748364L, 1, 1)));
    TEST_OK((__test_string_to_intmax<int32_t, StringT>("-2147483649", 10, -214748364L, -1, 1)));

    TEST_OK((__test_string_to_intmax<int32_t, StringT>( "7FFFFFFF", 16,  2147483647L, 0, 0)));
    TEST_OK((__test_string_to_intmax<int32_t, StringT>( "80000000", 16, -2147483648L, 0, 0)));
    TEST_OK((__test_string_to_intmax<int32_t, StringT>( "FFFFFFFF", 16, -1L, 0, 0)));
    
#if PFS_HAVE_INT64
    ADD_TESTS(2);
    
    TEST_OK((__test_string_to_intmax<int64_t, StringT>( "9223372036854775807", 10,  9223372036854775807LL, 0, 0)));
    TEST_OK((__test_string_to_intmax<int64_t, StringT>("-9223372036854775808", 10, -9223372036854775807LL - 1LL, 0, 0)));

    TEST_OK((__test_string_to_intmax<int64_t, StringT>( "9223372036854775808", 10,  922337203685477580LL, 1, 1)));
    TEST_OK((__test_string_to_intmax<int64_t, StringT>("-9223372036854775809", 10, -922337203685477580LL, -1, 1)));
#endif
//        hex_min_intmax_str = StringT("0x8000000000000000");
//        hex_max_intmax_str = StringT("0x7FFFFFFFFFFFFFFF");
//        hex_intmax_underflow_str = StringT("0x80000000000000001");
//        hex_intmax_overflow_str = StringT("0x7FFFFFFFFFFFFFFF1");
//        min_intmax_str = StringT("-32768");
//        max_intmax_str = StringT("32767");
}

#endif /* TEST_STRING_TO_INTMAX_HPP */

