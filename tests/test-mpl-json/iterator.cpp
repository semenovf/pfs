/*
 * iterator.cpp
 *
 *  Created on: Jan 9, 2016
 *      Author: wladt
 */

#include <pfs/test/test.hpp>
#include "pfs/json/json.hpp"

#include <iostream>


void test_json_iterator ()
{
    {
        ADD_TESTS(11);

        pfs::json::json json;

        json["Image"]["Width"]               = 800;
        json["Image"]["Height"]              = 600;
        json["Image"]["Title"]               = "View from 15th Floor";
        json["Image"]["Thumbnail"]["Url"]    = "http://www.example.com/image/481989943";
        json["Image"]["Thumbnail"]["Height"] = 125;
        json["Image"]["Thumbnail"]["Width"]  = "100";

        json["Image"]["IDs"].push_back(116);
        json["Image"]["IDs"].push_back(943);
        json["Image"]["IDs"].push_back(234);
        json["Image"]["IDs"].push_back(38793);

        TEST_OK(json.size() == 1);
        TEST_OK(json.find("Image") != json.end());
        TEST_OK(json["Image"].find("Title") != json["Image"].end());
        TEST_OK(json["Image"].find("--unknown--") == json["Image"].end());
        TEST_OK(json["Image"]["Thumbnail"].contains("Width"));
        TEST_OK(not json["Image"]["Thumbnail"].contains("--unknown--"));

        pfs::json::json::const_iterator it = json.cbegin();

        TEST_OK(it->is_object());
        TEST_OK(it->size() == 5);
        TEST_OK(++it == json.cend());
        TEST_OK((--it)->is_object());

        pfs::json::json::const_iterator it_image = it->cbegin();
        TEST_OK(it_image->size() == 1);
    }
    
    {
        ADD_TESTS(2);
        
        pfs::json::json json;
        json["id"] = 1;
        pfs::json::json::const_iterator it = json.find("id");
        TEST_OK(it != json.cend());
        TEST_OK(it != json.end());
    }
}



