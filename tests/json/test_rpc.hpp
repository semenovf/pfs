#pragma once
#include "pfs/json/rpc.hpp"
#include "pfs/json/ubjson_istream.hpp"
#include "pfs/json/ubjson_ostream.hpp"

namespace test_rpc
{

template <typename json_type>
struct serializer
{
    pfs::byte_string operator () (json_type const & value) const
    {
        return pfs::json::to_ubjson(value, pfs::json::UBJSON_FULL_OPTIMIZED);
    }

    pfs::error_code operator () (pfs::byte_string const & bytes, json_type & result) const
    {
        pfs::error_code ec;
        result = pfs::json::from_ubjson<json_type>(bytes, ec);
        return ec;
    }
    
    pfs::error_code operator () (pfs::byte_string::const_iterator first
            , pfs::byte_string::const_iterator last
            , json_type & result) const
    {
        pfs::error_code ec;
        result = pfs::json::from_ubjson<json_type>(first, last, ec);
        return ec;
    }
};

struct id_generator
{
    typedef int32_t type;
    
    id_generator () : id(0) {}

    type next_id ()
    {
        return ++id;
    }

private:
    type id;
};

template <typename json_type>
void test ()
{
    std::cout << "///////////////////////////////////////////////////////////////////////////\n";
    std::cout << "//                       Test JSON-RPC basics                            //\n";
    std::cout << "///////////////////////////////////////////////////////////////////////////\n";
    
    ADD_TESTS(4);

    typedef pfs::json::rpc<json_type, id_generator::type, 2, 0> rpc_ns;
    typedef typename rpc_ns::method method_t;
    typedef typename rpc_ns::notification notification_t;
    typedef typename rpc_ns::request request_t;

    id_generator idgen;
    serializer<json_type> ar;

    // Call method with no parameters
    {
        json_type value;
        method_t m(value, idgen.next_id(), "method0");
        
        std::cout << pfs::to_string(value) << std::endl;
        
        pfs::byte_string payload = ar(value);
        
        // Pack payload into packet ...
        
        // ... Transfer packet ...
        
        // ... Receive packet and unpack it ...
        
        // Handle payload
        
        json_type rq;
        
        // Parse received
        TEST_OK2(ar(payload, rq) == pfs::error_code(), "Parse received packet");
        
        std::cout << pfs::to_string(rq) << std::endl;
        
        request_t request(rq);
        
        TEST_OK2(request.is_method(), "Request is a method");
        TEST_OK2(request.method() == "method0", "Requested method name is `method0`");
        TEST_OK2(!request.has_params(), "Method `method0` has no parameters");
        
        // Prepare success response
        
        
    }
}

} // namespace test_rpc
