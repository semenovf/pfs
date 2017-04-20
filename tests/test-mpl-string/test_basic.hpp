/* 
 * File:   test_basic.hpp
 * Author: wladt
 *
 * Created on December 27, 2016, 2:02 PM
 */

#ifndef __PFS_TEST_BASIC_HPP__
#define __PFS_TEST_BASIC_HPP__

template <typename Foundation, typename CharT>
void test_assign ()
{
    test_description<CharT>(__PRETTY_FUNCTION__);
    
    ADD_TESTS(5);
    
    typedef pfs::traits::string<Foundation, CharT>       string_type;
    typedef typename string_type::const_iterator         const_iterator;
    typedef typename string_type::const_reverse_iterator const_reverse_iterator;
    typedef typename string_type::value_type             value_type;
    
    string_type orig(string_samples<CharT>(STR_ORIG)); // Constructor (StringImpl const &)
    string_type same(string_samples<CharT>(STR_SAME)); // Constructor (StringImpl const &)
    string_type diff(string_samples<CharT>(STR_DIFF)); // Constructor (StringImpl const &)
    string_type dup(orig); // Copy constructor
    string_type orig1;     // Empty string
    orig1 = orig;     // Copy assign operator
    string_type orig2;
    orig2 = string_samples<CharT>(STR_ORIG); // Assign from string implementation type

    TEST_OK(orig == same);
    TEST_OK(orig != diff);
    TEST_OK(orig == dup);
    TEST_OK(orig == orig1);
    TEST_OK(orig == orig2);
}

template <typename Foundation, typename CharT>
void test_iterator ()
{
    test_description<CharT>(__PRETTY_FUNCTION__);
    
    ADD_TESTS(16);
    
    typedef pfs::traits::string<Foundation, CharT>       string_type;
    typedef typename string_type::const_iterator         const_iterator;
    typedef typename string_type::const_reverse_iterator const_reverse_iterator;
    typedef typename string_type::value_type             value_type;
    
    string_type str(string_samples<CharT>(STR_ORIG));

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


template <typename Foundation, typename CharT>
void test_subscript ()
{
    test_description<CharT>(__PRETTY_FUNCTION__);

    ADD_TESTS(7);

    typedef pfs::traits::string<Foundation, CharT> string_type;
    typedef typename string_type::value_type       value_type;

    string_type str(string_samples<CharT>(STR_ORIG)); 
 
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

template <typename Foundation, typename CharT>
void test_basic ()
{
    test_assign<Foundation, CharT>();
    test_iterator<Foundation, CharT>();
    test_subscript<Foundation, CharT>();
}

#endif /* __PFS_TEST_BASIC_HPP__ */
