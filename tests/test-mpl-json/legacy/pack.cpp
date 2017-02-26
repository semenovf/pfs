/*
 * pretty_print.cpp
 *
 *  Created on: Dec 3, 2015
 *      Author: wladt
 */
//#include <pfs/iostream.hpp>
#include <pfs/test/test.hpp>
#include "pfs/json/json.hpp"

void test_json_pack ()
{
    using pfs::json::json;
    
	const char * plain_str = "{\"array\":[[200,300],\"abcd\",100,[200,300],{},[],"
			"{\"bar\":\"hello\",\"fee\":[100,200],\"foo\":100}],"
			"\"object\":{\"bar\":\"hello\",\"fee\":[100,200],\"foo\":100}}";

    ADD_TESTS(3);

    pfs::pack_context pctx;

    json j1;
    
    TEST_FAIL(pfs::json::parse(j1, _u8(plain_str)));
    
    pfs::pack(pctx, j1);
    
    pfs::unpack_context uctx(pctx.buffer.cbegin(), pctx.buffer.cend());
    json j2;
    
    TEST_FAIL(pfs::unpack(uctx, j2));
    
    TEST_OK(pfs::to_string(j2) == plain_str);
    //std::cout << pfs::to_string(j2) << std::endl;
}

