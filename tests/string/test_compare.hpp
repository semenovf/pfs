/* 
 * File:   test_compare.hpp
 * Author: wladt
 *
 * Created on January 10, 2017, 10:30 AM
 */

#ifndef __PFS_TEST_COMPARE_HPP__
#define __PFS_TEST_COMPARE_HPP__

template <typename StringT>
void test_compare_basic ()
{
    typedef pfs::string<StringT> string;
    typedef typename string::value_type  value_type;
    
    test_description<value_type>(__PRETTY_FUNCTION__);
    
    ADD_TESTS(10);
    
    string orig(string_samples<value_type>(STR_ORIG));
    string same(string_samples<value_type>(STR_SAME));
    string diff(string_samples<value_type>(STR_DIFF));
    string less(string_samples<value_type>(STR_LESS));
    string greater(string_samples<value_type>(STR_GREATER));
    string head(string_samples<value_type>(STR_HEAD));
    string tail(string_samples<value_type>(STR_TAIL));

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

template <typename StringT>
void test_compare_empty ()
{
    typedef pfs::string<StringT> string;
    typedef typename string::value_type  value_type;

    test_description<value_type>(__PRETTY_FUNCTION__);
    
    ADD_TESTS(1);

    string s1(string_samples<value_type>(STR_ORIG));
    string s2(string_samples<value_type>(STR_SAME));
    
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
                    << stringify_string_impl<value_type>()
                    << ">: failed with index: " << i << std::endl;
            break;
        }
    }

    std::ostringstream ss;
    ss << "Compare strings implemented using <" 
            << stringify_string_impl<value_type>() 
            << ">";
    
    TEST_OK2(i == count, ss.str().c_str());
}

template <typename StringT>
void test_compare ()
{
    test_compare_basic<StringT>();
    test_compare_empty<StringT>();
}

template <typename StringT>
void test_compare_cstr ()
{
    typedef pfs::string<StringT> string;
    typedef typename string::value_type  value_type;
    
    test_description<value_type>(__PRETTY_FUNCTION__);
    
    ADD_TESTS(16);
    
    string orig(string_samples<value_type>(STR_ORIG));
    
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

