#include <cstdlib> // for random
#include <ctime>
#include <pfs/string.hpp>
#include <pfs/integral.hpp>
#include "../catch.hpp"

#include <pfs/debug.hpp>

typedef pfs::string::const_iterator char_iterator_type;

pfs::string generate_integral_part (int radix)
{
    char const * digits = "0123456789abcdefjhijklmnopqrstuvwxyz";
    pfs::string result;

    // Whitespaces (0-3)
    int ws_count = std::rand() % 3;

    for (int i = 0; i < ws_count; i++)
        result.push_back(' ');

    // Sign
    char sign[] = {'?', '+', '-'};
    int sign_index = std::rand() % 3;

    if (sign_index > 0)
        result.push_back(sign[sign_index]);

    int digit_count = std::rand() % 32;

    for (int i = 0; i < digit_count; i++) {
        int digit_index = std::rand() % radix;
        result.push_back(digits[digit_index]);
    }

    int tail_count = std::rand() % 3;

    for (int i = 0; i < tail_count; i++)
        result.push_back('-');

    return result;
}

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
    std::srand(std::time(0));

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

    typedef signed char IntType;

    for (int radix = 2; radix <= 36; radix++) {
        for (int i = 0; i < 10; i++) {
            pfs::string s = generate_integral_part(radix);
            pfs::error_code ec;
            char_iterator_type endpos;

            IntType result = pfs::parse_integral_part<IntType>(s.cbegin()
                    , s.cend()
                    , & endpos
                    , radix
                    , ec);

            std::cout << "source=\"" << s << '"'
                    << " result=[" << 1 * result << ']'
                    << " radix=" << radix
                    << " endpos=" << pfs::distance(s.cbegin(), endpos)
                    << " error=" << '"' <<  pfs::to_string(ec) << '"'
                    << std::endl;
        }
    }
}
