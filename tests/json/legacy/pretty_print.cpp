/*
 * pretty_print.cpp
 *
 *  Created on: Dec 3, 2015
 *      Author: wladt
 */
#include <pfs/test/test.hpp>
#include "pfs/json/json.hpp"

void test_json_pretty_print ()
{
//    test_json_pretty_print1();
//    return;
    
    
	const char * plain_str = "{\"array\":[[200,300],\"abcd\",100,[200,300],{},[],"
			"{\"bar\":\"hello\",\"fee\":[100,200],\"foo\":100}],"
			"\"object\":{\"bar\":\"hello\",\"fee\":[100,200],\"foo\":100}}";

	const char * json_object_kr_str =
"{\n\
    \"array\": [\n\
        [\n\
            200,\n\
            300\n\
        ],\n\
        \"abcd\",\n\
        100,\n\
        [\n\
            200,\n\
            300\n\
        ],\n\
        {},\n\
        [],\n\
        {\n\
            \"bar\": \"hello\",\n\
            \"fee\": [\n\
                100,\n\
                200\n\
            ],\n\
            \"foo\": 100\n\
        }\n\
    ],\n\
    \"object\": {\n\
        \"bar\": \"hello\",\n\
        \"fee\": [\n\
            100,\n\
            200\n\
        ],\n\
        \"foo\": 100\n\
    }\n\
}";

	const char * json_object_bsd_str =
"{\n\
    \"array\":\n\
    [\n\
        [\n\
            200,\n\
            300\n\
        ],\n\
        \"abcd\",\n\
        100,\n\
        [\n\
            200,\n\
            300\n\
        ],\n\
        {},\n\
        [],\n\
        {\n\
            \"bar\": \"hello\",\n\
            \"fee\":\n\
            [\n\
                100,\n\
                200\n\
            ],\n\
            \"foo\": 100\n\
        }\n\
    ],\n\
    \"object\":\n\
    {\n\
        \"bar\": \"hello\",\n\
        \"fee\":\n\
        [\n\
            100,\n\
            200\n\
        ],\n\
        \"foo\": 100\n\
    }\n\
}";

//while (x == y)
//  {
//    something();
//    somethingelse();
//  }
    const char * json_object_gnu_str =
"{\n\
  \"array\":\n\
    [\n\
      [\n\
        200,\n\
        300\n\
      ],\n\
      \"abcd\",\n\
      100,\n\
      [\n\
        200,\n\
        300\n\
      ],\n\
      {},\n\
      [],\n\
      {\n\
        \"bar\": \"hello\",\n\
        \"fee\":\n\
          [\n\
            100,\n\
            200\n\
          ],\n\
        \"foo\": 100\n\
      }\n\
    ],\n\
  \"object\":\n\
    {\n\
      \"bar\": \"hello\",\n\
      \"fee\":\n\
        [\n\
          100,\n\
          200\n\
        ],\n\
      \"foo\": 100\n\
    }\n\
}";

    const char * json_object_ws_str =
"{\n\
\"array\":\n\
    [\n\
    [\n\
    200,\n\
    300\n\
    ],\n\
    \"abcd\",\n\
    100,\n\
    [\n\
    200,\n\
    300\n\
    ],\n\
    {},\n\
    [],\n\
    {\n\
    \"bar\": \"hello\",\n\
    \"fee\":\n\
        [\n\
        100,\n\
        200\n\
        ],\n\
    \"foo\": 100\n\
    }\n\
    ],\n\
\"object\":\n\
    {\n\
    \"bar\": \"hello\",\n\
    \"fee\":\n\
        [\n\
        100,\n\
        200\n\
        ],\n\
    \"foo\": 100\n\
    }\n\
}";

    const char * json_object_fav_str =
"{\n\
      \"array\": [\n\
          [\n\
              200\n\
            , 300\n\
        ]\n\
        , \"abcd\"\n\
        , 100\n\
        , [\n\
              200\n\
            , 300\n\
        ]\n\
        , {}\n\
        , []\n\
        , {\n\
              \"bar\": \"hello\"\n\
            , \"fee\": [\n\
                  100\n\
                , 200\n\
            ]\n\
            , \"foo\": 100\n\
        }\n\
    ]\n\
    , \"object\": {\n\
          \"bar\": \"hello\"\n\
        , \"fee\": [\n\
              100\n\
            , 200\n\
        ]\n\
        , \"foo\": 100\n\
    }\n\
}";

    ADD_TESTS(8);

    using pfs::json::json;

    json json_object;
    
    TEST_FAIL(pfs::json::parse(json_object, _u8(plain_str)));
	TEST_FAIL(json_object.is_object());

	TEST_OK(pfs::to_string(json_object, pfs::json::style_plain) == _u8(plain_str));
//    std::cout << plain_str << std::endl;
//    std::cout << "=========================" << std::endl;
//    std::cout << pfs::to_string(json_object, pfs::json::style_plain) << std::endl;
    
	TEST_OK(pfs::to_string(json_object, pfs::json::style_kr) == _u8(json_object_kr_str));
//    std::cout << json_object_kr_str << std::endl;
//    std::cout << "=========================" << std::endl;
//    std::cout << pfs::to_string(json_object, pfs::json::style_kr) << std::endl;


    TEST_OK(pfs::to_string(json_object, pfs::json::style_bsd) == _u8(json_object_bsd_str));
//    std::cout << json_object_bsd_str << std::endl;
//    std::cout << "=========================" << std::endl;
//    std::cout << pfs::to_string(json_object, pfs::json::style_bsd) << std::endl;

    TEST_OK(pfs::to_string(json_object, pfs::json::style_gnu) == _u8(json_object_gnu_str));
//    std::cout << json_object_gnu_str << std::endl;
//    std::cout << "=========================" << std::endl;
//    std::cout << pfs::to_string(json_object, pfs::json::style_gnu) << std::endl;


    TEST_OK(pfs::to_string(json_object, pfs::json::style_whitesmiths) == _u8(json_object_ws_str));
//    std::cout << json_object_ws_str << std::endl;
//    std::cout << "=========================" << std::endl;
//    std::cout << pfs::to_string(json_object, pfs::json::style_whitesmiths) << std::endl;


    TEST_OK(pfs::to_string(json_object, pfs::json::style_favorite) == _u8(json_object_fav_str));
//    std::cout << json_object_fav_str << std::endl;
//    std::cout << "=========================" << std::endl;
//    std::cout << pfs::to_string(json_object, pfs::json::style_favorite) << std::endl;
}

