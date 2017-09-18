/*
 * test.cpp
 *
 *  Created on: Oct 10, 2014
 *      Author: wladt
 */

#ifndef TEST_STRING_TO_FLOAT_HPP
#define TEST_STRING_TO_FLOAT_HPP

#include <cstdio>
#include <cstdlib> // strtof, strtod, strtold
#include <cerrno>

template <typename RealType>
RealType std_string_to_float (char const * nptr, char ** endptr);

template <>
inline float std_string_to_float<float> (char const * nptr, char ** endptr)
{
    errno = 0;
    return strtof(nptr, endptr);
}

template <>
inline double std_string_to_float<double> (char const * nptr, char ** endptr)
{
    errno = 0;
    return strtod(nptr, endptr);
}

#if PFS_HAVE_LONG_DOUBLE

template <>
inline long double std_string_to_float<long double> (char const * nptr, char ** endptr)
{
    errno = 0;
    return strtold(nptr, endptr);
}

#endif

template <typename RealType>
static void __print_diff_less_than_epsilon (char const * prefix
        , RealType result
        , RealType sample
        , RealType absdiff)
{
    fprintf(stdout, "***WARN: %s: fabs(\n"
            "\tresult =%f - \n"
            "\tsample =%f) = \n"
            "\tabsdiff=%g <= \n"
            "\tepsilon=%g\n"
            , prefix, result, sample, absdiff
            , pfs::numeric_limits<RealType>::epsilon());
}

template <typename RealType>
static void __print_diff_greater_than_epsilon (char const * prefix
        , RealType result
        , RealType sample
        , RealType absdiff)
{
    fprintf(stderr, "***ERROR: %s: fabs(\n"
            "\tresult =%f - \n"
            "\tsample =%f)= \n"
            "\tabsdiff=%g > \n"
            "\tepsilon=%g\n"
            , prefix, result, sample, absdiff
            , pfs::numeric_limits<RealType>::epsilon());
}

template <typename RealType, typename StringType>
static bool __test_string_to_real (char const * s
    , RealType /*result_sample*/
    , ptrdiff_t badpos_increment)
{
    typedef typename StringType::const_iterator char_iterator_type;
    
    bool r = true;
    char_iterator_type badpos;
    StringType str(s);
    
    RealType result = pfs::string_to_real<RealType, char_iterator_type>(str.cbegin()
        , str.cend()
        ,'.'
        , & badpos);

    char * endptr;
    RealType std_result = std_string_to_float<RealType>(s, & endptr);
    
    if (result != std_result) {
        RealType absdiff = pfs::fabs(result - std_result);
        
        if (absdiff <= pfs::numeric_limits<RealType>::epsilon()) {
            __print_diff_less_than_epsilon("Comparing with result of strtof[d,ld]()"
                    , result
                    , std_result
                    , absdiff);
        } else {
            __print_diff_greater_than_epsilon("Comparing with result of strtof[d,ld]()"
                    , result
                    , std_result
                    , absdiff);
            r = false;
        }
    }
    
//    if (result != result_sample) {
//        RealType absdiff = pfs::fabs(result - result_sample);
//        
//        if (absdiff <= pfs::numeric_limits<RealType>::epsilon()) {
//            __print_diff_less_than_epsilon("Comparing with sample"
//                    , result
//                    , result_sample
//                    , absdiff);
//        } else {
//            __print_diff_greater_than_epsilon("Comparing with sample"
//                    , result
//                    , result_sample
//                    , absdiff);
//            r = false;
//        }
//    }

    pfs::advance(badpos, badpos_increment);
    
    if (badpos != str.cend()) {
        std::cout << "***ERROR: badpos does not match" << std::endl;
        r = false;
    }
    
    return r;
}

template <typename RealType, typename StringType>
static bool __test_isnan (char const * s)
{
    typedef typename StringType::const_iterator char_iterator_type;
    char_iterator_type badpos;
    StringType str(s);
    
    return pfs::isnan(pfs::string_to_real<RealType, char_iterator_type>(str.begin()
            , str.end()
            , '.'
            , & badpos));
}

template <typename RealType, typename StringType>
static bool __test_isinf (char const * s)
{
    typedef typename StringType::const_iterator char_iterator_type;
    char_iterator_type badpos;
    StringType str(s);
    
    return pfs::isinf(pfs::string_to_real<RealType, char_iterator_type>(str.begin()
            , str.end()
            , '.'
            , & badpos));
}

template <typename RealType, typename StringType>
void test_string_to_real ()
{
    ADD_TESTS(95);
    
    TEST_OK((__test_isinf<RealType, StringType>("INFINITY")));
    TEST_OK((__test_isinf<RealType, StringType>("-INFINITY")));
    TEST_OK((__test_isinf<RealType, StringType>("+INFINITY")));
    TEST_OK((__test_isinf<RealType, StringType>("+InFiNiTy")));
    TEST_OK((__test_isinf<RealType, StringType>("-infinity")));
    TEST_OK((__test_isinf<RealType, StringType>("+INFInity")));
    TEST_OK((__test_isinf<RealType, StringType>("INF")));
    TEST_OK((__test_isinf<RealType, StringType>("-INF")));
    TEST_OK((__test_isinf<RealType, StringType>("+INF")));
    TEST_OK((__test_isinf<RealType, StringType>("INFINITY$%^")));
    
    TEST_OK((__test_isnan<RealType, StringType>("NAN")));
    TEST_OK((__test_isnan<RealType, StringType>("-NAN")));
    TEST_OK((__test_isnan<RealType, StringType>("+NAN")));
    TEST_OK((__test_isnan<RealType, StringType>("nAN")));
    TEST_OK((__test_isnan<RealType, StringType>("-nAn")));
    TEST_OK((__test_isnan<RealType, StringType>("+nan")));
    TEST_OK((__test_isnan<RealType, StringType>("NAN$%^")));

    TEST_OK((__test_string_to_real<RealType, StringType>("-"  , 0.0f, 1)));
    TEST_OK((__test_string_to_real<RealType, StringType>("+"  , 0.0f, 1)));
    TEST_OK((__test_string_to_real<RealType, StringType>("   ", 0.0f, 3)));
    TEST_OK((__test_string_to_real<RealType, StringType>(" ." , 0.0f, 2)));
    TEST_OK((__test_string_to_real<RealType, StringType>(""   , 0.0f, 0)));
    
    TEST_OK((__test_string_to_real<RealType, StringType>("0"  , 0.0f, 0)));
    TEST_OK((__test_string_to_real<RealType, StringType>("00" , 0.0f, 0)));
    TEST_OK((__test_string_to_real<RealType, StringType>("000", 0.0f, 0)));
    TEST_OK((__test_string_to_real<RealType, StringType>(".1" , 0.1f, 0)));
    TEST_OK((__test_string_to_real<RealType, StringType>(".12" , 0.12f, 0)));
    TEST_OK((__test_string_to_real<RealType, StringType>(".1234567890" , 0.1234567890f, 0)));
    TEST_OK((__test_string_to_real<RealType, StringType>("1.2e3"   , 1.2e3, 0)));
    TEST_OK((__test_string_to_real<RealType, StringType>(" 1.2e3"  , 1.2e3, 0)));
    TEST_OK((__test_string_to_real<RealType, StringType>(" +1.2e3" , 1.2e3, 0)));
    TEST_OK((__test_string_to_real<RealType, StringType>("+1.e3"   , 1.e3, 0)));
    TEST_OK((__test_string_to_real<RealType, StringType>("-1.2e3"  , -1.2e3, 0)));
    TEST_OK((__test_string_to_real<RealType, StringType>("-1.2e3.5", -1.2e3, 2)));
    TEST_OK((__test_string_to_real<RealType, StringType>(".1e", 0.1f, 1)));
    TEST_OK((__test_string_to_real<RealType, StringType>("-1.2e    ", -1.2, 5)));
    TEST_OK((__test_string_to_real<RealType, StringType>("--1.2e3.5", 0/*-1.2e3*/, 9)));
    TEST_OK((__test_string_to_real<RealType, StringType>("a", 0, 1)));
    TEST_OK((__test_string_to_real<RealType, StringType>("-a", 0, 2)));
    TEST_OK((__test_string_to_real<RealType, StringType>(".1e3", .1e3, 0)));
    TEST_OK((__test_string_to_real<RealType, StringType>(".1e-3", .1e-3, 0)));
    TEST_OK((__test_string_to_real<RealType, StringType>(".1e-", .1, 2)));
    TEST_OK((__test_string_to_real<RealType, StringType>(" .e", 0, 3)));
    TEST_OK((__test_string_to_real<RealType, StringType>(" .e-", 0, 4)));
    TEST_OK((__test_string_to_real<RealType, StringType>(" e"  , 0, 2)));
    TEST_OK((__test_string_to_real<RealType, StringType>(" e0" , 0, 3)));
    TEST_OK((__test_string_to_real<RealType, StringType>(" ee" , 0, 3)));
    TEST_OK((__test_string_to_real<RealType, StringType>(" -e" , 0, 3)));
    TEST_OK((__test_string_to_real<RealType, StringType>(" .9" , 0.9f, 0)));
    TEST_OK((__test_string_to_real<RealType, StringType>(" 0.9", 0.9f, 0)));
    TEST_OK((__test_string_to_real<RealType, StringType>(" ..9", 0, 4)));
    TEST_OK((__test_string_to_real<RealType, StringType>("9."  , 9.0f, 0)));
    TEST_OK((__test_string_to_real<RealType, StringType>("9"   , 9.0f, 0)));
    TEST_OK((__test_string_to_real<RealType, StringType>("9.0"       , 9.0f, 0)));
    TEST_OK((__test_string_to_real<RealType, StringType>("9.0000"    , 9.0f, 0)));
    TEST_OK((__test_string_to_real<RealType, StringType>("9.00001"   , 9.00001f, 0)));
    TEST_OK((__test_string_to_real<RealType, StringType>("009"       , 9.0f, 0)));
    TEST_OK((__test_string_to_real<RealType, StringType>("0.09e02"   , 9.0f, 0)));
    TEST_OK((__test_string_to_real<RealType, StringType>("2.22e-32"  , RealType(2.22e-32f), 0)));
    TEST_OK((__test_string_to_real<RealType, StringType>("1.34"      , 1.34f, 0)));
    TEST_OK((__test_string_to_real<RealType, StringType>("12.34"     , 12.34f, 0)));
    TEST_OK((__test_string_to_real<RealType, StringType>("123.456"   , 123.456f, 0)));
    
    TEST_OK((__test_string_to_real<RealType, StringType>("999999999999999999" , 1.0f, 0)));
    TEST_OK((__test_string_to_real<RealType, StringType>("000000000999999999" , 1.0f, 0)));
    TEST_OK((__test_string_to_real<RealType, StringType>("0.99999999999999999" , 1.0f, 0)));
    TEST_OK((__test_string_to_real<RealType, StringType>("0.9999999999999999999999999999999999" , 1.0f, 0)));
    
    TEST_OK((__test_string_to_real<RealType, StringType>("1", RealType(1.0f), 0)));
	TEST_OK((__test_string_to_real<RealType, StringType>("12", RealType(12.0f), 0)));
	TEST_OK((__test_string_to_real<RealType, StringType>("123", RealType(123.0f), 0)));
	TEST_OK((__test_string_to_real<RealType, StringType>("1234", RealType(1234.0f), 0)));
	TEST_OK((__test_string_to_real<RealType, StringType>("12345", RealType(12345.0f), 0)));
	TEST_OK((__test_string_to_real<RealType, StringType>("123456", RealType(123456.0f), 0)));
	TEST_OK((__test_string_to_real<RealType, StringType>("1234567", RealType(1234567.0f), 0)));
	TEST_OK((__test_string_to_real<RealType, StringType>("12345678", RealType(12345678.0f), 0)));
	TEST_OK((__test_string_to_real<RealType, StringType>("123456789", RealType(123456789.0f), 0)));
	TEST_OK((__test_string_to_real<RealType, StringType>("1234567890", RealType(1234567890.0f), 0)));
	TEST_OK((__test_string_to_real<RealType, StringType>("12345678901", RealType(12345678901.0f), 0)));
	TEST_OK((__test_string_to_real<RealType, StringType>("123456789012", RealType(123456789012.0f), 0)));
	TEST_OK((__test_string_to_real<RealType, StringType>("1234567890123", RealType(1234567890123.0f), 0)));
	TEST_OK((__test_string_to_real<RealType, StringType>("12345678901234", RealType(12345678901234.0f), 0)));
	TEST_OK((__test_string_to_real<RealType, StringType>("123456789012345", RealType(123456789012345.0f), 0)));
	TEST_OK((__test_string_to_real<RealType, StringType>("1234567890123456", RealType(1234567890123456.0f), 0)));
	TEST_OK((__test_string_to_real<RealType, StringType>("12345678901234567", RealType(12345678901234567.0f), 0)));
	TEST_OK((__test_string_to_real<RealType, StringType>("123456789012345678", RealType(123456789012345678.0f), 0)));
	TEST_OK((__test_string_to_real<RealType, StringType>("1234567890123456789", RealType(1234567890123456789.0f), 0)));
	TEST_OK((__test_string_to_real<RealType, StringType>("12345678901234567890", RealType(12345678901234567890.0f), 0)));
	TEST_OK((__test_string_to_real<RealType, StringType>("123456789012345678901", RealType(123456789012345678901.0f), 0)));
	TEST_OK((__test_string_to_real<RealType, StringType>("1234567890123456789012", RealType(1234567890123456789012.0f), 0)));
	TEST_OK((__test_string_to_real<RealType, StringType>("12345678901234567890123", RealType(12345678901234567890123.0f), 0)));
	TEST_OK((__test_string_to_real<RealType, StringType>("123456789012345678901234", RealType(123456789012345678901234.0f), 0)));
	TEST_OK((__test_string_to_real<RealType, StringType>("1234567890123456789012345", RealType(1234567890123456789012345.0f), 0)));
	TEST_OK((__test_string_to_real<RealType, StringType>("12345678901234567890123456", RealType(12345678901234567890123456.0f), 0)));
	TEST_OK((__test_string_to_real<RealType, StringType>("123456789012345678901234567", RealType(123456789012345678901234567.0f), 0)));
	TEST_OK((__test_string_to_real<RealType, StringType>("1234567890123456789012345678", RealType(1234567890123456789012345678.0f), 0)));
	TEST_OK((__test_string_to_real<RealType, StringType>("12345678901234567890123456789", RealType(12345678901234567890123456789.0f), 0)));
}

#if __OBSOLETE__

#ifdef __COMMENT__
char * doubleToHex (real_t x)
{
    unsigned int i;
    static char buf[100];
    unsigned char const * p = (unsigned char const *) & x;

    for (i = 0; i < sizeof x; ++i)
    	sprintf(buf + 2*i, "%02x", *p++);

    return buf;
}

	TEST_OK2(compare_with_strtold("2.22507385850720138309e-308"), _MSG);
	TEST_OK2(compare_with_strtold("1.79769313486231570815e+308"), _MSG);
	TEST_OK2(compare_with_strtold("3.36210314311209350626e-4932"), _MSG);
	TEST_OK2(compare_with_strtold("1.18973149535723176502e+4932"), _MSG);
	TEST_OK2(compare_with_strtold("1.18973149535723176502126385303e+4932"), _MSG);
	TEST_OK2(compare_with_strtold("18973149535723176502126385303"), _MSG);
	TEST_OK2(compare_with_strtold("12345678901234567890123456789"), _MSG);

	TEST_OK2(compare_with_literal("1", PFS_REAL_LITERAL(1.0)), _MSG);
	TEST_OK2(compare_with_literal("12", PFS_REAL_LITERAL(12.0)), _MSG);
	TEST_OK2(compare_with_literal("123", PFS_REAL_LITERAL(123.0)), _MSG);
	TEST_OK2(compare_with_literal("1234", PFS_REAL_LITERAL(1234.0)), _MSG);
	TEST_OK2(compare_with_literal("12345", PFS_REAL_LITERAL(12345.0)), _MSG);
	TEST_OK2(compare_with_literal("123456", PFS_REAL_LITERAL(123456.0)), _MSG);
	TEST_OK2(compare_with_literal("1234567", PFS_REAL_LITERAL(1234567.0)), _MSG);
	TEST_OK2(compare_with_literal("12345678", PFS_REAL_LITERAL(12345678.0)), _MSG);
	TEST_OK2(compare_with_literal("123456789", PFS_REAL_LITERAL(123456789.0)), _MSG);
	TEST_OK2(compare_with_literal("1234567890", PFS_REAL_LITERAL(1234567890.0)), _MSG);
	TEST_OK2(compare_with_literal("12345678901", PFS_REAL_LITERAL(12345678901.0)), _MSG);
	TEST_OK2(compare_with_literal("123456789012", PFS_REAL_LITERAL(123456789012.0)), _MSG);
	TEST_OK2(compare_with_literal("1234567890123", PFS_REAL_LITERAL(1234567890123.0)), _MSG);
	TEST_OK2(compare_with_literal("12345678901234", PFS_REAL_LITERAL(12345678901234.0)), _MSG);
	TEST_OK2(compare_with_literal("123456789012345", PFS_REAL_LITERAL(123456789012345.0)), _MSG);
	TEST_OK2(compare_with_literal("1234567890123456", PFS_REAL_LITERAL(1234567890123456.0)), _MSG);
	TEST_OK2(compare_with_literal("12345678901234567", PFS_REAL_LITERAL(12345678901234567.0)), _MSG);
	TEST_OK2(compare_with_literal("123456789012345678", PFS_REAL_LITERAL(123456789012345678.0)), _MSG);
	TEST_OK2(compare_with_literal("1234567890123456789", PFS_REAL_LITERAL(1234567890123456789.0)), _MSG);
	TEST_OK2(compare_with_literal("12345678901234567890", PFS_REAL_LITERAL(12345678901234567890.0)), _MSG);
	TEST_OK2(compare_with_literal("123456789012345678901", PFS_REAL_LITERAL(123456789012345678901.0)), _MSG);
	TEST_OK2(compare_with_literal("1234567890123456789012", PFS_REAL_LITERAL(1234567890123456789012.0)), _MSG);
	TEST_OK2(compare_with_literal("12345678901234567890123", PFS_REAL_LITERAL(12345678901234567890123.0)), _MSG);
	TEST_OK2(compare_with_literal("123456789012345678901234", PFS_REAL_LITERAL(123456789012345678901234.0)), _MSG);
	TEST_OK2(compare_with_literal("1234567890123456789012345", PFS_REAL_LITERAL(1234567890123456789012345.0)), _MSG);
	TEST_OK2(compare_with_literal("12345678901234567890123456", PFS_REAL_LITERAL(12345678901234567890123456.0)), _MSG);
	TEST_OK2(compare_with_literal("123456789012345678901234567", PFS_REAL_LITERAL(123456789012345678901234567.0)), _MSG);
	TEST_OK2(compare_with_literal("1234567890123456789012345678", PFS_REAL_LITERAL(1234567890123456789012345678.0)), _MSG);
	TEST_OK2(compare_with_literal("12345678901234567890123456789", PFS_REAL_LITERAL(12345678901234567890123456789.0)), _MSG);

	real_t d0 = real_t(123456789012345678901.0L);
	real_t d1 = strtoreal(_u8("123456789012345678901"), 0);
	real_t d2
	  = 1e0 * 123456789012345678901.0L;
	real_t d3
	  = 1e11 * 1234567890.0L + 1.e0 * 12345678901.0L;
	real_t d4
	  = 1e13 * 12345678.0L + 1e5 * 90123456.0L + 1e0 * 78901.0L;
	real_t d5
	  = 1e20 * 1.0L + 1e18 * 23.0L + 1e16 * 45.0L
	  + 1e14 * 67.0L + 1e12 * 89.0L + 1e10 * 01.0L
	  + 1e8 * 23.0L + 1e6 * 45.0L + 1e4 * 67.0L
	  + 1e2 * 89.0L + 1e0 * 01.0L;
	real_t d6
	  = 1e20 * 1.0 + 1e19 * 2.0 + 1e18 * 3.0
	  + 1e17 * 4.0 + 1e16 * 5.0 + 1e15 * 6.0
	  + 1e14 * 7.0 + 1e13 * 8.0 + 1e12 * 9.0
	  + 1e11 * 0.0 + 1e10 * 1.0 + 1e09 * 2.0
	  + 1e08 * 3.0 + 1e07 * 4.0 + 1e06 * 5.0
	  + 1e05 * 6.0 + 1e04 * 7.0 + 1e03 * 8.0
	  + 1e02 * 9.0 + 1e01 * 0.0 + 1e00 * 1.0;

	printf("  : 123456789012345678901 = 123456789012345678901\n");
	printf("d0: 123456789012345678901 = %.30Lg\n", d0);
	printf("d1: 123456789012345678901 = %.30Lg\n", d1);
	printf("d2: 123456789012345678901 = %.30Lg\n", d2);
	printf("d3: 123456789012345678901 = %.30Lg\n", d3);
	printf("d4: 123456789012345678901 = %.30Lg\n", d4);
	printf("d5: 123456789012345678901 = %.30Lg\n", d5);
	printf("d6: 123456789012345678901 = %.30Lg\n", d6);
//	TEST_OK2(compare_with_literal("123456789012345678901", PFS_REAL_LITERAL(123456789012345678901.0)), _MSG);

#ifdef __COMMENT__
	// PHP (slashdot.jp): http://opensource.slashdot.jp/story/11/01/08/0527259/PHP%E3%81%AE%E6%B5%AE%E5%8B%95%E5%B0%8F%E6%95%B0%E7%82%B9%E5%87%A6%E7%90%86%E3%81%AB%E7%84%A1%E9%99%90%E3%83%AB%E3%83%BC%E3%83%97%E3%81%AE%E3%83%90%E3%82%B0
	TEST_OK2(compare_with_strtold("2.2250738585072011e-308"), _MSG);
	// Gauche: http://blog.practical-scheme.net/gauche/20110203-bitten-by-floating-point-numbers-again
	TEST_OK2(compare_with_strtold("2.2250738585072012e-308"), _MSG);
	TEST_OK2(compare_with_strtold("2.2250738585072013e-308"), _MSG); // Hmm.
	TEST_OK2(compare_with_strtold("2.2250738585072014e-308"), _MSG); // Hmm.

	TEST_OK2(compare_with_strtold("123.456"), _MSG);

// TODO Tests not passed {
#ifdef PFS_HAVE_LONG_DOUBLE
	TEST_OK2(compare_with_strtold("3.36210314311209350626e-4932"), _MSG); // PFS_LONG_DOUBLE_MIN
	TEST_OK2(compare_with_strtold("1.18973149535723176502e+4932"), _MSG); // PFS_LONG_DOUBLE_MAX
#else
	TEST_OK2(compare_with_strtold("2.22507385850720138309e-308"), _MSG); // PFS_DOUBLE_MIN
	TEST_OK2(compare_with_strtold("1.79769313486231570815e+308"), _MSG); // PFS_DOUBLE_MAX
#endif
// }

	// MSVC supports strtold since VC 2013.
//	std::cout << "'" << strtold("123.456", NULL) << "'"
//			<< "=='" << double_t(123.456) << "'"
//			<< std::endl;
//	TEST_FAIL(strtold("123.456", NULL) == double_t(123.456));
#endif // __COMMENT__

    return END_TESTS;
}

#endif

#endif

#endif // TEST_STRING_TO_FLOAT_HPP