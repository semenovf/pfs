/* 
 * File:   test_append.hpp
 * Author: wladt
 *
 * Created on March 11, 2017, 1:25 PM
 */

#ifndef __TEST_APPEND_HPP__
#define __TEST_APPEND_HPP__

template <typename StringT>
void test_append ()
{
    typedef pfs::traits::string<StringT> string;
    typedef typename string::value_type  value_type;
    
    test_description<value_type>(__PRETTY_FUNCTION__);
    
    ADD_TESTS(3);

    {
        string orig(string_samples<value_type>(STR_ORIG));
        string same(string_samples<value_type>(STR_SAME));
        string result(string_samples<value_type>(STR_CONCAT));
        
        TEST_OK(orig.append(same) == result);
    }

    {
        string orig(string_samples<value_type>(STR_ORIG));
        string same(string_samples<value_type>(STR_SAME));
        string result(string_samples<value_type>(STR_CONCAT));

        TEST_OK(orig.append(same, 0, string::npos) == result);
    }
    
    {
        string orig(string_samples<value_type>(STR_ORIG));
        string same(string_samples<value_type>(STR_SAME));
        
        try {
            orig.append(same, same.size(), 1);
            TEST_FAIL2(false, "expected `out_of_range` exception throw");
        } catch (pfs::out_of_range const & ex) {
            TEST_OK2(true, "`out_of_range` exception throw");
        }
    }
}

#endif /* __TEST_APPEND_HPP__ */

