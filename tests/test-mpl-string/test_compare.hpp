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

template <typename StringImpl>
void test_compare_basic ()
{
    test_description<StringImpl>(__PRETTY_FUNCTION__);
    
    ADD_TESTS(3);
    
    typedef pfs::traits::string<StringImpl> string;
    string orig(string_samples<StringImpl>(STR_ORIG));
    string same(string_samples<StringImpl>(STR_SAME));
    string head(string_samples<StringImpl>(STR_HEAD));
    string tail(string_samples<StringImpl>(STR_TAIL));
    
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

template <typename StringImpl>
void test_compare_empty ()
{
    test_description<StringImpl>(__PRETTY_FUNCTION__);
    
    ADD_TESTS(1);

    typedef pfs::traits::string<StringImpl> string;
    string s1(string_samples<StringImpl>(STR_ORIG));
    string s2(string_samples<StringImpl>(STR_SAME));
    
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
                    << stringify_string_impl<StringImpl>()
                    << ">: failed with index: " << i << std::endl;
            break;
        }
    }

    std::ostringstream ss;
    ss << "Compare strings implemented using <" 
            << stringify_string_impl<StringImpl>() 
            << ">";
    
    TEST_OK2(i == count, ss.str().c_str());
}

template <typename StringImpl>
void test_compare ()
{
    test_compare_basic<StringImpl>();
    test_compare_empty<StringImpl>();
}

#endif /* __PFS_TEST_COMPARE_HPP__ */

