/*
 * mbcs_string.cpp
 *
 *  Created on: Nov 15, 2013
 *  Author: wladt
 */

#ifndef __PFS_TEST_TO_STRING_HPP__
#define __PFS_TEST_TO_STRING_HPP__

template <typename StringT>
void test_to_string ()
{
    typedef pfs::traits::string<StringT>     string_type;
    typedef typename string_type::value_type value_type;

    test_description<value_type>(__PRETTY_FUNCTION__);
    
    ADD_TESTS(13);
        
	TEST_OK((pfs::to_string<string_type>(0) == string_type("0")));
	TEST_OK((pfs::to_string<string_type>(127)     == string_type("127")));
	TEST_OK((pfs::to_string<string_type>(-128)    == string_type("-128")));
	TEST_OK((pfs::to_string<string_type>(255)     == string_type("255")));
	TEST_OK((pfs::to_string<string_type>(32767)   == string_type("32767")));
	TEST_OK((pfs::to_string<string_type>(-32768)  == string_type("-32768")));
	TEST_OK((pfs::to_string<string_type>(65535)   == string_type("65535")));
	TEST_OK((pfs::to_string<string_type>(8388607) == string_type("8388607")));
	TEST_OK((pfs::to_string<string_type>(-8388608) == string_type("-8388608")));
	TEST_OK((pfs::to_string<string_type>(16777215) == string_type("16777215")));
	TEST_OK((pfs::to_string<string_type>(pfs::numeric_limits<int32_t>::max()) == string_type("2147483647")));
	TEST_OK((pfs::to_string<string_type>(pfs::numeric_limits<int32_t>::min()) == string_type("-2147483648")));
	TEST_OK((pfs::to_string<string_type>(pfs::numeric_limits<uint32_t>::max()) == string_type("4294967295")));

#ifdef PFS_HAVE_LONG_LONG
	ADD_TESTS(3);
	TEST_OK((pfs::to_string<string_type>(pfs::numeric_limits<long long>::max()) == string_type("9223372036854775807")));
	TEST_OK((pfs::to_string<string_type>(pfs::numeric_limits<long long>::min()) == string_type("-9223372036854775808")));
	TEST_OK((pfs::to_string<string_type>(pfs::numeric_limits<unsigned long long>::max()) == string_type("18446744073709551615")));
#endif

    ADD_TESTS(6);
	// Note: single-precision floating-point numbers have a 24-bit mantissa, which is approximately 7.2 decimal digits.
	TEST_OK((pfs::to_string<string_type>(0.0f, 'g')        == string_type("0")));
	TEST_OK((pfs::to_string<string_type>(0.0f, 'f', 6)     == string_type("0.000000")));
	TEST_OK((pfs::to_string<string_type>(0.0f, 'f', 0)     == string_type("0")));
	TEST_OK((pfs::to_string<string_type>(0.0f, 'f', 1)     == string_type("0.0")));
	TEST_OK((pfs::to_string<string_type>(3.14159f, 'f', 5) == string_type("3.14159")));
	TEST_OK((pfs::to_string<string_type>(1234567.875f, 'f', 3) == string_type("1234567.875")));
}

#endif /* __PFS_TEST_TO_STRING_HPP__ */
