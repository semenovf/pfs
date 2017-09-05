/*
 * stringify.cpp
 *
 *  Created on: Dec 3, 2015
 *      Author: wladt
 */

namespace test_stringify {

template <typename JsonType>
void test ()
{
    typedef typename JsonType::string_type string_type;
#if 0    
    {
        ADD_TESTS(1);
        char const * sample = "[]";
        JsonType j = JsonType::make_array();
        TEST_OK(j.to_string() == sample);
    }

    {
        ADD_TESTS(1);
        char const * sample = "[[[]]]";
        JsonType j = JsonType::make_array();
        j[0] = JsonType::make_array();
        j[0][0] = JsonType::make_array();
        TEST_OK(j.to_string() == sample);
    }
    
    {
        ADD_TESTS(1);
        char const * sample = "{}";
        JsonType j = JsonType::make_object();
        TEST_OK(j.to_string() == sample);
    }

    {
        ADD_TESTS(1);
        char const * sample = "{\"a\":{\"b\":{\"c\":{}}}}";
        JsonType j = JsonType::make_object();
        j["a"] = JsonType::make_object();
        j["a"]["b"] = JsonType::make_object();
        j["a"]["b"]["c"] = JsonType::make_object();
        TEST_OK(j.to_string() == sample);
    }

    {
        ADD_TESTS(1);
        char const * sample = "[5]";
        JsonType j = JsonType::make_array();
        j.push_back(5);
        TEST_OK(j.to_string() == sample);
    }

    {
        ADD_TESTS(1);
        char const * sample = "[\"a\"]";
        JsonType j = JsonType::make_array();
        j.push_back("a");
        TEST_OK(j.to_string() == sample);
    }

    {
        ADD_TESTS(1);
        char const * sample = "[5,6,7,\"a\"]";
        JsonType j = JsonType::make_array();
        j.push_back(5);
        j.push_back(6);
        j.push_back(7);
        j.push_back("a");
        TEST_OK(j.to_string() == sample);
    }
#endif    
    {
        ADD_TESTS(1);

        string_type a("a");
        string_type b("b");
        a.append(b);
        a.append(pfs::to_string<string_type>(6));
        
        
        char const * sample = "{\"Image\":{\"Height\":600,\"IDs\":[116,943,234,38793],"
            "\"Thumbnail\":{\"Height\":125,\"Url\":\"http://www.example.com/image/481989943\","
            "\"Width\":\"100\"},\"Title\":\"View from 15th Floor\",\"Width\":800}}";
        
        JsonType j;

        j["Image"]["Width"]               = 800;
        j["Image"]["Height"]              = 600;
        j["Image"]["Title"]               = "View from 15th Floor";
        j["Image"]["Thumbnail"]["Url"]    = "http://www.example.com/image/481989943";
        j["Image"]["Thumbnail"]["Height"] = 125;
        j["Image"]["Thumbnail"]["Width"]  = "100";
        j["Image"]["IDs"].push_back(116);
        j["Image"]["IDs"].push_back(943);
        j["Image"]["IDs"].push_back(234);
        j["Image"]["IDs"].push_back(38793);
//        std::cout << "\n" << j.to_string() << "\n\n";
        
//        j.clear();
        
  //      std::cout << "\n" << j.to_string() << "\n\n";
//        
//        
//        j.parse(sample);
//        std::cout << "\n" << sample << "\n\n";
//        
//        std::cout << "\n" << j.to_string() << "\n\n";
        
        TEST_OK(j.to_string() == sample);
    }
}

}
