/**
 * @file   uri.c
 * @author wladt
 * @date   Oct 12, 2012 11:18:36 AM
 *
 * @brief URI testing
 */

#include <iostream>
#include <pfs/test/test.hpp>
//#include <pfs/typeinfo.hpp>
#include <pfs/fsm/test.hpp>
#include "pfs/json/json.hpp"
#include "../../src/pfs-json/rfc4627.hpp"

using std::cout;
using std::endl;

extern void test_json_parse ();
extern void test_json_stringify ();
extern void test_json_pretty_print ();
extern void test_json_iterator ();
extern void test_json_pack ();

static const char * json_empty_object_str = "{}";
static const char * json_empty_array_str = "[]";

static const char * json_object_str = "{                               \
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

static const char * json_array_str = "   [       \
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

#if __TODO__
/*
static const char * json_object_str1 = "                                                            \
{                                                                                                   \
      \"sep_char\"     : \",\"                                                                      \
    , \"quote_char\"   : \"\\\"\"                                                                   \
    , \"not_embed_nl\" : 0                                                                          \
    , \"encoding_in\"  : \"utf8\"                                                                   \
    , \"encoding_out\" : \"utf8\"                                                                   \
    , \"skip_header\"  : 1                                                                          \
    , \"comment_str\"  : \"#\"                                                                      \
    , \"fields\"       : [                                                                          \
              { \"index\": 1, \"name\" :\"code\", \"type\" : \"CHAR(2)\", \"spec\" : \"PK\" }       \
            , { \"index\": 2, \"name\" :\"name\", \"type\" : \"TEXT(45)\", \"spec\" : \"\" }        \
            , { \"index\": 5, \"name\" :\"currency\", \"type\" : \"CHAR(3)\", \"spec\" : \"\" }     \
    ]                                                                                               \
}";
 */

#endif

using pfs::fsm::test_valid_entry;
using pfs::fsm::test_invalid_entry;

void test_json_fsm ()
{
    ADD_TESTS(49);

    // number = [ minus ] int [ frac ] [ exp ]
    //
    TEST_OK(test_valid_entry<pfs::string>(pfs::json::number_fsm, 0, _u8("987654321")));
    TEST_OK(test_valid_entry<pfs::string>(pfs::json::number_fsm, 0, _u8("-12345.6789")));
    TEST_OK(test_valid_entry<pfs::string>(pfs::json::number_fsm, 0, _u8("-123e+543")));
    TEST_OK(test_valid_entry<pfs::string>(pfs::json::number_fsm, 0, _u8("987.2345e+234")));

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
}

void test_json_null ()
{
    ADD_TESTS(1);
    pfs::json::value nullValue;
    TEST_OK(nullValue.is_null());
}

template <typename T>
void test_boolean_signed ()
{
    ADD_TESTS(6);
    cout << "\ntest_boolean_signed<" << pfs::type_name<T>() << ">()\n";

    pfs::json::value min(pfs::min_value<T>());
    pfs::json::value max(pfs::max_value<T>());
    pfs::json::value zero(T(0));
    TEST_OK(!min.is_null());
    TEST_OK(!max.is_null());
    TEST_OK(!zero.is_null());
    TEST_OK(min.get<bool>() == true);
    TEST_OK(max.get<bool>() == true);
    TEST_OK(zero.get<bool>() == false);
}

template <typename T>
void test_boolean_unsigned ()
{
    ADD_TESTS(4);
    cout << "\ntest_boolean_unsigned<" << pfs::type_name<T>() << ">()\n";

    pfs::json::value max(pfs::max_value<T>());
    pfs::json::value zero(T(0));
    TEST_OK(!max.is_null());
    TEST_OK(!zero.is_null());
    TEST_OK(max.get<bool>() == true);
    TEST_OK(zero.get<bool>() == false);
}

void test_json_boolean ()
{
    ADD_TESTS(14);

    pfs::json::value true_value(true);
    pfs::json::value false_value(false);
    TEST_OK(!true_value.is_null());
    TEST_OK(!false_value.is_null());
    TEST_OK(true_value.get<bool>() == true);
    TEST_OK(false_value.get<bool>() == false);

    pfs::json::value char_value('W');
    TEST_OK(!char_value.is_null());
    TEST_OK(char_value.get<bool>() == true);

    test_boolean_signed<signed char>();
    test_boolean_signed<short>();
    test_boolean_signed<int>();
    test_boolean_signed<long>();
#ifdef PFS_HAVE_LONGLONG
    test_boolean_signed<long long>();
#endif
    test_boolean_unsigned<unsigned char>();
    test_boolean_unsigned<unsigned short>();
    test_boolean_unsigned<unsigned int>();
    test_boolean_unsigned<unsigned long>();
#ifdef PFS_HAVE_LONGLONG
    test_boolean_unsigned<unsigned long long>();
#endif

    test_boolean_signed<float>();
    test_boolean_signed<double>();
    //#ifdef PFS_HAVE_LONG_DOUBLE
    //	test_boolean_signed<long double>();
    //#endif

    pfs::string s;
    pfs::json::value null_string_value(s);
    pfs::json::value empty_string_value(pfs::string(""));
    pfs::json::value nonempty_string_value(pfs::string("this is a string"));
    pfs::json::value true_string_value(pfs::string("true"));
    pfs::json::value false_string_value(pfs::string("false"));
    TEST_OK(!null_string_value.is_null());
    TEST_OK(!empty_string_value.is_null());
    TEST_OK(!nonempty_string_value.is_null());

    TEST_OK(null_string_value.get<bool>() == false);
    TEST_OK(empty_string_value.get<bool>() == false);
    TEST_OK(nonempty_string_value.get<bool>() == false);
    TEST_OK(true_string_value.get<bool>() == true);
    TEST_OK(false_string_value.get<bool>() == false);
}

template <typename T>
void test_integral_signed ()
{
    ADD_TESTS(3);

    cout << "\ntest_integral_signed<" << pfs::type_name<T>() << ">()\n";

    pfs::json::value min(pfs::min_value<T>());
    pfs::json::value max(pfs::max_value<T>());
    pfs::json::value zero(T(0));
    TEST_OK(min.get<intmax_t>() == intmax_t(pfs::min_value<T>()));
    TEST_OK(max.get<intmax_t>() == intmax_t(pfs::max_value<T>()));
    TEST_OK(zero.get<intmax_t>() == intmax_t(0));
}

template <typename T>
void test_integral_unsigned ()
{
    ADD_TESTS(2);

    cout << "\ntest_integral_unsigned<" << pfs::type_name<T>() << ">()\n";

    pfs::json::value max(pfs::max_value<T>());
    pfs::json::value zero(T(0));
    TEST_OK(uintmax_t(max.get<uintmax_t>()) == uintmax_t(pfs::max_value<T>()));
    TEST_OK(zero.get<uintmax_t>() == uintmax_t(0));
}

void test_json_integral ()
{
    ADD_TESTS(7);

    pfs::json::value true_value(true);
    pfs::json::value false_value(false);
    TEST_OK(true_value.get<intmax_t>() == intmax_t(1));
    TEST_OK(false_value.get<intmax_t>() == 0);

    pfs::json::value char_value('W');
    TEST_OK(char_value.get<intmax_t>() == intmax_t('W'));

    test_integral_signed<signed char>();
    test_integral_signed<short>();
    test_integral_signed<int>();
    test_integral_signed<long>();
#ifdef PFS_HAVE_LONGLONG
    test_integral_signed<long long>();
#endif
    test_integral_unsigned<unsigned char>();
    test_integral_unsigned<unsigned short>();
    test_integral_unsigned<unsigned int>();
    test_integral_unsigned<unsigned long>();
#ifdef PFS_HAVE_LONGLONG
    test_integral_unsigned<unsigned long long>();
#endif

    test_integral_signed<float>();
    test_integral_signed<double>();
    //#ifdef PFS_HAVE_LONG_DOUBLE
    //	test_integral_signed<long double>();
    //#endif

    pfs::string s;
    pfs::json::value null_string_value(s);
    pfs::json::value empty_string_value(pfs::string(""));
    pfs::json::value string_value1(pfs::string("12345"));
    pfs::json::value string_value2(pfs::string("QWERTY"));
    TEST_OK(null_string_value.get<int>() == 0);
    TEST_OK(empty_string_value.get<int>() == 0);
    TEST_OK(string_value1.get<int>() == 12345);
    TEST_OK(string_value2.get<int>() == 0);

    cout << string_value2.get<int>() << endl;
}

template <typename T>
void test_number_signed ()
{
    ADD_TESTS(3);
    cout << "\ntest_number_signed<" << pfs::type_name<T>() << ">()\n";

    pfs::json::value min(pfs::min_value<T>());
    pfs::json::value max(pfs::max_value<T>());
    pfs::json::value zero(T(0));

    TEST_OK(min.get<real_t>() == static_cast<real_t> (pfs::min_value<T>()));
    TEST_OK(max.get<real_t>() == static_cast<real_t> (pfs::max_value<T>()));
    TEST_OK(zero.get<real_t>() == real_t(0));
}

template <typename T>
void test_number_unsigned ()
{
    ADD_TESTS(2);

    cout << "\ntest_number_unsigned<" << pfs::type_name<T>() << ">()\n";

    pfs::json::value max(pfs::max_value<T>());
    pfs::json::value zero(T(0));
    TEST_OK(max.get<real_t>() == static_cast<real_t> (pfs::max_value<T>()));
    TEST_OK(zero.get<real_t>() == static_cast<real_t> (.0));

    //	real_t n1 = max.get<real_t>();
    //	T n2 = pfs::max_value<T>();
    //	real_t n22 = static_cast<real_t>(n2);
}

void test_json_number ()
{
    ADD_TESTS(7);

    pfs::json::value true_value(true);
    pfs::json::value false_value(false);
    TEST_OK(true_value.get<real_t>() == real_t(1.0));
    TEST_OK(false_value.get<real_t>() == 0);

    pfs::json::value char_value('W');
    TEST_OK(char_value.get<real_t>() == real_t('W'));

    test_number_signed<signed char>();
    test_number_signed<short>();
    test_number_signed<int>();
    test_number_signed<long>();
#ifdef PFS_HAVE_LONGLONG
    test_number_signed<long long>();
#endif
    test_number_unsigned<unsigned char>();
    test_number_unsigned<unsigned short>();
    test_number_unsigned<unsigned int>();
    test_number_unsigned<unsigned long>();
#ifdef PFS_HAVE_LONGLONG
    test_number_unsigned<unsigned long long>();
#endif

    test_number_signed<float>();
    test_number_signed<double>();
    //#ifdef PFS_HAVE_LONG_DOUBLE
    //	test_number_signed<long double>();
    //#endif

    pfs::string s;
    pfs::json::value null_string_value(s);
    pfs::json::value empty_string_value(pfs::string(""));
    pfs::json::value string_value1(_u8("0.12345678"));
    pfs::json::value string_value2(_u8("QWERTY"));
    TEST_OK(null_string_value.get<real_t>() == real_t(.0));
    TEST_OK(empty_string_value.get<real_t>() == real_t(.0));
    TEST_OK(string_value1.get<real_t>() == pfs::lexical_cast<real_t>(_u8("0.12345678")));
    TEST_OK(string_value2.get<real_t>() == real_t(.0));
}

void test_json_build ()
{
    ADD_TESTS(23);

    pfs::json::json json;

    json["Image"] = pfs::json::make_object();

    TEST_FAIL(json.is_object());
    TEST_FAIL(json["Image"].is_object());

    pfs::json::value & image = json["Image"];

    TEST_FAIL(image.is_object());
    TEST_FAIL(image.size() == 0);

    image["Width"] = 800;
    image["Height"] = 600;
    image["Title"] = _u8("View from 15th Floor");
    image["Thumbnail"] = pfs::json::make_object();

    TEST_FAIL(image.size() == 4);

    image = json["Image"];

    TEST_FAIL(image.size() == 4);

    pfs::json::value & thumbnail = image["Thumbnail"];
    thumbnail["Url"] = _u8("http://www.example.com/image/481989943");
    thumbnail["Height"] = 125;
    thumbnail["Width"] = _u8("100");

    image["IDs"] = pfs::json::make_array();
    pfs::json::value & ids = image["IDs"];
    ids.push_back(116);
    ids.push_back(943);
    ids.push_back(234);
    ids.push_back(38793);

    TEST_OK(json["Image"]["Width"].get<real_t>() == real_t(800));
    TEST_OK(json["Image"]["Height"].get<real_t>() == real_t(600));
    TEST_OK(json["Image"]["Title"].get<pfs::string>() == _u8("View from 15th Floor"));
    TEST_OK(json["Image"]["Thumbnail"]["Url"].get<pfs::string>() == _u8("http://www.example.com/image/481989943"));
    TEST_OK(json["Image"]["Thumbnail"]["Height"].get<unsigned int>() == 125);
    TEST_OK(json["Image"]["Thumbnail"]["Width"].get<pfs::string>() == _u8("100"));
    TEST_OK(json["Image"]["IDs"][size_t(0)].get<int>() == 116);
    TEST_OK(json["Image"]["IDs"][1].get<int>() == 943);
    TEST_OK(json["Image"]["IDs"][2].get<int>() == 234);
    TEST_OK(json["Image"]["IDs"][3].get<int>() == 38793);

    TEST_OK(json["Image"]["IDs"][4].get<int>() == 0); // out of bounds
    TEST_OK(json["Image"]["Unknown"].is_null());

    json["Image"]["IDs"].erase(3);
    TEST_OK(json["Image"]["IDs"][3].is_null());

    pfs::json::value jv_null;
    pfs::json::value jv_object(pfs::json::make_object());

    TEST_OK(jv_null.is_null());
    TEST_OK(jv_object.is_object());

    jv_null.swap(jv_object);

    TEST_OK(jv_null.is_object());
    TEST_OK(jv_object.is_null());
}

int main (int argc, char *argv[])
{
    PFS_UNUSED(argc);
    PFS_UNUSED(argv);

    BEGIN_TESTS(0);

    test_json_fsm();
    test_json_null();
    test_json_boolean();
    test_json_integral();
    test_json_number();
    test_json_build();
    test_json_parse();
    test_json_iterator();
    test_json_stringify();
    test_json_pretty_print();
    test_json_pack();

    return END_TESTS;
}
