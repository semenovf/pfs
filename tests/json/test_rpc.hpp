#pragma once
#include "pfs/json/rpc.hpp"
#include "pfs/json/ubjson_istream.hpp"
#include "pfs/json/ubjson_ostream.hpp"

namespace test_rpc
{

template <typename json_type>
void test ()
{
    std::cout << "///////////////////////////////////////////////////////////////////////////\n";
    std::cout << "//                       Test JSON-RPC basics                            //\n";
    std::cout << "///////////////////////////////////////////////////////////////////////////\n";

    enum { RPC_MAJOR_VERSION = 2, RPC_MINOR_VERSION = 0 };

    typedef pfs::json::rpc<json_type> rpc_ns; // Is equivalent to below commented typedef
//     typedef pfs::json::rpc<json_type, pfs::json::id_generator
//             , pfs::json::ubjson_serializer<json_type>
//             , RPC_MAJOR_VERSION
//             , RPC_MINOR_VERSION> rpc_ns;

    typedef typename rpc_ns::request           request_t;
    typedef typename rpc_ns::response          response_t;
    typedef typename rpc_ns::id_generator_type id_generator_t;

    id_generator_t idgen;

    ///////////////////////////////////////////////////////////////////////////
    // Call method with no parameters and return success                     //
    ///////////////////////////////////////////////////////////////////////////
    {
        ADD_TESTS(10);

        request_t m = request_t::make_method(idgen, "method0");
        pfs::byte_string request_payload = request_t::serialize(m);

        // Pack payload into packet ...

        // ... Transfer packet ...

        // ... Receive packet and unpack it ...

        // Handle request payload

        // Parse request packet
        request_t request;
        TEST_OK2(request_t::deserialize(request_payload, request) == pfs::error_code(), "Parse received request packet");

        TEST_OK2(request.version_match(RPC_MAJOR_VERSION, RPC_MINOR_VERSION), "Request version matches sample");
        TEST_OK2(request.is_method(), "Request is a method");
        TEST_OK2(request.name() == "method0", "Requested method name is `method0`");
        TEST_OK2(!request.has_params(), "Method `method0` has no parameters");

        // Prepare success response
        response_t ok = response_t::make_success(request, true);
        pfs::byte_string response_payload = response_t::serialize(ok);

        // Pack payload into packet ...

        // ... Transfer packet ...

        // ... Receive packet and unpack it ...

        // Handle response payload

        response_t response;

        // Parse response packet
        TEST_OK2(response_t::deserialize(response_payload, response) == pfs::error_code(), "Parse received response packet");

        TEST_OK2(response.version_match(RPC_MAJOR_VERSION, RPC_MINOR_VERSION), "Response version matches sample");
        TEST_OK2(response.is_success(), "Success response");
        TEST_OK2(response.has_result(), "Response has result");
        TEST_OK2(response.template get<bool>() == true, "Response result value is `true`");
    }

    ///////////////////////////////////////////////////////////////////////////
    // Call method with one parameter and return failure                     //
    ///////////////////////////////////////////////////////////////////////////
    {
        ADD_TESTS(12);

        request_t m = request_t::make_method(idgen, "method1").arg(10);

        pfs::byte_string request_payload = request_t::serialize(m);
        std::cout << pfs::to_string(m.internal_data()) << std::endl;

        // Parse request packet
        request_t request;
        TEST_OK2(request_t::deserialize(request_payload, request) == pfs::error_code(), "Parse received request packet");

        std::cout << pfs::to_string(request.internal_data()) << std::endl;

        TEST_OK2(request.version_match(RPC_MAJOR_VERSION, RPC_MINOR_VERSION), "Request version matches sample");
        TEST_OK2(request.is_method(), "Request is a method");
        TEST_OK2(request.name() == "method1", "Requested method name is `method1`");
        TEST_FAIL2(request.has_params(), "Method `method1` has parameters");
        TEST_OK2(request.template get<int>(0) == 10, "Get first parameter for `method1`");

        // Prepare failure response
        response_t failure = response_t::make_failure(rpc_ns::INVALID_REQUEST, "Invalid request", request);
        pfs::byte_string response_payload = response_t::serialize(failure);

        // Parse response packet

        response_t response;
        TEST_OK2(response_t::deserialize(response_payload, response) == pfs::error_code(), "Parse received response packet");

        std::cout << pfs::to_string(response.internal_data()) << std::endl;

        TEST_OK2(response.version_match(RPC_MAJOR_VERSION, RPC_MINOR_VERSION), "Response version matches sample");
        TEST_OK2(response.is_failure(), "Response is failure");
        TEST_OK2(response.code() == rpc_ns::INVALID_REQUEST, "Error code is INVALID_REQUEST");
        TEST_OK2(response.message() == "Invalid request", "Error message is 'Invalid request'");
        TEST_OK2(!response.has_error_data(), "Failure has no addition information about an error");
    }

    // TODO
    ///////////////////////////////////////////////////////////////////////////
    // Call notification with no parameters                                  //
    ///////////////////////////////////////////////////////////////////////////
    {

    }

    // TODO Check failure with addition error has_error_data
    // TODO Check failure with INTERNAL_ERROR (not a response for method)
}

} // namespace test_rpc
