/*
 * encode.cpp
 *
 *  Created on: Nov 7, 2015
 *      Author: wladt
 */

#include <pfs/test/test.hpp>
#include <pfs/unicode/utf8_iterator.hpp>
#include <string>
#include <sstream>
#include "test_data.hpp"

template <typename OctetIt>
void __test_encode (char const * itertype)
{
	int ntests = sizeof(data)/sizeof(data[0]);
	ADD_TESTS(ntests);

	for (int i = 0; i < ntests; ++i) {
		OctetIt it(iter_cast<OctetIt>(data[i].text));
		OctetIt it_end(iter_cast<OctetIt>(data[i].text) + data[i].len);

		pfs::unicode::char_t uc(0);
		std::string result;

		while (it < it_end) {
			uc = pfs::unicode::utf8_iterator<OctetIt>::decode(it);
			pfs::unicode::utf8_iterator<OctetIt>::encode(uc, std::back_inserter(result));
		}

		std::ostringstream desc;

		desc << "Decode/Encode to UTF-8 using `" << itertype << "' as pointer. String `"
				<< data[i].name	<< "'.";
		TEST_OK2(result == std::string(reinterpret_cast<char *>(data[i].text), data[i].len)
			, desc.str().c_str());
	}
}

void test_encode ()
{
	__test_encode<unsigned char *>("unsigned char *");
	__test_encode<const unsigned char *>("const unsigned char *");
	__test_encode<char *>("char *");
	__test_encode<const char *>("const char *");
	__test_encode<std::string::iterator>("std::string::iterator");
	__test_encode<std::string::const_iterator>("std::string::const_iterator");
}
