#pragma once

template <typename StringT>
void test_trim ()
{
    std::cout << "///////////////////////////////////////////////////////////////////////////\n";
    std::cout << "//                           Trim functions                              //\n";
    std::cout << "///////////////////////////////////////////////////////////////////////////\n";

    ADD_TESTS(30);

    //
    // Default constructor
    //
    StringT str_empty;
    StringT str_no_spaces("АБВ");
    StringT str_left_spaces("   АБВ");
    StringT str_right_spaces("АБВ   ");
    StringT str_both_spaces("   АБВ   ");

    StringT result;

    //
    // template <typename InputIt, typename OutputIt>
    // void ltrim (InputIt first, InputIt last, OutputIt & out)
    //
    result.clear();
    pfs::ltrim(str_empty.cbegin(), str_empty.cend(), pfs::back_inserter(result));
    TEST_OK2(result == str_empty, "Trim leading spaces for empty string");

    result.clear();
    pfs::ltrim(str_no_spaces.cbegin(), str_no_spaces.cend(), pfs::back_inserter(result));
    TEST_OK2(result == str_no_spaces, "Trim leading spaces for string w/o leading and trailing spaces");

    result.clear();
    pfs::ltrim(str_left_spaces.cbegin(), str_left_spaces.cend(), pfs::back_inserter(result));
    TEST_OK2(result == str_no_spaces, "Trim leading spaces for string w/ leading spaces");

    result.clear();
    pfs::ltrim(str_right_spaces.cbegin(), str_right_spaces.cend(), pfs::back_inserter(result));
    TEST_OK2(result == str_right_spaces, "Trim leading spaces for string w/ trailing spaces");

    result.clear();
    pfs::ltrim(str_both_spaces.cbegin(), str_both_spaces.cend(), pfs::back_inserter(result));
    TEST_OK2(result == str_right_spaces, "Trim leading spaces for string w/ leading and trailing spaces");

    //
    // template <typename InputIt, typename OutputIt>
    // void rtrim (InputIt first, InputIt last, OutputIt out)
    //
    result.clear();
    pfs::rtrim(str_empty.cbegin(), str_empty.cend(), pfs::back_inserter(result));
    TEST_OK2(result == str_empty, "Trim trailing spaces for empty string");

    result.clear();
    pfs::rtrim(str_no_spaces.cbegin(), str_no_spaces.cend(), pfs::back_inserter(result));
    TEST_OK2(result == str_no_spaces, "Trim trailing spaces for string w/o leading and trailing spaces");

    result.clear();
    pfs::rtrim(str_left_spaces.cbegin(), str_left_spaces.cend(), pfs::back_inserter(result));
    TEST_OK2(result == str_left_spaces, "Trim trailing spaces for string w/ leading spaces");

    result.clear();
    pfs::rtrim(str_right_spaces.cbegin(), str_right_spaces.cend(), pfs::back_inserter(result));
    TEST_OK2(result == str_no_spaces, "Trim trailing spaces for string w/ trailing spaces");

    result.clear();
    pfs::rtrim(str_both_spaces.cbegin(), str_both_spaces.cend(), pfs::back_inserter(result));
    TEST_OK2(result == str_left_spaces, "Trim trailing spaces for string w/ leading and trailing spaces");

    //
    // template <typename InputIt, typename OutputIt>
    // void trim (InputIt first, InputIt last, OutputIt out)
    //
    result.clear();
    pfs::trim(str_empty.cbegin(), str_empty.cend(), pfs::back_inserter(result));
    TEST_OK2(result == str_empty, "Trim leading and trailing spaces for empty string");

    result.clear();
    pfs::trim(str_no_spaces.cbegin(), str_no_spaces.cend(), pfs::back_inserter(result));
    TEST_OK2(result == str_no_spaces, "Trim leading and trailing spaces for string w/o leading and trailing spaces");

    result.clear();
    pfs::trim(str_left_spaces.cbegin(), str_left_spaces.cend(), pfs::back_inserter(result));
    TEST_OK2(result == str_no_spaces, "Trim leading and trailing spaces for string w/ leading spaces");

    result.clear();
    pfs::trim(str_right_spaces.cbegin(), str_right_spaces.cend(), pfs::back_inserter(result));
    TEST_OK2(result == str_no_spaces, "Trim leading and trailing spaces for string w/ trailing spaces");

    result.clear();
    pfs::trim(str_both_spaces.cbegin(), str_both_spaces.cend(), pfs::back_inserter(result));
    TEST_OK2(result == str_no_spaces, "Trim leading and trailing spaces for string w/ leading and trailing spaces");

    //
    // string ltrim (string const & s)
    //
    result.clear();
    TEST_OK2(pfs::ltrim(str_empty) == str_empty, "Trim leading spaces for empty string");

    result.clear();
    TEST_OK2(pfs::ltrim(str_no_spaces) == str_no_spaces, "Trim leading spaces for string w/o leading and trailing spaces");

    result.clear();
    TEST_OK2(pfs::ltrim(str_left_spaces) == str_no_spaces, "Trim leading spaces for string w/ leading spaces");

    result.clear();
    TEST_OK2(pfs::ltrim(str_right_spaces) == str_right_spaces, "Trim leading spaces for string w/ trailing spaces");

    result.clear();
    TEST_OK2(pfs::ltrim(str_both_spaces) == str_right_spaces, "Trim leading spaces for string w/ leading and trailing spaces");

    //
    // string rtrim (string const & s)
    //
    result.clear();
    TEST_OK2(pfs::rtrim(str_empty) == str_empty, "Trim trailing spaces for empty string");

    result.clear();
    TEST_OK2(pfs::rtrim(str_no_spaces) == str_no_spaces, "Trim trailing spaces for string w/o leading and trailing spaces");

    result.clear();
    TEST_OK2(pfs::rtrim(str_left_spaces) == str_left_spaces, "Trim trailing spaces for string w/ leading spaces");

    result.clear();
    TEST_OK2(pfs::rtrim(str_right_spaces) == str_no_spaces, "Trim trailing spaces for string w/ trailing spaces");

    result.clear();
    TEST_OK2(pfs::rtrim(str_both_spaces) == str_left_spaces, "Trim trailing spaces for string w/ leading and trailing spaces");

    //
    // string trim (string const & s)
    //
    result.clear();
    TEST_OK2(pfs::trim(str_empty) == str_empty, "Trim leading and trailing spaces for empty string");

    result.clear();
    TEST_OK2(pfs::trim(str_no_spaces) == str_no_spaces, "Trim leading and trailing spaces for string w/o leading and trailing spaces");

    result.clear();
    TEST_OK2(pfs::trim(str_left_spaces) == str_no_spaces, "Trim leading and trailing spaces for string w/ leading spaces");

    result.clear();
    TEST_OK2(pfs::trim(str_right_spaces) == str_no_spaces, "Trim leading and trailing spaces for string w/ trailing spaces");

    result.clear();
    TEST_OK2(pfs::trim(str_both_spaces) == str_no_spaces, "Trim leading and trailing spaces for string w/ leading and trailing spaces");
}

