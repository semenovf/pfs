#pragma once
#include "pfs/json/rpc.hpp"

namespace test_rpc
{

template <>
struct id_generator<int32_t> : pfs::id_generator<int32_t>
{
    id_generator () {}

    type next_id () const
    {
        static type id = 0;
        return ++id;
    }
};

template <typename json_type>
void test ()
{
    ADD_TESTS(0);

    typedef int32_t id_type;
    typedef pfs::json::rpc::method<json_type, id_type> method_t;
    typedef pfs::json::rpc::notification<json_type, id_type> notification_t;
    typedef pfs::json::rpc::success<json_type, id_type> success_t;
    typedef pfs::json::rpc::failure<json_type, id_type> failure_t;


    //

}

} // namespace test_rpc
