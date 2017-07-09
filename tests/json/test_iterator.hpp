/*
 * iterator.cpp
 *
 *  Created on: Jan 9, 2016
 *      Author: wladt
 */

#ifndef TEST_ITERATOR_HPP
#define TEST_ITERATOR_HPP

namespace test_iterator {

template <typename json_type>
void test ()
{
    {
        ADD_TESTS(11);

        json_type json; // null value
        //typename json_type::iterator it = json.begin();

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

        typename json_type::const_iterator cit = json.cbegin();

        TEST_OK(cit->is_object());
        TEST_OK(cit->size() == 5);
        
        TEST_OK(++cit == json.cend());
        TEST_OK((--cit)->is_object());

        typename json_type::const_iterator it_image = cit->cbegin();
        TEST_OK(it_image->size() == 1);
    }
    
    {
        ADD_TESTS(2);
        
        json_type json;
        json["id"] = 1;
        typename json_type::const_iterator it(json.find("id"));
        typename json_type::const_iterator it1;
        
        it1 = json.find("id");
        
        TEST_OK(it != json.cend());
        TEST_OK(it != typename json_type::const_iterator(json.end()));
    }
}

}

#endif // TEST_ITERATOR_HPP

