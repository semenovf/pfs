#include <pfs/test/test.hpp>
#include "pfs/json/json.hpp"

namespace test_serialize {

template <typename JsonType>
void test ()
{
    typedef typename JsonType::string_type string_type;
    
	const char * plain_str = "{\"array\":[[200,300],\"abcd\",100,[200,300],{},[],"
			"{\"bar\":\"hello\",\"fee\":[100,200],\"foo\":100}],"
			"\"object\":{\"bar\":\"hello\",\"fee\":[100,200],\"foo\":100}}";

    ADD_TESTS(3);

//    pfs::pack_context pctx;
    pfs::byte_ostream os;

    JsonType j1;
    
    TEST_OK(j1.parse(plain_str) == pfs::error_code());

    os << j1;

//    pfs::unpack_context uctx(pctx.buffer.cbegin(), pctx.buffer.cend());
//    json j2;
//    
//    TEST_FAIL(pfs::unpack(uctx, j2));
//    
//    TEST_OK(pfs::to_string(j2) == plain_str);
//    //std::cout << pfs::to_string(j2) << std::endl;
}

}