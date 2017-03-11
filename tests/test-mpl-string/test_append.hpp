/* 
 * File:   test_append.hpp
 * Author: wladt
 *
 * Created on March 11, 2017, 1:25 PM
 */

#ifndef __TEST_APPEND_HPP__
#define __TEST_APPEND_HPP__

template <typename Foundation, typename CharT>
void test_append ()
{
    test_description<CharT>(__PRETTY_FUNCTION__);
    
    ADD_TESTS(3);
    
    typedef pfs::traits::string<Foundation, CharT> string_type;

    {
        string_type orig(string_samples<CharT>(STR_ORIG));
        string_type same(string_samples<CharT>(STR_SAME));
        string_type result(string_samples<CharT>(STR_CONCAT));
        
        TEST_OK(orig.append(same) == result);
    }

    {
        string_type orig(string_samples<CharT>(STR_ORIG));
        string_type same(string_samples<CharT>(STR_SAME));
        string_type result(string_samples<CharT>(STR_CONCAT));

        TEST_OK(orig.append(same, 0, string_type::npos) == result);
    }
    
    {
        string_type orig(string_samples<CharT>(STR_ORIG));
        string_type same(string_samples<CharT>(STR_SAME));
        
        try {
            orig.append(same, same.size(), 1);
            TEST_FAIL2(false, "expected `out_of_range` exception throw");
        } catch (pfs::out_of_range const & ex) {
            TEST_OK2(true, "`out_of_range` exception throw");
        }
    }
}

#endif /* __TEST_APPEND_HPP__ */

