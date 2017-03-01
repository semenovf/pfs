/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   test_substr.hpp
 * Author: wladt
 *
 * Created on January 16, 2017, 10:42 AM
 */

#ifndef __PFS_TEST_SUBSTR_HPP__
#define __PFS_TEST_SUBSTR_HPP__

template <typename Foundation, typename CharT>
void test_substr ()
{
    test_description<CharT>(__PRETTY_FUNCTION__);
    
    ADD_TESTS(7);
    
    typedef pfs::traits::string<Foundation, CharT> string_type;
    
    string_type orig(string_samples<CharT>(STR_ORIG));
    string_type same(string_samples<CharT>(STR_SAME));
    string_type head(string_samples<CharT>(STR_HEAD));
    string_type tail(string_samples<CharT>(STR_TAIL));
    string_type mid(string_samples<CharT>(STR_MID));
    
    TEST_OK(orig.substr(0) == orig);
    TEST_OK(orig.substr(0) == same);
    TEST_OK(orig.substr(1, 4) == mid);
    TEST_OK(orig.left(head.size()) == head);
    TEST_OK(orig.right(tail.size()) == tail);
    TEST_OK(orig.substr(1) == tail);
    TEST_OK(orig.substr(1, 100) == tail);
}

#endif /* __PFS_TEST_SUBSTR_HPP__ */
