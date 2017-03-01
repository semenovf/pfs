/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   test_erase.hpp
 * Author: wladt
 *
 * Created on February 9, 2017, 1:23 PM
 */

#ifndef __PFS_TEST_ERASE_HPP__
#define __PFS_TEST_ERASE_HPP__

template <typename Foundation, typename CharT>
void test_erase ()
{
    test_description<CharT>(__PRETTY_FUNCTION__);
    
    ADD_TESTS(21);
    
    typedef pfs::traits::string<Foundation, CharT> string_type;

    {
        string_type s(string_samples<CharT>(STR_ORIG));

        s.erase(0, s.size());
        TEST_OK(s.empty());
    }

    {
        string_type orig(string_samples<CharT>(STR_ORIG));
        string_type s(string_samples<CharT>(STR_ORIG));

        s.erase(0, 1);
        TEST_OK(s.size() == orig.size() - 1);
        TEST_OK(s == orig.substr(1));
    }

    {
        string_type orig(string_samples<CharT>(STR_ORIG));
        string_type s(string_samples<CharT>(STR_ORIG));

        s.erase(1, 2);
        TEST_OK(s.size() == orig.size() - 2);
        TEST_OK(s[0] == orig[0]);
        TEST_OK(s[1] != orig[1]);
        TEST_OK(s[1] == orig[3]);
        TEST_OK(s[2] == orig[4]);
        TEST_OK(s[3] == orig[5]);
    }
    
    {
        string_type s(string_samples<CharT>(STR_ORIG));

        typename string_type::iterator it = s.erase(s.cbegin(), s.cend());
        
        TEST_OK(it == s.cbegin());
        TEST_OK(it == s.cend())
        TEST_OK(s.empty());
    }

    {
        string_type orig(string_samples<CharT>(STR_ORIG));
        string_type s(string_samples<CharT>(STR_ORIG));

        typename string_type::iterator it = s.erase(s.cbegin() + 1, s.cbegin() + 3);
        TEST_OK(s.size() == orig.size() - 2);
        TEST_OK(*it == orig[3]);
        TEST_OK(s[0] == orig[0]);
        TEST_OK(s[1] != orig[1]);
        TEST_OK(s[1] == orig[3]);
        TEST_OK(s[2] == orig[4]);
        TEST_OK(s[3] == orig[5]);
    }

    {
        string_type s(string_samples<CharT>(STR_ORIG));
        TEST_OK(!s.empty());
        
        s.clear();
        
        TEST_OK(s.empty());
    }
}

#endif /* __PFS_TEST_ERASE_HPP__ */
