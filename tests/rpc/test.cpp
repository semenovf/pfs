#include <iostream>
#include <pfs/test.hpp>
#include <pfs/rpc.hpp>

typedef pfs::rpc<1, 0> rpc_ns;

static int server_counter = 0;

struct method_pool
{
    rpc_ns::shared_response method1 (rpc_ns::id_type id, rpc_ns::server::request & rq)
    {
        std::cout << "method_pool::method1()" << std::endl;
        ++server_counter;
        return rpc_ns::make_success(id);
    }

    rpc_ns::shared_response notify1 (rpc_ns::server::request & rq)
    {
        std::cout << "method_pool::notify1()" << std::endl;
        ++server_counter;
        return rpc_ns::make_none();
    }

    rpc_ns::shared_response faulty_method (rpc_ns::id_type id, rpc_ns::server::request & rq)
    {
        std::cout << "method_pool::faulty_method()" << std::endl;
         ++server_counter;
        return rpc_ns::make_error(id, rpc_ns::INVALID_PARAMS);
    }

    rpc_ns::shared_response faulty_notify (rpc_ns::server::request & rq)
    {
        std::cout << "method_pool::faulty_notify()" << std::endl;
         ++server_counter;
        return rpc_ns::make_error(rpc_ns::INVALID_PARAMS);
    }
};

rpc_ns::shared_response method2 (rpc_ns::id_type id, rpc_ns::server::request & rq)
{
    std::cout << "method2()" << std::endl;
    ++server_counter;
    return rpc_ns::make_success(id);
}

rpc_ns::shared_response notify2 (rpc_ns::server::request & rq)
{
    std::cout << "notify2()" << std::endl;
    ++server_counter;
    return rpc_ns::make_none();
}

int main ()
{
    BEGIN_TESTS(21);

    rpc_ns::id_type idc = 0;

    method_pool mpool;
    rpc_ns::server server;
    rpc_ns::client client;

    server.bind("method1", & method_pool::method1, mpool);
    server.bind("method2", & method2);
//    server.bind("notify1", & method_pool::notify1, mpool);
//    server.bind("notify2", & notify2);
    server.bind("faulty_method", & method_pool::faulty_method, mpool);
//    server.bind("faulty_notify", & method_pool::faulty_notify, mpool);

//     rpc_ns::shared_request rq1 = client.make_request(++idc, "method1");
//     rpc_ns::shared_request rq2 = client.make_request(++idc, "method2");
//     rpc_ns::shared_request rq3 = client.make_request(++idc, "faulty_method");
//     rpc_ns::shared_request rq4 = client.make_request(++idc, "unknown_method");
//     rpc_ns::shared_request nq1 = client.make_notification("notify1");
//     rpc_ns::shared_request nq2 = client.make_notification("notify2");
//     rpc_ns::shared_request nq3 = client.make_notification("faulty_notify");
//     rpc_ns::shared_request nq4 = client.make_notification("unknown_notify");

//     rpc_ns::shared_response rp1 = server.exec(rq1);
//     rpc_ns::shared_response rp2 = server.exec(rq2);
//     rpc_ns::shared_response rp3 = server.exec(rq3);
//     rpc_ns::shared_response rp4 = server.exec(rq4);
//     rpc_ns::shared_response np1 = server.exec(nq1);
//     rpc_ns::shared_response np2 = server.exec(nq2);
//     rpc_ns::shared_response np3 = server.exec(nq3);
//     rpc_ns::shared_response np4 = server.exec(nq4);
//
//     TEST_OK2(server_counter == 6, "All methods/notifications executed");
//     TEST_OK(rq1->is_request());
//     TEST_OK(rq2->is_request());
//     TEST_OK(rq3->is_request());
//     TEST_OK(rq4->is_request());
//     TEST_OK(nq1->is_notification());
//     TEST_OK(nq2->is_notification());
//     TEST_OK(nq3->is_notification());
//     TEST_OK(nq4->is_notification());
//
//     TEST_OK(rp1->is_success());
//     TEST_OK(rp2->is_success());
//     TEST_OK(rp3->is_error());
//     TEST_OK(rp4->is_error());
//     TEST_OK(rp3->code() == rpc_ns::INVALID_PARAMS);
//     TEST_OK(rp4->code() == rpc_ns::METHOD_NOT_FOUND);
//
//     TEST_OK(!np1);
//     TEST_OK(!np2);
//     TEST_OK(np3 && np3->is_error());
//     TEST_OK(np4 && np4->is_error());
//     TEST_OK(np3->code() == rpc_ns::INVALID_PARAMS);
//     TEST_OK(np4->code() == rpc_ns::METHOD_NOT_FOUND);

    return END_TESTS;
}
