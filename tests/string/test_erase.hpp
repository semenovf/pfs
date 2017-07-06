/* 
 * File:   test_erase.hpp
 * Author: wladt
 *
 * Created on February 9, 2017, 1:23 PM
 */

#ifndef __TEST_ERASE_HPP__
#define __TEST_ERASE_HPP__

template <typename StringT>
void test_erase ()
{
    typedef pfs::traits::string<StringT> string;
    typedef typename string::value_type  value_type;
    
    test_description<value_type>(__PRETTY_FUNCTION__);
    
    ADD_TESTS(21);
    {
        string s(string_samples<value_type>(STR_ORIG));

        s.erase(0, s.size());
        TEST_OK(s.empty());
    }

    {
        string orig(string_samples<value_type>(STR_ORIG));
        string s(string_samples<value_type>(STR_ORIG));

        s.erase(0, 1);
        TEST_OK(s.size() == orig.size() - 1);
        TEST_OK(s == orig.substr(1));
    }

    {
        string orig(string_samples<value_type>(STR_ORIG));
        string s(string_samples<value_type>(STR_ORIG));

        s.erase(1, 2);
        TEST_OK(s.size() == orig.size() - 2);
        TEST_OK(s[0] == orig[0]);
        TEST_OK(s[1] != orig[1]);
        TEST_OK(s[1] == orig[3]);
        TEST_OK(s[2] == orig[4]);
        TEST_OK(s[3] == orig[5]);
    }
    
    {
        string s(string_samples<value_type>(STR_ORIG));

        typename string::iterator it = s.erase(s.cbegin(), s.cend());
        
        TEST_OK(it == s.cbegin());
        TEST_OK(it == s.cend())
        TEST_OK(s.empty());
    }

    {
        string orig(string_samples<value_type>(STR_ORIG));
        string s(string_samples<value_type>(STR_ORIG));

        typename string::iterator it = s.erase(s.cbegin() + 1, s.cbegin() + 3);
        TEST_OK(s.size() == orig.size() - 2);
        TEST_OK(*it == orig[3]);
        TEST_OK(s[0] == orig[0]);
        TEST_OK(s[1] != orig[1]);
        TEST_OK(s[1] == orig[3]);
        TEST_OK(s[2] == orig[4]);
        TEST_OK(s[3] == orig[5]);
    }

    {
        string s(string_samples<value_type>(STR_ORIG));
        TEST_OK(!s.empty());
        
        s.clear();
        
        TEST_OK(s.empty());
    }
}

#endif /* __TEST_ERASE_HPP__ */
