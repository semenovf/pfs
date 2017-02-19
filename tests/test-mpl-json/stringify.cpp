/*
 * stringify.cpp
 *
 *  Created on: Dec 3, 2015
 *      Author: wladt
 */
#include <pfs/test/test.hpp>
#include "pfs/json/json.hpp"

void test_json_stringify ()
{
	ADD_TESTS(1);

	const pfs::string sample_object_str("{\"Image\":{\"Height\":600,\"IDs\":[116,943,234,38793],"
		"\"Thumbnail\":{\"Height\":125,\"Url\":\"http://www.example.com/image/481989943\","
		"\"Width\":\"100\"},\"Title\":\"View from 15th Floor\",\"Width\":800}}");

	const pfs::string sample_array_str("[{\"Address\":\"\",\"City\":\"SAN FRANCISCO\","
			"\"Country\":\"US\",\"Latitude\":37.766800,\"Longitude\":-122.395900,"
			"\"State\":\"CA\",\"Zip\":\"94107\",\"precision\":\"zip\"},"
			"{\"Address\":\"\",\"City\":\"SUNNYVALE\",\"Country\":\"US\","
			"\"Latitude\":37.371991,\"Longitude\":-122.026020,\"State\":\"CA\","
			"\"Zip\":\"94085\",\"precision\":\"zip\"}]");

	pfs::json::json sample_object;

	sample_object["Image"]["Width"]               = 800;
	sample_object["Image"]["Height"]              = 600;
	sample_object["Image"]["Title"]               = _u8("View from 15th Floor");
	sample_object["Image"]["Thumbnail"]["Url"]    = _u8("http://www.example.com/image/481989943");
	sample_object["Image"]["Thumbnail"]["Height"] = 125;
	sample_object["Image"]["Thumbnail"]["Width"]  = _u8("100");

	sample_object["Image"]["IDs"].push_back(116);
	sample_object["Image"]["IDs"].push_back(943);
	sample_object["Image"]["IDs"].push_back(234);
	sample_object["Image"]["IDs"].push_back(38793);

	TEST_OK(pfs::to_string(sample_object) == sample_object_str);
}
