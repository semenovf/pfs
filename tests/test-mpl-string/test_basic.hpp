/* 
 * File:   test_basic.hpp
 * Author: wladt
 *
 * Created on December 27, 2016, 2:02 PM
 */

#ifndef __PFS_TEST_BASIC_HPP__
#define __PFS_TEST_BASIC_HPP__

template <typename StringT>
void test_assign ()
{
    typedef pfs::traits::string<StringT>                 string_type;
    typedef typename string_type::const_iterator         const_iterator;
    typedef typename string_type::const_reverse_iterator const_reverse_iterator;
    typedef typename string_type::value_type             value_type;

    test_description<value_type>(__PRETTY_FUNCTION__);
    
    ADD_TESTS(5);
    
    string_type orig(string_samples<value_type>(STR_ORIG)); // Constructor (StringImpl const &)
    string_type same(string_samples<value_type>(STR_SAME)); // Constructor (StringImpl const &)
    string_type diff(string_samples<value_type>(STR_DIFF)); // Constructor (StringImpl const &)
    string_type dup(orig); // Copy constructor
    string_type orig1;     // Empty string
    orig1 = orig;          // Copy assign operator
    string_type orig2;
    orig2 = string_samples<value_type>(STR_ORIG); // Assign from string implementation type

    TEST_OK(orig == same);
    TEST_OK(orig != diff);
    TEST_OK(orig == dup);
    TEST_OK(orig == orig1);
    TEST_OK(orig == orig2);
}

template <typename StringT>
void test_iterator ()
{
    typedef pfs::traits::string<StringT>                 string_type;
    typedef typename string_type::const_iterator         const_iterator;
    typedef typename string_type::const_reverse_iterator const_reverse_iterator;
    typedef typename string_type::value_type             value_type;

    test_description<value_type>(__PRETTY_FUNCTION__);
    
    ADD_TESTS(16);
    
    string_type str(string_samples<value_type>(STR_ORIG));

    const_iterator it = str.cbegin();
    const_iterator end = str.cend();
    
    TEST_OK(it != end);
    
    TEST_OK(*it++ == value_type('A'));
    TEST_OK(*it++ == value_type('B'));
    TEST_OK(*it++ == value_type('C'));
    TEST_OK(*it++ == value_type('D'));
    TEST_OK(*it++ == value_type('E'));
    TEST_OK(*it++ == value_type('F'));
    
    TEST_OK(it == end);

    const_reverse_iterator itr = str.crbegin();
    const_reverse_iterator endr = str.crend();

    TEST_OK(itr != endr);
    
    TEST_OK(*itr++ == value_type('F'));
    TEST_OK(*itr++ == value_type('E'));
    TEST_OK(*itr++ == value_type('D'));
    TEST_OK(*itr++ == value_type('C'));
    TEST_OK(*itr++ == value_type('B'));
    TEST_OK(*itr++ == value_type('A'));
    
    TEST_OK(itr == endr);
}


template <typename StringT>
void test_subscript ()
{
    typedef pfs::traits::string<StringT>     string_type;
    typedef typename string_type::value_type value_type;

    test_description<value_type>(__PRETTY_FUNCTION__);

    ADD_TESTS(7);

    string_type str(string_samples<value_type>(STR_ORIG)); 
 
    TEST_OK(str[0] == value_type('A'));
    TEST_OK(str[1] == value_type('B'));
    TEST_OK(str[2] == value_type('C'));
    TEST_OK(str[3] == value_type('D'));
    TEST_OK(str[4] == value_type('E'));
    TEST_OK(str[5] == value_type('F'));

    try {
        // throw, even if capacity allowed to access element
        (void)str[6];
        TEST_FAIL2(false, "expected `out_of_range` exception throw");
    } catch (pfs::out_of_range const & ex) {
        TEST_OK2(true, "`out_of_range` exception throw");
        std::cout << ex.what() << std::endl;
    }    
}

template <typename StringT>
void test_basic ()
{
    test_assign<StringT>();
    test_iterator<StringT>();
    test_subscript<StringT>();
}

#endif /* __PFS_TEST_BASIC_HPP__ */
