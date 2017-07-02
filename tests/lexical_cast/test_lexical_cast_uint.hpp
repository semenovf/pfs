/* 
 * File:   test_lexical_cast_uint.hpp
 * Author: wladt
 *
 * Created on July 2, 2017, 1:38 PM
 */

#ifndef TEST_LEXICAL_CAST_UINT_HPP
#define TEST_LEXICAL_CAST_UINT_HPP

template <typename StringT>
void test_lexical_cast_uint ()
{
    ADD_TESTS(2);
    
    TEST_OK((pfs::lexical_cast<unsigned char, StringT>(StringT("0")) == 0));
    TEST_OK((pfs::lexical_cast<unsigned char, StringT>(StringT("255"))
            == pfs::numeric_limits<unsigned char>::max()));
}

#endif /* TEST_LEXICAL_CAST_UINT_HPP */

