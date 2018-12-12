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
IntT stdstrtoi (pfs::string const & str, pfs::string::pointer * endpos, int radix);

template <>
long int stdstrtoi<long int> (pfs::string const & str, pfs::string::pointer * endptr, int radix)
{
    long int result = std::strtol(str.c_str(), endptr, radix);
    return result;
}

template <>
unsigned long int stdstrtoi<unsigned long int> (pfs::string const & str, pfs::string::pointer * endptr, int radix)
{
    long int result = std::strtoul(str.c_str(), endptr, radix);
    return result;
}

template <typename IntT>
void compare (pfs::string const & str, int radix)
{
    pfs::string msg_prefix("***ERROR with string \"");
    msg_prefix += str;
    msg_prefix += "\" and radix(";
    msg_prefix += pfs::to_string(radix);
    msg_prefix += "): ";

    errno = 0;
    pfs::string::pointer endptr;
    IntT expected_result = stdstrtoi<IntT>(str, & endptr, radix);

    pfs::error_code ec;
    pfs::string::const_iterator endpos;
    IntT result = pfs::parse_integral_part<IntT>(str.cbegin()
                , str.cend()
                , ec
                , & endpos
                , radix);

    CHECK(result == expected_result);

    if (result != expected_result)
        std::cout << msg_prefix << "unexpected result" << std::endl;

    ptrdiff_t distance = pfs::distance(str.c_str()
            , static_cast<pfs::string::const_pointer>(endptr));
    pfs::string::const_iterator expected_endpos = str.cbegin();
    pfs::advance(expected_endpos, distance);

    CHECK(endpos == expected_endpos);

    if (endpos != expected_endpos)
        std::cout << msg_prefix << "end positions are different: "
                << pfs::distance(str.cbegin(), endpos) << " != " << distance << std::endl;

    if (errno) {
        if (errno == EINVAL)
            CHECK(ec == pfs::make_error_code(pfs::errc::invalid_argument));
        else if (errno == ERANGE)
            CHECK(ec == pfs::make_error_code(pfs::errc::result_out_of_range));
        else
            std::cout << msg_prefix << "unexpected errno: " << errno << std::endl;
    } else {
        if (ec) {
            // This situation may occure in case no conversion was performed
            // (no digits seen, and 0 returned).
            // Depends on implementation (see `man strtol`).
            CHECK(ec == pfs::make_error_code(pfs::errc::invalid_argument));

            if (ec != pfs::make_error_code(pfs::errc::invalid_argument))
                std::cout << msg_prefix << "unexpected error code: " << ec << std::endl;
        }
    }
}

template <typename IntT>
void run_generated_tests (int count)
{
    for (int radix = 2; radix <= 36; radix++) {
        for (int i = 0; i < count; i++) {
            pfs::string str = generate_integral_part(radix);
            compare<IntT>(str, radix);
        }
    }
}

// TEST_CASE("Tests on predefined sources") {
//     std::srand(std::time(0));
//
//     SECTION("signed char") {
//         test_data<signed char> data[] = {
//               {    "0", 10, 1, pfs::error_code(),    0 }
//             , {    "1", 10, 1, pfs::error_code(),    1 }
//             , {  "127", 10, 3, pfs::error_code(),  127 }
//             , { "+127", 10, 4, pfs::error_code(),  127 }
//             , { "-127", 10, 4, pfs::error_code(), -127 }
//             , { "-128", 10, 4, pfs::error_code(), -128 }
//         };
//
//         //run_synthetic_tests<signed char>(data, sizeof(data) / sizeof(data[0]));
//     }
//
//     SECTION("char") {
//         test_data<char> data[] = {
//               {    "0", 10, 1, pfs::error_code(),    0 }
//             , {    "1", 10, 1, pfs::error_code(),    1 }
//             , {  "127", 10, 3, pfs::error_code(),  127 }
//             , { "+127", 10, 4, pfs::error_code(),  127 }
//             , { "-127", 10, 4, pfs::error_code(), -127 }
//             , { "-128", 10, 4, pfs::error_code(), -128 }
//         };
//
//         //run_synthetic_tests<char>(data, sizeof(data) / sizeof(data[0]));
//     }
// }

TEST_CASE("Tests on generated sources") {
    compare<long int>(pfs::string("0"), 2);
    compare<long int>(pfs::string("+0"), 2);
    compare<long int>(pfs::string("-0"), 2);
    compare<long int>(pfs::string("1"), 2);
    compare<long int>(pfs::string("+1"), 2);
    compare<long int>(pfs::string("-1"), 2);
    compare<long int>(pfs::string("0"), 8);
    compare<long int>(pfs::string("0x0"), 0);
    compare<long int>(pfs::string("0x0"), 0);
    compare<long int>(pfs::string("0xf"), 0);
    compare<long int>(pfs::string("-0xf"), 0);
    compare<long int>(pfs::string("+0xf"), 0);

    compare<long int>(pfs::string(" +-"), 4);
    compare<long int>(pfs::string("--"), 4);
    compare<long int>(pfs::string("---"), 12);
    compare<long int>(pfs::string("---"), 15);
    compare<long int>(pfs::string("  -j1c0012168f25-"), 18);

    compare<unsigned long int>(pfs::string(" -692479728237--"), 10);
    compare<unsigned long int>(pfs::string("18446744073709551615"), 10);  // uint64 max value
    compare<unsigned long int>(pfs::string("18446744073709551616"), 10);  // uint64 max value + 1
    compare<unsigned long int>(pfs::string("-18446744073709551615"), 10); // uint64 max value * -1
    compare<unsigned long int>(pfs::string("-18446744073709551616"), 10); // (uint64 max value + 1) * -1

    run_generated_tests<long int>(10000);
    run_generated_tests<unsigned long int>(10000);
}
