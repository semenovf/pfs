/* 
 * File:   test_lexical_cast_flost.hpp
 * Author: wladt
 *
 * Created on July 2, 2017, 1:36 PM
 */

#ifndef TEST_LEXICAL_CAST_FLOST_HPP
#define TEST_LEXICAL_CAST_FLOST_HPP

template <typename StringT>
void test_lexical_cast_float ()
{
    ADD_TESTS(8);
    
    TEST_OK((pfs::lexical_cast<float, StringT>(StringT("0"))  == 0.0f));
    TEST_OK((pfs::lexical_cast<float, StringT>(StringT("0.")) == 0.0f));
    TEST_OK((pfs::lexical_cast<float, StringT>(StringT("0.0")) == 0.0f));
    TEST_OK((pfs::lexical_cast<float, StringT>(StringT(".0")) == 0.0f));

    TEST_OK((pfs::lexical_cast<float, StringT>(StringT("1"))  == 1.0f));
    TEST_OK((pfs::lexical_cast<float, StringT>(StringT("1.")) == 1.0f));
    TEST_OK((pfs::lexical_cast<float, StringT>(StringT("1.0")) == 1.0f));
    TEST_OK((pfs::lexical_cast<float, StringT>(StringT(".1")) == 0.1f));
}


#endif /* TEST_LEXICAL_CAST_FLOST_HPP */

