#include <iostream>
#include <pfs/test.hpp>
#include <pfs/rpc.hpp>
#include <pfs/system_error.hpp>
#include <pfs/v2/byte_string.hpp>
#include <pfs/v2/binary_istream.hpp>
#include <pfs/v2/binary_ostream.hpp>
#include <pfs/v2/io/buffer.hpp>

class Protocol
{
public:
    typedef pfs::byte_string response_type;

    Protocol ()
        : _os(_buffer)
    {}

    void begin ()
    {
        _buffer.clear();
        _os << '\x10' << '\x01'
            << pfs::byte_string::size_type(0); // reserve for length
    }

    void commit ()
    {
        _os << '\x10' << '\x02';
    }

    template <typename T>
    inline Protocol & operator << (T const & x)
    {
        _os << x;
    }

    inline Protocol & operator << (pfs::string const & s)
    {
        _os << s.size();
        pfs::string::const_iterator first = s.cbegin();
        pfs::string::const_iterator last = s.cend();

        while (first != last) {
            _os << *first++;
        }
    }

private:
    pfs::byte_string _buffer;
    //pfs::binary_istream<pfs::byte_string> _is;
    pfs::binary_ostream<pfs::byte_string> _os;
};

class ServerTransport
{
public:
    ServerTransport () {}

    ssize_t send (pfs::byte_string const &, pfs::error_code & ec);
private:
};

class ClientTransport
{
public:
    ClientTransport () {}

    ssize_t send (pfs::byte_string const &, pfs::error_code & ec);
private:
};

typedef pfs::rpc<1, 0, Protocol> rpc_ns;


#if __TODO__
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

#endif

int main ()
{
    BEGIN_TESTS(0);

    rpc_ns::id_type idc = 0;

    Protocol sproto;
    Protocol cproto;
    ServerTransport stran;
    ClientTransport ctran;
    rpc_ns::server<ServerTransport> server(sproto, stran);
    rpc_ns::client<ClientTransport> client(cproto, ctran);

//    server.bind("method1", & method_pool::method1, mpool);

//     server.bind("method1", & method_pool::method1, mpool);
//     server.bind("method2", & method2);
// //    server.bind("notify1", & method_pool::notify1, mpool);
// //    server.bind("notify2", & notify2);
//     server.bind("faulty_method", & method_pool::faulty_method, mpool);
// //    server.bind("faulty_notify", & method_pool::faulty_notify, mpool);

    client.call("method1").result();
    client.call("method2")(123).result();
    client.call("notify1").notify();
    client.call("notify2")(123).notify();
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
