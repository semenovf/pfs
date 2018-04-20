#pragma once
#include <pfs/regex.hpp>
#include <iostream>

template <typename StringType>
void test_regex_match ()
{
    typedef StringType                     string_type;
    typedef pfs::basic_regex<string_type>  regex;
    typedef typename regex::match_results  match;

    {
        ADD_TESTS(5);

        regex re("a(a)*b");
        char const * target = "aaab";
        match m;

        TEST_OK(m.size() == 0);
        TEST_OK(pfs::regex_match(target, m, re));
        TEST_OK(m.size() == 2);
        TEST_OK(m[0] == target);
        TEST_OK(m[1] == "a");
    }

    {
        ADD_TESTS(5);

        regex re("a(a)*b");
        string_type target("aaab");
        match m;

        TEST_OK(m.size() == 0);
        TEST_OK(pfs::regex_match(target, m, re));
        TEST_OK(m.size() == 2);
        TEST_OK(m[0].str() == target);
        TEST_OK(m[1] == "a");
    }

    {
        ADD_TESTS(16);
        std::cout << "START: regex re(...)" << std::endl;
        regex re("\\[([^\\?]*)\\?(\\d+)\\&ДЕЁ(\\d\\d\\d)\\.(\\d\\d)\\]\\s*(..)\\s*\\[([^\\]]*)\\]");
        std::cout << "END: regex re(...)" << std::endl;
        string_type target("[local://АБВГ?0000000001&ДЕЁ092.56]<<  [00 0C 5C 03 09 00 02 00 00 00 00 00 46 CD 00 00 00 00]");

        match m;

        TEST_OK(m.size() == 0);
        TEST_OK(pfs::regex_match(target, m, re));
        TEST_OK(m.size() == 7);
        TEST_OK(m[0].str() == target);
        TEST_OK(m[1] == "local://АБВГ");
        TEST_OK(m[2] == "0000000001");
        TEST_OK(m[3] == "092");
        TEST_OK(m[4] == "56");
        TEST_OK(m[5] == "<<");
        TEST_OK(m[6] == "00 0C 5C 03 09 00 02 00 00 00 00 00 46 CD 00 00 00 00");

        match m1;
        TEST_OK(pfs::regex_match(target.cbegin(), target.cend(), m1, re));
        TEST_OK(m1.size() == 7);
        TEST_OK(m1[0].str() == target);

        match m2;
        string_type re_str("\\[([^\\?]*)\\?(\\d+)\\&ДЕЁ(\\d\\d\\d)\\.(\\d\\d)\\]\\s*(..)\\s*\\[([^\\]]*)\\]");
        regex re2(re_str.cbegin(), re_str.cend());
        TEST_OK(pfs::regex_match(target.cbegin(), target.cend(), m2, re2));
        TEST_OK(m2.size() == 7);
        TEST_OK(m2[0].str() == target);
    }
}
