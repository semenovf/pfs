/*
 * parse.cpp
 *
 *  Created on: Jan 5, 2016
 *      Author: wladt
 */

#include <pfs/test/test.hpp>
#include "pfs/json/json.hpp"

static const pfs::string json_empty_object_str = _u8("{}");
static const pfs::string json_empty_array_str = _u8("[]");

static const pfs::string json_object_str = _u8("{                      \
		\"Image\": {                                                   \
			\"Width\":  800,                                           \
			\"Height\": 600,                                           \
			\"Title\":  \"View from 15th Floor\",                      \
			\"Thumbnail\": {                                           \
				\"Url\":    \"http://www.example.com/image/481989943\",\
				\"Height\": 125,                                       \
				\"Width\":  \"100\"                                    \
			},                                                         \
			\"IDs\": [116, 943, 234, 38793]                            \
		  }                                                            \
	 }");

static const pfs::string json_invalid_str = _u8("   [     \
		  {                                      \
			 \"Latitude\":  37;7668              \
		  },                                     \
		  {                                      \
			 \"Latitude\":  37.371991            \
		  }                                      \
	   ]");

void test_json_parse ()
{
    ADD_TESTS(8);

	pfs::json::json json_empty_object;
	TEST_OK(json_empty_object.is_null());

	json_empty_object = pfs::json::parse(json_empty_object_str);

	TEST_OK(!json_empty_object.is_null());
	TEST_OK(json_empty_object.is_object());

	pfs::json::json json_empty_array;
	TEST_OK(json_empty_array.is_null());

	json_empty_array = pfs::json::parse(json_empty_array_str);

	TEST_OK(!json_empty_array.is_null());
	TEST_OK(json_empty_array.is_array());

	pfs::json::json json_object = pfs::json::parse(json_object_str);
	TEST_OK(json_object.is_object());

	pfs::json::json json_invalid = pfs::json::parse(json_invalid_str);
	TEST_OK(json_invalid.is_null());
//
//	pfs::string errstr;
//	if (!pfs::json::json().parse(_u8(json_invalid_str), & errstr)) {
//		cout << "Error: " << errstr << endl;
//		TEST_FAIL(!errstr.isEmpty())
//	}
//
//	pfs::json::json json_array(_u8(json_array_str));
//	TEST_OK(json_array.isArray());
//
//	TEST_OK(json_object.size() > 0);
//	TEST_OK(json_array.size() > 0);
//
//	TEST_OK(json_object[_l1("Image")][_l1("Width")].number() == real_t(800));
//
//	TEST_OK(json_object[_l1("Image")][_l1("Height")].number() == real_t(600));
//	TEST_OK(json_object[_l1("Image")][_l1("Title")].str() == _l1("View from 15th Floor"));
//	TEST_OK(json_object[_l1("Image")][_l1("Thumbnail")][_l1("Url")].str() == _l1("http://www.example.com/image/481989943"));
//	TEST_OK(json_object[_l1("Image")][_l1("Thumbnail")][_l1("Height")].number() == real_t(125));
//	TEST_OK(json_object[_l1("Image")][_l1("Thumbnail")][_l1("Width")].str() == _l1("100"));
//	TEST_OK(json_object[_l1("Image")][_l1("IDs")][0].number() == real_t(116));
//	TEST_OK(json_object[_l1("Image")][_l1("IDs")][1].number() == real_t(943));
//	TEST_OK(json_object[_l1("Image")][_l1("IDs")][2].number() == real_t(234));
//	TEST_OK(json_object[_l1("Image")][_l1("IDs")][3].number() == real_t(38793));
//
//	TEST_OK(json_object[_l1("Image")][_l1("IDs")][4].number() == real_t(0));
//	TEST_OK(json_object[_l1("Image")][_l1("Unknown")].is_null());
//
//	TEST_OK(json_array[0][_l1("precision")].str() == _l1("zip"));
//	TEST_OK(json_array[0][_l1("Latitude")].number()  == real_t(37.7668));
//	TEST_OK(json_array[0][_l1("Longitude")].number() == real_t(-122.3959));
//
//	TEST_OK(json_array[0][_l1("Address")].str()   == _l1(""));
//	TEST_FAIL(json_array[0][_l1("Address")].str().isEmpty());
//
//	TEST_OK(json_array[0][_l1("City")].str()      == _l1("SAN FRANCISCO"));
//	TEST_OK(json_array[0][_l1("State")].str()     == _l1("CA"));
//	TEST_OK(json_array[0][_l1("Zip")].str()       == _l1("94107"));
//	TEST_OK(json_array[0][_l1("Country")].str()   == _l1("US"));
//
//	TEST_OK(json_array[1][_l1("precision")].str() == _l1("zip"));
//	TEST_OK(json_array[1][_l1("Latitude")].number()  == real_t(37.371991));
//	TEST_OK(json_array[1][_l1("Longitude")].number() == real_t(-122.026020));
//	TEST_OK(json_array[1][_l1("Address")].str()   == _l1(""));
//	TEST_OK(json_array[1][_l1("City")].str()      == _l1("SUNNYVALE"));
//	TEST_OK(json_array[1][_l1("State")].str()     == _l1("CA"));
//	TEST_OK(json_array[1][_l1("Zip")].str()       == _l1("94085"));
//	TEST_OK(json_array[1][_l1("Country")].str()   == _l1("US"));
//
//	pfs::vector<pfs::string> keys = json_array[1].keys();
//	TEST_FAIL(keys.size() == 8);
//	for (size_t i = 0; i < keys.size(); ++i) {
//	    ADD_TESTS(1);
//		TEST_OK(_l1("precision Latitude Longitude Address City State Zip Country").contains(keys[i]));
//	}
}


