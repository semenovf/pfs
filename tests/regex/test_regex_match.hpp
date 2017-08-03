/* 
 * File:   test_regex_match.hpp
 * Author: wladt
 *
 * Created on July 4, 2017
 */

#ifndef TEST_REGEX_MATCH_HPP
#define TEST_REGEX_MATCH_HPP

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
}

#endif /* TEST_REGEX_MATCH_HPP */

