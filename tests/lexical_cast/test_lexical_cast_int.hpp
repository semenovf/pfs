/* 
 * File:   test_integer.hpp
 * Author: wladt
 *
 * Created on March 15, 2017, 5:40 PM
 */

#ifndef TEST_LEXICAL_CAST_INT_HPP
#define TEST_LEXICAL_CAST_INT_HPP

template <typename StringT>
void test_lexical_cast_int ()
{
    ADD_TESTS(3);
    
    TEST_OK((pfs::lexical_cast<char, StringT>(StringT("0")) == 0));
    TEST_OK((pfs::lexical_cast<char, StringT>(StringT("127")) 
            == pfs::numeric_limits<char>::max()));
    TEST_OK((pfs::lexical_cast<char, StringT>(StringT("-128")) 
            == pfs::numeric_limits<char>::min()));

//    TEST_OK((pfs::lexical_cast<unsigned char, StringT>(StringT("0")) == 0));
//    TEST_OK((pfs::lexical_cast<unsigned char, StringT>(StringT("255"))
//            == pfs::numeric_limits<unsigned char>::max()));
}

#endif /* TEST_LEXICAL_CAST_INT_HPP */

