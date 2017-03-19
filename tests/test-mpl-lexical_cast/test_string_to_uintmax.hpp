/* 
 * File:   test_string_to_uintmax.hpp
 * Author: wladt
 *
 * Created on March 19, 2017, 1:37 PM
 */

#ifndef TEST_STRING_TO_UINTMAX_HPP
#define TEST_STRING_TO_UINTMAX_HPP

template <typename StringT>
void test_string_to_uintmax ()
{
    ADD_TESTS(9);
    
    StringT zero_str("0");
    StringT max_uintmax_str;
    StringT max_uintmax_overflow_1_str;
    
    uintmax_t max_uintmax = 0;
    uintmax_t max_uintmax_cutoff = 0;
    uintmax_t result = 0;
    typename StringT::const_iterator badpos;
    int overflow = 0;
    
    if (sizeof(uintmax_t) == 8) {
        max_uintmax = 18446744073709551615ULL;
        max_uintmax_cutoff = 1844674407370955161ULL;
        max_uintmax_str = StringT("18446744073709551615");
        max_uintmax_overflow_1_str = StringT("18446744073709551616");
    } else if (sizeof(uintmax_t) == 4) {
        max_uintmax = 4294967295UL;
        max_uintmax_cutoff = 429496729UL;
        max_uintmax_str = StringT("4294967295");
        max_uintmax_overflow_1_str = StringT("4294967296");
    } else if (sizeof(uintmax_t) == 2) {
        max_uintmax = 65535;
        max_uintmax_cutoff = 6553;
        max_uintmax_str = StringT("65535");
        max_uintmax_overflow_1_str = StringT("65536");
    } else {
        TEST_FAIL2(false, "sizeof(uintmax_t) ?");
    }

    result = pfs::string_to_uintmax(zero_str.cbegin()
        , zero_str.cend()
        , & badpos
        , 10
        , & overflow);
    
    TEST_OK(result == 0);
    TEST_OK(overflow == 0);
    TEST_OK(badpos == zero_str.cend());
    
    result = pfs::string_to_uintmax(max_uintmax_str.cbegin()
        , max_uintmax_str.cend()
        , & badpos
        , 10
        , & overflow);
    
    TEST_OK(result == max_uintmax);
    TEST_OK(overflow == 0);
    TEST_OK(badpos == max_uintmax_str.cend());

    result = pfs::string_to_uintmax(max_uintmax_overflow_1_str.cbegin()
        , max_uintmax_overflow_1_str.cend()
        , & badpos
        , 10
        , & overflow);
    
    TEST_OK(result == max_uintmax_cutoff);
    TEST_OK(overflow > 0);
    TEST_OK(badpos + 1 == max_uintmax_overflow_1_str.cend());
}

#endif /* TEST_STRING_TO_UINTMAX_HPP */

