#include <iostream>
#include <cstdarg>
#include <pfs/test.hpp>
#include <pfs/string.hpp>
#include <pfs/lexical_cast/parse_number.hpp>

static bool parse_digits_helper (char const * s
    , int radix
    , ptrdiff_t badpos_offset
    , int first_digit
    , ...)
{
    typedef pfs::string::const_iterator char_iterator_type;

    pfs::string str(s);
    pfs::vector<int> digits;
    char_iterator_type pos = pfs::parse_digits(str.cbegin()
            , str.cend()
            , radix
            , digits);

    ptrdiff_t pos_offset = pfs::distance(str.cbegin(), pos);

    if (badpos_offset != pos_offset) {
        std::cout << "***ERROR: badpos_offset (" << badpos_offset << ")"
                << " is not match result position ("
                << pos_offset
                << ") after parsing" << std::endl;
        return false;
    }

    va_list ap;
    va_start(ap, first_digit);

    int digit = first_digit;

    pfs::vector<int>::const_iterator first = digits.cbegin();
    pfs::vector<int>::const_iterator last = digits.cend();

    while (digit != 0xFF && first != last) {
        if (*first != digit) {
            std::cout << "***ERROR: digits are different at position "
                    << pfs::distance(digits.cbegin(), first)
                    << ": result is " << *first
                    << ", but expected " << digit << std::endl;
            va_end(ap);
            return false;
        }

        digit = va_arg(ap, int);
        ++first;
    }

    va_end(ap);

    return true;
}

void test_parse_digits ()
{
    ADD_TESTS(5);

    TEST_OK(parse_digits_helper("0", 10, 1, 0, 0xFF));
    TEST_OK(parse_digits_helper("1", 10, 1, 1, 0xFF));
    TEST_OK(parse_digits_helper("2147483647", 10, 10, 2, 1, 4, 7, 4, 8, 3, 6, 4, 7, 0xFF));
    TEST_OK(parse_digits_helper("01010101", 2, 8, 0, 1, 0, 1, 0, 1, 0, 1, 0xFF));
    TEST_EX(pfs::range_error, parse_digits_helper("", 1, 0, 0xFF));
}

