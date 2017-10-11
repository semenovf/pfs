
/* 
 * File:   test_trim.hpp
 * Author: wladt
 *
 * Created on October 11, 2017, 10:49 AM
 */

#ifndef __PFS_TEST_TRIM_HPP__
#define __PFS_TEST_TRIM_HPP__

template <typename StringImplType>
void test_trim ()
{
    typedef pfs::string<StringImplType> string_type;
    typedef typename string_type::const_pointer  const_pointer;

    test_description<const_pointer>(__PRETTY_FUNCTION__);
    
    ADD_TESTS(21);

    TEST_OK(pfs::ltrim(string_type()) == string_type());
    TEST_OK(pfs::ltrim(string_type()) == string_type(""));

    TEST_OK(pfs::rtrim(string_type()) == string_type());
    TEST_OK(pfs::rtrim(string_type()) == string_type(""));
    
    TEST_OK(pfs::trim(string_type()) == string_type());
    TEST_OK(pfs::trim(string_type()) == string_type(""));

    TEST_OK(pfs::ltrim(string_type("")) == string_type());
    TEST_OK(pfs::ltrim(string_type("")) == string_type(""));

    TEST_OK(pfs::rtrim(string_type("")) == string_type());
    TEST_OK(pfs::rtrim(string_type("")) == string_type(""));
    
    TEST_OK(pfs::trim(string_type("")) == string_type());
    TEST_OK(pfs::trim(string_type("")) == string_type(""));

    TEST_OK(pfs::ltrim(string_type("Abc")) == string_type("Abc"));
    TEST_OK(pfs::ltrim(string_type(" Abc")) == string_type("Abc"));
    TEST_OK(pfs::ltrim(string_type("  Abc ")) == string_type("Abc "));

    TEST_OK(pfs::rtrim(string_type("Abc")) == string_type("Abc"));
    TEST_OK(pfs::rtrim(string_type("Abc ")) == string_type("Abc"));
    TEST_OK(pfs::rtrim(string_type(" Abc  ")) == string_type(" Abc"));
    
    TEST_OK(pfs::trim(string_type("Abc")) == string_type("Abc"));
    TEST_OK(pfs::trim(string_type("Abc ")) == string_type("Abc"));
    TEST_OK(pfs::trim(string_type(" Abc  ")) == string_type("Abc"));
}

#endif /* __PFS_TEST_TRIM_HPP__ */

