#ifndef TEST_PARSE_HPP
#define TEST_PARSE_HPP

#include <iostream>

#include "pfs/json/json.hpp"
#include "pfs/fsm/fsm.hpp"
#include "pfs/fsm/test.hpp"

namespace test_parse {

#if __UNCOMMENT__

static const char * empty_object_str = "{}";
static const char * empty_array_str = "[]";

static const char * object_str = "{                               \
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
	 }";

static const char * array_str = "   [       \
		  {                                      \
			 \"precision\": \"zip\",             \
			 \"Latitude\":  37.7668,             \
			 \"Longitude\": -122.3959,           \
			 \"Address\":   \"\",                \
			 \"City\":      \"SAN FRANCISCO\",   \
			 \"State\":     \"CA\",              \
			 \"Zip\":       \"94107\",           \
			 \"Country\":   \"US\"               \
		  },                                     \
		  {                                      \
			 \"precision\": \"zip\",             \
			 \"Latitude\":  37.371991,           \
			 \"Longitude\": -122.026020,         \
			 \"Address\":   \"\",                \
			 \"City\":      \"SUNNYVALE\",       \
			 \"State\":     \"CA\",              \
			 \"Zip\":       \"94085\",           \
			 \"Country\":   \"US\"               \
		  }                                      \
	   ]";

#endif

template <typename JsonType>
void test_fsm ()
{
    ADD_TESTS(49);

    typedef typename JsonType::string_type string_type;
    typedef pfs::json::grammar<JsonType> grammar_type;

    typedef typename grammar_type::iterator iterator;
    typedef pfs::fsm::test_valid_entry<typename grammar_type::iterator> test_valid_entry;
    //typedef pfs::fsm::test_invalid_entry<typename grammar_type::unicode_iterator> test_invalid_entry;
    
    grammar_type grammar;
    
    static string_type const DIGIT9_1("987654321");
    
    TEST_OK(test_valid_entry()(grammar.p_decimal_num_tr
        , 0
        , iterator(DIGIT9_1.begin())
        , iterator(DIGIT9_1.end())));
    
    // number = [ minus ] int [ frac ] [ exp ]
    TEST_OK(test_valid_entry()(grammar.p_number_tr
            , 0
            , iterator(DIGIT9_1.begin())
            , iterator(DIGIT9_1.end())));
//    TEST_OK(test_valid_entry<string_type>(pfs::json::number_fsm, 0, "-12345.6789"));
//    TEST_OK(test_valid_entry<string_type>(pfs::json::number_fsm, 0, "-123e+543"));
//    TEST_OK(test_valid_entry<string_type>(pfs::json::number_fsm, 0, "987.2345e+234"));

#if __COMMENT__    
    // exp = e [ minus / plus ] 1*DIGIT
    //
    TEST_OK(test_valid_entry<pfs::string>(pfs::json::exp_fsm, 0, _u8("e+1234")));
    TEST_OK(test_valid_entry<pfs::string>(pfs::json::exp_fsm, 0, _u8("E-987654321")));
    TEST_OK(test_valid_entry<pfs::string>(pfs::json::exp_fsm, 0, _u8("e1234")));
    TEST_OK(test_valid_entry<pfs::string>(pfs::json::exp_fsm, 0, _u8("E987654321")));

    // frac = decimal-point 1*DIGIT
    //
    TEST_OK(test_valid_entry<pfs::string>(pfs::json::frac_fsm, 0, _u8(".123456")));
    TEST_OK(test_valid_entry<pfs::string>(pfs::json::frac_fsm, 0, _u8(".0123456789")));

    TEST_OK(test_invalid_entry<pfs::string>(pfs::json::frac_fsm, 0, _u8(".abcd"), -1));
    TEST_OK(test_invalid_entry<pfs::string>(pfs::json::frac_fsm, 0, _u8(".012c"), 4));
    TEST_OK(test_invalid_entry<pfs::string>(pfs::json::frac_fsm, 0, _u8("1234"), -1));

    // int = zero / ( digit1-9 *DIGIT )
    //
    TEST_OK(test_valid_entry<pfs::string>(pfs::json::int_fsm, 0, _u8("0")));
    TEST_OK(test_valid_entry<pfs::string>(pfs::json::int_fsm, 0, _u8("123456789")));

    TEST_OK(test_invalid_entry<pfs::string>(pfs::json::int_fsm, 0, _u8("01234"), 1));
    TEST_OK(test_invalid_entry<pfs::string>(pfs::json::int_fsm, 0, _u8("A0"), -1));

    // number = [ minus ] int [ frac ] [ exp ]
    //
    TEST_OK(test_valid_entry<pfs::string>(pfs::json::number_fsm, 0, _u8("987654321")));
    TEST_OK(test_valid_entry<pfs::string>(pfs::json::number_fsm, 0, _u8("-12345.6789")));
    TEST_OK(test_valid_entry<pfs::string>(pfs::json::number_fsm, 0, _u8("-123e+543")));
    TEST_OK(test_valid_entry<pfs::string>(pfs::json::number_fsm, 0, _u8("987.2345e+234")));

    // %x75 4HEXDIG )  ; uXXXX
    //
    TEST_OK(test_valid_entry<pfs::string>(pfs::json::unicode_char_fsm, 0, _u8("u1234")));
    TEST_OK(test_valid_entry<pfs::string>(pfs::json::unicode_char_fsm, 0, _u8("uABCD")));
    TEST_OK(test_valid_entry<pfs::string>(pfs::json::unicode_char_fsm, 0, _u8("uFFFF")));
    TEST_OK(test_valid_entry<pfs::string>(pfs::json::unicode_char_fsm, 0, _u8("u1a2c")));

    TEST_OK(test_invalid_entry<pfs::string>(pfs::json::unicode_char_fsm, 0, _u8("UX124"), -1));
    TEST_OK(test_invalid_entry<pfs::string>(pfs::json::unicode_char_fsm, 0, _u8("uFEAR"), -1));

    // char = unescaped /
    //	escape (reference
    //		%x22 /          ; "    quotation mark  U+0022
    //		%x5C /          ; \    reverse solidus U+005C
    //		%x2F /          ; /    solidus         U+002F
    //		%x62 /          ; b    backspace       U+0008
    //		%x66 /          ; f    form feed       U+000C
    //		%x6E /          ; n    line feed       U+000A
    //		%x72 /          ; r    carriage return U+000D
    //		%x74 /          ; t    tab             U+0009
    //		%x75 4HEXDIG )  ; uXXXX                U+XXXX
    //
    TEST_OK(test_valid_entry<pfs::string>(pfs::json::char_fsm, 0, _u8("\\\"")));
    TEST_OK(test_valid_entry<pfs::string>(pfs::json::char_fsm, 0, _u8("\\\\")));
    TEST_OK(test_valid_entry<pfs::string>(pfs::json::char_fsm, 0, _u8("\\/")));
    TEST_OK(test_valid_entry<pfs::string>(pfs::json::char_fsm, 0, _u8("\\b")));
    TEST_OK(test_valid_entry<pfs::string>(pfs::json::char_fsm, 0, _u8("\\f")));
    TEST_OK(test_valid_entry<pfs::string>(pfs::json::char_fsm, 0, _u8("\\n")));
    TEST_OK(test_valid_entry<pfs::string>(pfs::json::char_fsm, 0, _u8("\\r")));
    TEST_OK(test_valid_entry<pfs::string>(pfs::json::char_fsm, 0, _u8("\\t")));
    TEST_OK(test_valid_entry<pfs::string>(pfs::json::char_fsm, 0, _u8("\\u0123")));
    TEST_OK(test_valid_entry<pfs::string>(pfs::json::char_fsm, 0, _u8("\\uABCD")));

    // string = quotation-mark *char quotation-mark
    //
    TEST_OK(test_valid_entry<pfs::string>(pfs::json::string_fsm, 0, _u8("\"This is a string with unescaped characters at all\"")));
    TEST_OK(test_valid_entry<pfs::string>(pfs::json::string_fsm, 0, _u8("\"This is a string with \\\" double quote inside\"")));
    TEST_OK(test_valid_entry<pfs::string>(pfs::json::string_fsm, 0, _u8("\"This \\b string \\n contains \\\\ unescaped \\t characters \\r \\/ \\\"\"")));
    TEST_OK(test_valid_entry<pfs::string>(pfs::json::string_fsm, 0, _u8("\"\\u0030\\u0031\\u0032\\u0033\\uF100\\uABCD\\u0020\\u0020\\u0020\\u0020\\u0020\\u0020\"")));


    // member = string name-separator value
    //
    TEST_OK(test_valid_entry<pfs::string>(pfs::json::member_fsm, 0, _u8("\"width\": 0")));

    // object = begin-object [ member *( value-separator member ) ] end-object
    //
    TEST_OK(test_valid_entry<pfs::string>(pfs::json::object_fsm, 0, _u8("{ }")));
    TEST_OK(test_valid_entry<pfs::string>(pfs::json::object_fsm, 0, _u8(" {\"Width\":  800} ")));
    TEST_OK(test_valid_entry<pfs::string>(pfs::json::object_fsm, 0, _u8(" {\"Width\":  800, \"Height\": 600 }")));

    TEST_OK(test_valid_entry<pfs::string>(pfs::json::json_fsm, 0, pfs::string(json_object_str)));
    TEST_OK(test_valid_entry<pfs::string>(pfs::json::json_fsm, 0, pfs::string(json_array_str)));

    static const pfs::string objstr1("{\"m\":0}");

    TEST_OK(test_valid_entry<pfs::string>(pfs::json::json_fsm, 0, objstr1));

    static const pfs::string objstr2("{\n"
            "\"modules\": [\n"
            "  {\n"
            "    \"name\": \"mod-link-msod-d\"\n"
            "    , \"active\": true\n"
            "    , \"msod\" : [\n"
            "    {\n"
            "       \"route-number\" : 0\n"
            "     , \"transmitter-number\" : 1\n"
            "     , \"addr\" : \"tcp://192.168.10.4:5042\"\n"
            "    }\n"
            "   ]\n"
            "  }\n"
            " ]\n"
            "}");

    TEST_OK(test_valid_entry<pfs::string>(pfs::json::json_fsm, 0, objstr2));
#endif    
}

static char const * json_empty_object_str = "{}";
static char const * json_empty_array_str  = "[]";

static char const * json_object_str = 
    "{"
	    "\"Image\": {"
		    "\"Width\":  800,"
		    "\"Height\": 600,"
		    "\"Title\":  \"View from 15th Floor\","
		    "\"Thumbnail\": {"
		        "\"Url\":    \"http://www.example.com/image/481989943\","
			    "\"Height\": 125.98,"
			    "\"Width\":  \"100.75\""
		    "},"
		    "\"IDs\": [116, 943, 234, 38793]"
	     "}"
	"}";

static char const * json_invalid_str =
    "[\n"
		"{\n"
            "\"Latitude\":  37;7668\n"
		"},"
		"{"
            "\"Latitude\":  37.371991"
		"}"
	"]";

template <typename JsonType>
void test_parse ()
{
    typedef typename JsonType::string_type string_type;
    
    ADD_TESTS(26);
    
	JsonType json_empty_object;
	TEST_OK(json_empty_object.is_null());
	TEST_OK(json_empty_object.parse(json_empty_object_str) == pfs::error_code());
	TEST_OK(!json_empty_object.is_null());
	TEST_OK(json_empty_object.is_object());

	JsonType json_empty_array;
	TEST_OK(json_empty_array.is_null());
	TEST_OK(json_empty_array.parse(json_empty_array_str) == pfs::error_code());
	TEST_OK(!json_empty_array.is_null());
	TEST_OK(json_empty_array.is_array());

    JsonType json_object;
    TEST_OK(json_object.parse(json_object_str) == pfs::error_code());
    TEST_OK(json_object.is_object());
	TEST_OK(json_object.size() > 0);
	TEST_OK(json_object["Image"]["Width"].template get<int>() == 800);
	TEST_OK(json_object["Image"]["Height"].template get<int>() == 600);
    TEST_OK(json_object["Image"]["Title"].template get<string_type>() == "View from 15th Floor");
    
	TEST_OK(json_object["Image"]["Thumbnail"]["Url"].template get<string_type>() == "http://www.example.com/image/481989943");
    
	TEST_OK(pfs::to_string<string_type>(json_object["Image"]["Thumbnail"]["Height"].template get<double>(), 'f', 2) == "125.98");
	TEST_OK(json_object["Image"]["Thumbnail"]["Width"].template get<string_type>() == "100.75");
    TEST_OK(pfs::to_string<string_type>(json_object["Image"]["Thumbnail"]["Width"].template get<double>(), 'f', 2) == "100.75");
	TEST_OK(pfs::to_string<string_type>(json_object["Image"]["IDs"][size_t(0)].template get<double>(), 'f', 0) == "116");
	TEST_OK(pfs::to_string<string_type>(json_object["Image"]["IDs"][1].template get<double>(), 'f', 0) == "943");
	TEST_OK(pfs::to_string<string_type>(json_object["Image"]["IDs"][2].template get<double>(), 'f', 0) == "234");
	TEST_OK(pfs::to_string<string_type>(json_object["Image"]["IDs"][3].template get<double>(), 'f', 0) == "38793");
	TEST_OK(pfs::to_string<string_type>(json_object["Image"]["IDs"][4].template get<double>(), 'f', 0) == "0");
	TEST_OK(json_object["Image"]["Unknown"].is_null());
    
    JsonType json_invalid;
    TEST_OK(json_invalid.parse(json_invalid_str) == pfs::json_errc::bad_json);
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
//	TEST_OK(json_array.size() > 0);
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

template <typename JsonType>
void test ()
{
//    test_fsm<JsonType>();
    test_parse<JsonType>();
}

} // test_parse

#endif // TEST_PARSE_HPP