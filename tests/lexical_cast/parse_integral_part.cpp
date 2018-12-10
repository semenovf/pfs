#include <pfs/string.hpp>
#include <pfs/lexical_cast/parse_number.hpp>
#include "../catch.hpp"

template <typename IntT>
struct test_data {
    char const *    s;
    int             radix;
    ptrdiff_t       endpos_offset;
    pfs::error_code expected_ec;
    IntT            expected_result;
};

template <typename IntT>
void run_tests (test_data<IntT> * data, int count)
{
    typedef pfs::string::const_iterator char_iterator_type;

    for (int i = 0; i < count; i++) {
        pfs::string str(data[i].s);
        pfs::error_code ec;
        char_iterator_type endpos;

        IntT result = pfs::parse_integral_part<IntT>(str.cbegin()
                , str.cend()
                , & endpos
                , data[i].radix
                , ec);

        // multiplcation to 1 need to output char type
        // as number not as symbol
        CHECK(result * 1 == data[i].expected_result * 1);
        CHECK(ec == data[i].expected_ec);
        CHECK(pfs::distance(str.cbegin(), endpos) == data[i].endpos_offset);
    }
}

TEST_CASE("Parse integral part") {
    SECTION("signed char") {
        test_data<signed char> data[] = {
              { "0", 10, 1, pfs::error_code(), 0 }
            , { "1", 10, 1, pfs::error_code(), 1 }
            , { "127", 10, 3, pfs::error_code(), 127 }
            , { "+127", 10, 4, pfs::error_code(), 127 }
            , { "-127", 10, 4, pfs::error_code(), -127 }
            , { "-128", 10, 4, pfs::error_code(), -128 }
        };

        run_tests<signed char>(data, sizeof(data) / sizeof(data[0]));
    }
}
