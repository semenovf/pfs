/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   test_compare.hpp
 * Author: wladt
 *
 * Created on January 10, 2017, 10:30 AM
 */

#ifndef __PFS_TEST_COMPARE_HPP__
#define __PFS_TEST_COMPARE_HPP__

template <typename Foundation, typename CharT>
void test_compare_basic ()
{
    test_description<CharT>(__PRETTY_FUNCTION__);
    
    ADD_TESTS(10);
    
    typedef pfs::traits::string<Foundation, CharT> string;
    string orig(string_samples<CharT>(STR_ORIG));
    string same(string_samples<CharT>(STR_SAME));
    string diff(string_samples<CharT>(STR_DIFF));
    string less(string_samples<CharT>(STR_LESS));
    string greater(string_samples<CharT>(STR_GREATER));
    string head(string_samples<CharT>(STR_HEAD));
    string tail(string_samples<CharT>(STR_TAIL));

    TEST_OK(orig == orig);
    TEST_OK(orig == same);
    TEST_OK(orig != diff);
    TEST_OK(orig > less);
    TEST_OK(orig >= less);
    TEST_OK(orig < greater);
    TEST_OK(orig <= greater);

    TEST_OK(orig.compare(same) == 0);
    TEST_OK(orig.starts_with(head));
    TEST_OK(orig.ends_with(tail));
}

struct test_compare_empty_data
{
    size_t pos1;
    size_t count1;
    size_t pos2;
    size_t count2;
    int    result;
};

template <typename Foundation, typename CharT>
void test_compare_empty ()
{
    test_description<CharT>(__PRETTY_FUNCTION__);
    
    ADD_TESTS(1);

    typedef pfs::traits::string<Foundation, CharT> string;
    string s1(string_samples<CharT>(STR_ORIG));
    string s2(string_samples<CharT>(STR_SAME));
    
    typename string::size_type n1 = s1.size();
    typename string::size_type n2 = s2.size();
    
    test_compare_empty_data t[] = {
          {  0, 0,  0,  0, 0 }
        , {  0, 0, n2,  0, 0 }
        , {  0, 0, n2,  1, 0 }
        , { n1, 0,  0,  0, 0 }
        , { n1, 1,  0,  0, 0 }
        , { n1, 0, n2,  0, 0 }
        , { n1, 0, n2,  1, 0 }
        , { n1, 1, n2,  0, 0 }
        , { n1, 1, n2,  1, 0 }
    };
    
    size_t count = sizeof(t)/sizeof(t[0]);
    size_t i = 0;
    
    for (; i < count; ++i) {
        if (s1.compare(t[i].pos1, t[i].count1, s2, t[i].pos2, t[i].count2) != t[i].result) {
            std::cerr << "Compare strings implemented using <" 
                    << stringify_string_impl<CharT>()
                    << ">: failed with index: " << i << std::endl;
            break;
        }
    }

    std::ostringstream ss;
    ss << "Compare strings implemented using <" 
            << stringify_string_impl<CharT>() 
            << ">";
    
    TEST_OK2(i == count, ss.str().c_str());
}

template <typename Foundation, typename CharT>
void test_compare ()
{
    test_compare_basic<Foundation, CharT>();
    test_compare_empty<Foundation, CharT>();
}

template <typename Foundation, typename CharT>
void test_compare_cstr ()
{
    test_description<CharT>(__PRETTY_FUNCTION__);
    
    ADD_TESTS(16);
    
    typedef pfs::traits::string<Foundation, CharT> string;
    string orig(string_samples<CharT>(STR_ORIG));
    
    TEST_OK(orig == string_samples<char>(STR_ORIG));
    TEST_OK(orig >= string_samples<char>(STR_ORIG));
    TEST_OK(orig <= string_samples<char>(STR_ORIG));
    
    TEST_OK(string_samples<char>(STR_ORIG) == orig);
    TEST_OK(string_samples<char>(STR_ORIG) >= orig);
    TEST_OK(string_samples<char>(STR_ORIG) <= orig);
    
    TEST_OK(orig != string_samples<char>(STR_DIFF));
    TEST_OK(string_samples<char>(STR_DIFF) != orig);
    
    TEST_OK(orig > string_samples<char>(STR_LESS));
    TEST_OK(string_samples<char>(STR_LESS) < orig);
    TEST_OK(orig >= string_samples<char>(STR_LESS));
    TEST_OK(string_samples<char>(STR_LESS) <= orig);
    
    TEST_OK(orig < string_samples<char>(STR_GREATER));
    TEST_OK(string_samples<char>(STR_GREATER) > orig);
    TEST_OK(orig <= string_samples<char>(STR_GREATER));
    TEST_OK(string_samples<char>(STR_GREATER) >= orig);
}


#endif /* __PFS_TEST_COMPARE_HPP__ */

