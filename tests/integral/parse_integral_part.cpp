#include <cstdlib> // for random, strtol
#include <ctime>
#include <pfs/string.hpp>
#include <pfs/integral.hpp>
#include "../catch.hpp"

#include <pfs/debug.hpp>

typedef pfs::string::const_iterator char_iterator_type;

//
// Randomly generate test string in format:
//
// *3ws [ '+' / '-' ] ?nonzero_digit *32digit *3'-'
//
pfs::string generate_integral_part (int radix)
{
    char const * nonzerodigits = "123456789abcdefjhijklmnopqrstuvwxyz";
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

//     int nonzerodigit_index = std::rand() % (radix - 1);
//     result.push_back(digits[nonzerodigit_index]);

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
IntT stdtoi (pfs::string const & str, pfs::string::const_iterator * endpos, int radix);

template <>
long int stdtoi<long int> (pfs::string const & str, pfs::string::const_iterator * endpos, int radix)
{
    char * endptr;
    long int result = std::strtol(str.c_str(), & endptr, radix);
    *endpos = pfs::string::const_iterator(endptr);
    return result;
}

template <typename IntT>
void compare (pfs::string const & str, int radix)
{
    pfs::string::const_iterator expected_endpos;
    IntT expected_result = stdtoi<IntT>(str.c_str(), & expected_endpos, radix);

    pfs::error_code ec;
    pfs::string::const_iterator endpos;
    IntT result = pfs::parse_integral_part<IntT>(str.cbegin()
                , str.cend()
                , ec
                , & endpos
                , radix);

    CHECK(expected_result == result);

    if (expected_result != result) {
        std::cout << "***Error with string \"" << str
                << "\" and radix = " << radix << std::endl;
    }
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
void run_synthetic_tests (test_data<IntT> * data, int count)
{
    for (int i = 0; i < count; i++) {
        pfs::string str(data[i].s);
        pfs::error_code ec;
        char_iterator_type endpos;

        IntT result = pfs::parse_integral_part<IntT>(str.cbegin()
                , str.cend()
                , ec
                , & endpos
                , data[i].radix);

        // multiplcation to 1 need to output char type
        // as number not as symbol
        CHECK(result * 1 == data[i].expected_result * 1);
        CHECK(ec == data[i].expected_ec);
        CHECK(pfs::distance(str.cbegin(), endpos) == data[i].endpos_offset);
    }
}

template <typename IntT>
void run_generated_tests (int count)
{
    for (int radix = 2; radix <= 36; radix++) {
        for (int i = 0; i < count; i++) {
            pfs::string str = generate_integral_part(radix);
            compare<IntT>(str, radix);
//             pfs::error_code ec;
//             char_iterator_type endpos;
//
//             IntT result = pfs::parse_integral_part<IntT>(s.cbegin()
//                     , s.cend()
//                     , ec
//                     , & endpos
//                     , radix);
//
//             std::cout << "source=\"" << s << '"'
//                     << " result=[" << 1 * result << ']'
//                     << " radix=" << radix
//                     << " endpos=" << pfs::distance(s.cbegin(), endpos)
//                     << " error=" << '"' <<  pfs::to_string(ec) << '"'
//                     << std::endl;
        }
    }
}

TEST_CASE("Tests on predefined sources") {
    std::srand(std::time(0));

    SECTION("signed char") {
        test_data<signed char> data[] = {
              {    "0", 10, 1, pfs::error_code(),    0 }
            , {    "1", 10, 1, pfs::error_code(),    1 }
            , {  "127", 10, 3, pfs::error_code(),  127 }
            , { "+127", 10, 4, pfs::error_code(),  127 }
            , { "-127", 10, 4, pfs::error_code(), -127 }
            , { "-128", 10, 4, pfs::error_code(), -128 }
        };

        run_synthetic_tests<signed char>(data, sizeof(data) / sizeof(data[0]));
    }

    SECTION("char") {
        test_data<char> data[] = {
              {    "0", 10, 1, pfs::error_code(),    0 }
            , {    "1", 10, 1, pfs::error_code(),    1 }
            , {  "127", 10, 3, pfs::error_code(),  127 }
            , { "+127", 10, 4, pfs::error_code(),  127 }
            , { "-127", 10, 4, pfs::error_code(), -127 }
            , { "-128", 10, 4, pfs::error_code(), -128 }
        };

        run_synthetic_tests<char>(data, sizeof(data) / sizeof(data[0]));
    }
}

TEST_CASE("Tests on generated sources") {
//     run_generated_tests<signed char>(10);
//     run_generated_tests<char>(10);
//     run_generated_tests<long>(10);


    //run_generated_tests<long int>(10);

    //compare<long int>(pfs::string("-1100001000111001--"), 2);
    compare<long int>(pfs::string("-1100001000--"), 2);
    //compare<long int>(pfs::string("1100001000--"), 2);
}
