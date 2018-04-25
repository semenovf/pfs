#include "pfs/json/pretty_printer.hpp"

namespace test_pretty_printer {

template <typename JsonType>
void test ()
{
    typedef typename JsonType::string_type string_type;

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

    std::cout << "///////////////////////////////////////////////////////////////////////////\n";
    std::cout << "//                             Pretty Printer                            //\n";
    std::cout << "///////////////////////////////////////////////////////////////////////////\n";

    ADD_TESTS(8);

    JsonType json_object;

    TEST_FAIL(json_object.parse(plain_str) == pfs::error_code());
	TEST_FAIL(json_object.is_object());

	TEST_OK(pfs::to_string(json_object, pfs::json::style_plain) == plain_str);
//    std::cout << plain_str << std::endl;
//    std::cout << "=========================" << std::endl;
//    std::cout << pfs::to_string(json_object, pfs::json::style_plain) << std::endl;

	TEST_OK(pfs::to_string(json_object, pfs::json::style_kr) == json_object_kr_str);
//    std::cout << json_object_kr_str << std::endl;
//    std::cout << "=========================" << std::endl;
//    std::cout << pfs::to_string(json_object, pfs::json::style_kr) << std::endl;


    TEST_OK(pfs::to_string(json_object, pfs::json::style_bsd) == json_object_bsd_str);
//    std::cout << json_object_bsd_str << std::endl;
//    std::cout << "=========================" << std::endl;
//    std::cout << pfs::to_string(json_object, pfs::json::style_bsd) << std::endl;

    TEST_OK(pfs::to_string(json_object, pfs::json::style_gnu) == json_object_gnu_str);
//    std::cout << json_object_gnu_str << std::endl;
//    std::cout << "=========================" << std::endl;
//    std::cout << pfs::to_string(json_object, pfs::json::style_gnu) << std::endl;


    TEST_OK(pfs::to_string(json_object, pfs::json::style_whitesmiths) == json_object_ws_str);
//    std::cout << json_object_ws_str << std::endl;
//    std::cout << "=========================" << std::endl;
//    std::cout << pfs::to_string(json_object, pfs::json::style_whitesmiths) << std::endl;


    TEST_OK(pfs::to_string(json_object, pfs::json::style_favorite) == json_object_fav_str);
//    std::cout << json_object_fav_str << std::endl;
//    std::cout << "=========================" << std::endl;
//    std::cout << pfs::to_string<string_type>(json_object, pfs::json::style_favorite) << std::endl;
}

}

