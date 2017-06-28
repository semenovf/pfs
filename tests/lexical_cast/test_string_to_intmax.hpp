/* 
 * File:   test_string_to_intmax.hpp
 * Author: wladt
 *
 * Created on Jun 28, 2017
 */

#ifndef TEST_STRING_TO_INTMAX_HPP
#define TEST_STRING_TO_INTMAX_HPP

template <typename StringT>
void test_string_to_intmax ()
{
    ADD_TESTS(29);
    
    StringT zero_str("0");
    StringT min_intmax_str;
    StringT max_intmax_str;
    StringT intmax_underflow_str;
    StringT intmax_overflow_str;

    StringT hex_min_intmax_str;
    StringT hex_max_intmax_str;
    StringT hex_intmax_underflow_str;
    StringT hex_intmax_overflow_str;
    
    intmax_t min_intmax = 0;
    intmax_t max_intmax = 0;
    intmax_t min_intmax_cutoff = 0;
    intmax_t max_intmax_cutoff = 0;
    intmax_t result = 0;
    typename StringT::const_iterator badpos;
    int overflow = 0;
    
    if (sizeof(intmax_t) == 8) {
        min_intmax = -9223372036854775807LL - 1LL;
        max_intmax = 9223372036854775807LL;
        min_intmax_cutoff = -922337203685477580LL;
        max_intmax_cutoff = 922337203685477580LL;
        min_intmax_str = StringT("-9223372036854775808");
        max_intmax_str = StringT("9223372036854775807");
        intmax_underflow_str = StringT("-9223372036854775809");
        intmax_overflow_str = StringT("9223372036854775808");
        
        hex_min_intmax_str = StringT("0x8000000000000000");
        hex_max_intmax_str = StringT("0x7FFFFFFFFFFFFFFF");
        hex_intmax_underflow_str = StringT("0x80000000000000001");
        hex_intmax_overflow_str = StringT("0x7FFFFFFFFFFFFFFF1");
    } else if (sizeof(intmax_t) == 4) {
        min_intmax = -2147483648;
        max_intmax = 2147483647;
        min_intmax_str = StringT("-2147483648");
        max_intmax_str = StringT("2147483647");
    } else if (sizeof(intmax_t) == 2) {
        min_intmax = -2147483648;
        max_intmax = 2147483647;
        min_intmax_str = StringT("-32768");
        max_intmax_str = StringT("32767");
    } else {
        TEST_FAIL2(false, "sizeof(intmax_t) ?");
    }

    result = pfs::string_to_intmax(zero_str.cbegin()
        , zero_str.cend()
        , & badpos
        , 10
        , & overflow);
    
    TEST_OK(result == 0);
    TEST_OK(overflow == 0);
    TEST_OK(badpos == zero_str.cend());
    
    //
    // Decimal values
    //
    
    result = pfs::string_to_intmax(min_intmax_str.cbegin()
        , min_intmax_str.cend()
        , & badpos
        , 10
        , & overflow);
    
    TEST_OK(result == min_intmax);
    TEST_OK(overflow == 0);
    TEST_OK(badpos == min_intmax_str.cend());

    result = pfs::string_to_intmax(max_intmax_str.cbegin()
        , max_intmax_str.cend()
        , & badpos
        , 10
        , & overflow);
    
    TEST_OK(result == max_intmax);
    TEST_OK(overflow == 0);
    TEST_OK(badpos == max_intmax_str.cend());

    result = pfs::string_to_intmax(intmax_underflow_str.cbegin()
        , intmax_underflow_str.cend()
        , & badpos
        , 10
        , & overflow);
    
    TEST_OK(result == min_intmax_cutoff);
    TEST_OK(overflow < 0);
    TEST_OK(badpos + 1 == intmax_underflow_str.cend());
    
    result = pfs::string_to_intmax(intmax_overflow_str.cbegin()
        , intmax_overflow_str.cend()
        , & badpos
        , 10
        , & overflow);
    
    TEST_OK(result == max_intmax_cutoff);
    TEST_OK(overflow > 0);
    TEST_OK(badpos + 1 == intmax_overflow_str.cend());
    
    //
    // Hexadecimal values
    //
    
    result = pfs::string_to_intmax(hex_min_intmax_str.cbegin()
        , hex_min_intmax_str.cend()
        , & badpos
        , 16
        , & overflow);
    
    TEST_OK(result == min_intmax);
    TEST_OK(overflow == 0);
    TEST_OK(badpos == hex_min_intmax_str.cend());

    result = pfs::string_to_intmax(hex_max_intmax_str.cbegin()
        , hex_max_intmax_str.cend()
        , & badpos
        , 16
        , & overflow);
    
    TEST_OK(result == max_intmax);
    TEST_OK(overflow == 0);
    TEST_OK(badpos == hex_max_intmax_str.cend());

    result = pfs::string_to_intmax(hex_intmax_underflow_str.cbegin()
        , hex_intmax_underflow_str.cend()
        , & badpos
        , 16
        , & overflow);
    
    TEST_OK(result == min_intmax_cutoff);
    TEST_OK(overflow < 0);
    TEST_OK(badpos + 1 == hex_intmax_underflow_str.cend());
    
    result = pfs::string_to_intmax(hex_intmax_overflow_str.cbegin()
        , hex_intmax_overflow_str.cend()
        , & badpos
        , 16
        , & overflow);
    
    TEST_OK(result == max_intmax_cutoff);
    TEST_OK(overflow > 0);
    TEST_OK(badpos + 1 == hex_intmax_overflow_str.cend());    
    
}

#endif /* TEST_STRING_TO_INTMAX_HPP */

