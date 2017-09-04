#include <pfs/test/test.hpp>
#include "pfs/json/json.hpp"

namespace test_serialize {

template <typename JsonType>
void test ()
{
    typedef typename JsonType::string_type string_type;
    
    char const * test_data[] = {
        "[]"
        , "{}"
        , "[1,2,3]"
        , "[1,\"hello\",3]"
        , "{\"a\": 5}"
        , "{\"array\":[[200,300],\"abcd\",100,[200,300],{},[],"
			"{\"bar\":\"hello\",\"fee\":[100,200],\"foo\":100}],"
			"\"object\":{\"bar\":\"hello\",\"fee\":[100,200],\"foo\":100}}"
    };
    
    for (int i = 0; i < sizeof(test_data)/sizeof(test_data[0]); i++) {
        ADD_TESTS(2);

        pfs::byte_ostream os;
        JsonType j1;
        JsonType j2;

        TEST_OK(j1.parse(test_data[i]) == pfs::error_code());

        os << j1;
        pfs::byte_istream is(os.data().cbegin(), os.data().cend());
        is >> j2;

        TEST_OK(j1 == j2);
    }
}

}