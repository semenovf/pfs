/* 
 * File:   test_substr.hpp
 * Author: wladt
 *
 * Created on January 16, 2017, 10:42 AM
 */

#ifndef __PFS_TEST_SUBSTR_HPP__
#define __PFS_TEST_SUBSTR_HPP__

template <typename StringT>
void test_substr ()
{
    typedef pfs::traits::string<StringT>     string_type;
    typedef typename string_type::value_type value_type;

    test_description<value_type>(__PRETTY_FUNCTION__);
    
    ADD_TESTS(7);
    
    string_type orig(string_samples<value_type>(STR_ORIG));
    string_type same(string_samples<value_type>(STR_SAME));
    string_type head(string_samples<value_type>(STR_HEAD));
    string_type tail(string_samples<value_type>(STR_TAIL));
    string_type mid(string_samples<value_type>(STR_MID));
    
    TEST_OK(orig.substr(0)          == string_type(orig));
    TEST_OK(orig.substr(0)          == string_type(same));
    TEST_OK(orig.substr(1, 4)       == string_type(mid));
    TEST_OK(orig.left(head.size())  == string_type(head));
    TEST_OK(orig.right(tail.size()) == string_type(tail));
    TEST_OK(orig.substr(1)          == string_type(tail));
    TEST_OK(orig.substr(1, 100)     == string_type(tail));
}

#endif /* __PFS_TEST_SUBSTR_HPP__ */
