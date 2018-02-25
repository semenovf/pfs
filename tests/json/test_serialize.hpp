#include <sstream>
#include <pfs/test/test.hpp>
#include "pfs/json/json.hpp"
#include "pfs/json/ubjson.hpp"

namespace test_serialize {

template <typename JsonType>
void test ()
{
    typedef pfs::json::ubjson_ostream<pfs::byte_ostream, JsonType> ubjson_ostream_t;

    {
        ADD_TESTS(2);
        JsonType j;
        TEST_FAIL(j.parse("null") == pfs::error_code());
        TEST_OK(pfs::json::to_ubjson(j) == pfs::byte_string("Z"))
    }

    {
        ADD_TESTS(2);
        JsonType j;
        TEST_FAIL(j.parse("true") == pfs::error_code());
        TEST_OK(pfs::json::to_ubjson(j) == pfs::byte_string("T"))
    }

    {
        ADD_TESTS(2);
        JsonType j;
        TEST_FAIL(j.parse("false") == pfs::error_code());
        TEST_OK(pfs::json::to_ubjson(j) == pfs::byte_string("F"))
    }

    {
        char const * src[] = {
            "-128",
              "0", "-128", "127", "255"
            , "-32768", "32767", "65535"
            , "-2147483648", "2147483647", "4294967295"
#if PFS_HAVE_INT64
            , "-9223372036854775808", "9223372036854775807", "18446744073709551615"
#endif
        };


        // Results in big-endian order
        pfs::byte_string result[] = {
              pfs::byte_string("i\x80", 2)         //    -128
            , pfs::byte_string("i\x0", 2)          //       0
            , pfs::byte_string("i\x80", 2)         //    -128
            , pfs::byte_string("i\x7f", 2)         //     127
            , pfs::byte_string("U\xff", 2)         //     255
            , pfs::byte_string("I\x80\x00", 3)     //  -32768
            , pfs::byte_string("I\x7f\xff", 3)     //   32767
            , pfs::byte_string("I\xff\xff", 3)     //   65535
            , pfs::byte_string("l\x80\x00\x00\x00", 5)     // -2147483648
            , pfs::byte_string("l\x7f\xff\xff\xff", 5)     //  2147483647
            , pfs::byte_string("l\xff\xff\xff\xff", 5)     //  4294967295
#if PFS_HAVE_INT64
            , pfs::byte_string("L\x80\x00\x00\x00\x00\x00\x00\x00", 9)
            , pfs::byte_string("L\x7f\xff\xff\xff\xff\xff\xff\xff", 9)
            , pfs::byte_string("L\xff\xff\xff\xff\xff\xff\xff\xff", 9)
#endif
        };

        const int count = sizeof(src)/sizeof(src[0]);

        ADD_TESTS(2 * count);

        int i = 0;

        for (; i < count; i++) {
            JsonType j;

            std::stringstream ss1, ss2;

            ss1 << "j.parse(\"" << src[i] << ") == pfs::error_code()";
            ss2 << "to_ubjson(" << src[i] << ")";

            TEST_FAIL2(j.parse(src[i]) == pfs::error_code(), ss1.str().c_str());
            TEST_OK2(pfs::json::to_ubjson(j) == result[i], ss2.str().c_str());
        }
    }

//    {
//        ADD_TESTS(2);
//        JsonType j;
//        TEST_FAIL(j.parse("0") == pfs::error_code());
//        TEST_OK(pfs::json::to_ubjson(j) == pfs::byte_string("i\x0", 2))
//    }

    {
        ADD_TESTS(2);
        JsonType j;
        TEST_FAIL(j.parse("\"X\"") == pfs::error_code());
        TEST_OK(pfs::json::to_ubjson(j) == pfs::byte_string("CX"))
    }

    {
        ADD_TESTS(2);
        JsonType j;
        TEST_FAIL(j.parse("\"XYZ\"") == pfs::error_code());
        TEST_OK(pfs::json::to_ubjson(j) == pfs::byte_string("Si\x3XYZ"))
    }

//    {
//        ADD_TESTS(2);
//
//        static char const * array_source = "["
//            "null,"
//            "true,"
//            "false,"
//            "4782345193,"
//            "153.132,"
//            "\"ham\""
//        "]";
//
//        char const * array_ubjsoned = "[ZTFl4782345193D153.132Si3ham]";
//
//        JsonType j;
//
//        TEST_FAIL(j.parse(array_source) == pfs::error_code());
//
//        pfs::byte_ostream os;
//        ubjson_ostream_t uos(os);
//        uos << j;
//
//        std::cout << std::hex << os.data() << std::endl;
//        TEST_OK(os.data() == pfs::byte_string(array_ubjsoned));
//    }

//    char const * test_data[] = {
//        "[]"
//        , "{}"
//        , "[1,2,3]"
//        , "[1,\"hello\",3]"
//        , "{\"a\": 5}"
//        , "{\"array\":[[200,300],\"abcd\",100,[200,300],{},[],"
//			"{\"bar\":\"hello\",\"fee\":[100,200],\"foo\":100}],"
//			"\"object\":{\"bar\":\"hello\",\"fee\":[100,200],\"foo\":100}}"
//    };

//    for (size_t i = 0; i < sizeof(test_data)/sizeof(test_data[0]); i++) {
//        ADD_TESTS(2);
//
//        pfs::byte_ostream os;
//        JsonType j1;
//        JsonType j2;
//
//        TEST_OK(j1.parse(test_data[i]) == pfs::error_code());
//
//        os << j1;
//        pfs::byte_istream is(os.data().cbegin(), os.data().cend());
//        is >> j2;
//
//        TEST_OK(j1 == j2);
//    }
}

}
