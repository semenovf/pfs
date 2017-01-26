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

template <typename StringImpl>
void test_c_str_cast ()
{
    test_description<StringImpl>(__PRETTY_FUNCTION__);
    
    ADD_TESTS(7);
    
    typedef pfs::traits::string<StringImpl> string;

    string orig(string_samples<StringImpl>(STR_ORIG));
    string same(string_samples<StringImpl>(STR_SAME));
    string diff(string_samples<StringImpl>(STR_DIFF));
    string empty(string_samples<StringImpl>(STR_EMPTY));
    
    TEST_OK(std::strcmp(string_samples<char const *>(STR_ORIG)
            , pfs::traits::c_str<StringImpl>(orig)()) == 0);

    TEST_OK(std::strcmp(string_samples<char const *>(STR_SAME)
            , pfs::traits::c_str<StringImpl>(orig)()) == 0);

    TEST_OK(std::strcmp(string_samples<char const *>(STR_DIFF)
            , pfs::traits::c_str<StringImpl>(orig)()) != 0);

    TEST_OK(std::strcmp(pfs::traits::c_str<StringImpl>(orig)()
            , pfs::traits::c_str<StringImpl>(orig)()) == 0);

    TEST_OK(std::strcmp(pfs::traits::c_str<StringImpl>(orig)()
            , pfs::traits::c_str<StringImpl>(same)()) == 0);

    TEST_OK(std::strcmp(pfs::traits::c_str<StringImpl>(orig)()
            , pfs::traits::c_str<StringImpl>(same)()) == 0);

    TEST_OK(std::strcmp(pfs::traits::c_str<StringImpl>(empty)()
            , "") == 0);
}

#endif /* TEST_CAST_HPP */

