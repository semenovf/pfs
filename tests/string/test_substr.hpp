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
    typedef pfs::string<StringT>                string_type;
    typedef typename string_type::const_pointer const_pointer;

    test_description<const_pointer>(__PRETTY_FUNCTION__);
    
    ADD_TESTS(7);
    
    string_type orig(string_samples<const_pointer>(STR_ORIG));
    string_type same(string_samples<const_pointer>(STR_SAME));
    string_type head(string_samples<const_pointer>(STR_HEAD));
    string_type tail(string_samples<const_pointer>(STR_TAIL));
    string_type mid(string_samples<const_pointer>(STR_MID));

    string_type orig_ru(string_samples_ru<const_pointer>(STR_ORIG));
    string_type same_ru(string_samples_ru<const_pointer>(STR_SAME));
    string_type head_ru(string_samples_ru<const_pointer>(STR_HEAD));
    string_type tail_ru(string_samples_ru<const_pointer>(STR_TAIL));
    string_type mid_ru(string_samples_ru<const_pointer>(STR_MID));

    
    TEST_OK(orig.substr(orig.cbegin()) == orig);
    TEST_OK(orig.substr(orig.cbegin()) == same);
//    TEST_OK(orig.substr(1, 4)       == string_type(mid));
//    TEST_OK(orig.left(head.size())  == string_type(head));
//    TEST_OK(orig.right(tail.size()) == string_type(tail));
//    TEST_OK(orig.substr(1)          == string_type(tail));
//    TEST_OK(orig.substr(1, 100)     == string_type(tail));

    TEST_OK(orig_ru.substr(orig_ru.cbegin()) == orig_ru);
    TEST_OK(orig_ru.substr(orig_ru.cbegin()) == same_ru);

}

#endif /* __PFS_TEST_SUBSTR_HPP__ */
