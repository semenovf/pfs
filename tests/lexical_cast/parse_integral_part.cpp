#include <iostream>
#include <pfs/test.hpp>
#include <pfs/string.hpp>
#include <pfs/lexical_cast/parse_number.hpp>

template <typename IntT>
static bool parse_integral_part_helper (char const * s
    , int radix
    , ptrdiff_t endpos_offset
    , pfs::error_code expected_ec
    , IntT expected_result)
{
    typedef pfs::string::const_iterator char_iterator_type;

    pfs::string str(s);
    pfs::error_code ec;
    char_iterator_type endpos;
    IntT result = pfs::parse_integral_part<IntT>(str.cbegin()
            , str.cend()
            , & endpos
            , radix
            , ec);

    if (result != expected_result) {
        std::cout << "***ERROR: result not match: "
                // multiplcation to 1 need to output char type
                // as number not as symbol
                << result * 1 << " != " << expected_result * 1
                << std::endl;
        return false;
    }

    if (ec != expected_ec) {
        std::cout << "***ERROR: error code not match: "
                << pfs::to_string(ec) << " != " << pfs::to_string(expected_ec)
                << std::endl;
        return false;
    }

    ptrdiff_t pos_offset = pfs::distance(str.cbegin(), endpos);

    if (pos_offset != endpos_offset) {
        std::cout << "***ERROR: endpos offset not match: "
                << pos_offset << " != " << endpos_offset << std::endl;
        return false;
    }

    return true;
}

void test_parse_integral_part ()
{
    ADD_TESTS(6);

    TEST_OK(parse_integral_part_helper<signed char>("0", 10, 1, pfs::error_code(), 0));
    TEST_OK(parse_integral_part_helper<signed char>("1", 10, 1, pfs::error_code(), 1));
    TEST_OK(parse_integral_part_helper<signed char>("127", 10, 3, pfs::error_code(), 127));
    TEST_OK(parse_integral_part_helper<signed char>("+127", 10, 4, pfs::error_code(), 127));
    TEST_OK(parse_integral_part_helper<signed char>("-127", 10, 4, pfs::error_code(), -127));
    TEST_OK(parse_integral_part_helper<signed char>("-128", 10, 4, pfs::error_code(), -128));
}


