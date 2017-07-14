/* 
 * File:   test_iterator.hpp
 * Author: wladt
 *
 * Created on July 12, 2017, 9:04 PM
 */

#ifndef TEST_ITERATOR_HPP
#define TEST_ITERATOR_HPP

template <typename StringImplType>
void test_iterator ()
{
    typedef pfs::safeformat_iterator<StringImplType> fmt_iterator;
    typedef typename fmt_iterator::string_type       string_type;
    
//    string_type f2("%");
//    string_type f3("%%");
    {
        ADD_TESTS(3);
        
        string_type s("");
        fmt_iterator i(s.cbegin(), s.cend());
        fmt_iterator end = i.end();
    
        TEST_OK(i == end);
        TEST_OK(++i == end);
        TEST_OK(i++ == end);
    }

    {
        ADD_TESTS(5);
        string_type s("%");
        fmt_iterator i(s.cbegin(), s.cend());
        fmt_iterator end = i.end();
    
        TEST_OK(i != end);
        TEST_OK(++i == end);
        TEST_OK(i++ == end);
        TEST_OK(!i.success());
        TEST_OK(i.status() == fmt_iterator::STATUS_INCOMPLETE);
    }

    {
        ADD_TESTS(4);
        string_type s("%%");
        fmt_iterator i(s.cbegin(), s.cend());
        fmt_iterator end = i.end();
    
        TEST_OK(i != end);
        TEST_OK(++i == end);
        TEST_OK(i++ == end);
        TEST_OK(i.success());
    }

    {
        ADD_TESTS(4);
        string_type s("some text");
        fmt_iterator i(s.cbegin(), s.cend());
        fmt_iterator end = i.end();
    
        TEST_OK(i != end);
        TEST_OK(++i == end);
        TEST_OK(i++ == end);
        TEST_OK(i.success());
    }

}

#endif /* TEST_ITERATOR_HPP */

