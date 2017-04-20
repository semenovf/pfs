/* 
 * File:   test_integer.hpp
 * Author: wladt
 *
 * Created on March 15, 2017, 5:40 PM
 */

#ifndef TEST_INTEGER_HPP
#define TEST_INTEGER_HPP

static char const * zero = "0";
static char const * max_schar = "128";
static char const * min_schar = "-127";
static char const * max_uchar = "255";

template <typename StringT>
void test_integer ()
{
    ADD_TESTS(0);
    
//    TEST_OK((pfs::lexical_cast<StringT, char>(StringT(zero)) == 0));
//    TEST_OK((pfs::lexical_cast<StringT, char>(StringT(max_schar)) 
//            == pfs::numeric_limits<char>::max()));
//    TEST_OK((pfs::lexical_cast<StringT, char>(StringT(min_schar)) 
//            == pfs::numeric_limits<char>::min()));
//
//    TEST_OK((pfs::lexical_cast<StringT, signed char>(StringT(zero)) == 0));
//    TEST_OK((pfs::lexical_cast<StringT, signed char>(StringT(max_schar)) 
//            == pfs::numeric_limits<signed char>::max()));
//    TEST_OK((pfs::lexical_cast<StringT, signed char>(StringT(min_schar)) 
//            == pfs::numeric_limits<signed char>::min()));

    TEST_OK((pfs::lexical_cast<StringT, unsigned char>(StringT(zero)) == 0));
    TEST_OK((pfs::lexical_cast<StringT, unsigned char>(StringT(max_uchar))
            == pfs::numeric_limits<unsigned char>::max()));
    
    try {
        
    } catch (...) {}
}

#endif /* TEST_INTEGER_HPP */

