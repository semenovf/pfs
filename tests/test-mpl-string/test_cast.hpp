/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   test_cast.hpp
 * Author: wladt
 *
 * Created on January 25, 2017, 3:20 PM
 */

#ifndef TEST_CAST_HPP
#define TEST_CAST_HPP

#include <cstring>

template <typename Foundation, typename CharT>
void test_c_str_cast ()
{
    test_description<CharT>(__PRETTY_FUNCTION__);
    
    ADD_TESTS(7);
    
    typedef pfs::traits::string<Foundation, CharT> string;

    string orig(string_samples<CharT>(STR_ORIG));
    string same(string_samples<CharT>(STR_SAME));
    string diff(string_samples<CharT>(STR_DIFF));
    string empty(string_samples<CharT>(STR_EMPTY));
    
    TEST_OK(std::strcmp(string_samples<char>(STR_ORIG)
            , pfs::traits::c_str<Foundation, CharT>(orig)()) == 0);

    TEST_OK(std::strcmp(string_samples<char>(STR_SAME)
            , pfs::traits::c_str<Foundation, CharT>(orig)()) == 0);

    TEST_OK(std::strcmp(string_samples<char>(STR_DIFF)
            , pfs::traits::c_str<Foundation, CharT>(orig)()) != 0);

    TEST_OK(std::strcmp(pfs::traits::c_str<Foundation, CharT>(orig)()
            , pfs::traits::c_str<Foundation, CharT>(orig)()) == 0);

    TEST_OK(std::strcmp(pfs::traits::c_str<Foundation, CharT>(orig)()
            , pfs::traits::c_str<Foundation, CharT>(same)()) == 0);

    TEST_OK(std::strcmp(pfs::traits::c_str<Foundation, CharT>(orig)()
            , pfs::traits::c_str<Foundation, CharT>(same)()) == 0);

    TEST_OK(std::strcmp(pfs::traits::c_str<Foundation, CharT>(empty)()
            , "") == 0);
}

#endif /* TEST_CAST_HPP */
