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
        
        static char const * array_source = "["
            "null,"
            "true,"
            "false,"
            "4782345193,"
            "153.132,"
            "\"ham\""
        "]";
        
        char const * array_ubjsoned = "[ZTFl4782345193d153.132Si3ham]";

        JsonType j;

        TEST_OK(j.parse(array_source) == pfs::error_code());
    
        pfs::byte_ostream os;
        ubjson_ostream_t uos(os);
        uos << j;
        
        TEST_OK(os.data() == pfs::byte_string(array_ubjsoned));
    }

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