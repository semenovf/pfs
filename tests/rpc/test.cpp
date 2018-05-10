#include <iostream>
#include <pfs/test.hpp>
#include <pfs/system_error.hpp>
#include <pfs/byte_string.hpp>
#include <pfs/binary_istream.hpp>
#include <pfs/binary_ostream.hpp>
#include <pfs/net/inet4_addr.hpp>
#include <pfs/io/device_manager.hpp>
#include <pfs/io/inet_server.hpp>
#include <pfs/thread.hpp>
#include <pfs/logger.hpp>
#include "json/rpc.hpp"

typedef pfs::log<> log_ns;

static int const                  TCP_LISTENER_DEFAULT_BACKLOG = 50;
static pfs::net::inet4_addr const TCP_LISTENER_ADDR(127, 0, 0, 1);
static uint16_t const             TCP_LISTENER_PORT(9876);

struct simple_protocol
{
    simple_protocol () {}

    pfs::byte_string pack (pfs::byte_string const & payload) const
    {
        pfs::byte_string buffer;
        pfs::byte_string_ostream os(buffer);
        os << '\x10' << '\x01' << payload.size() << payload << '\x10' << '\x02';
        return buffer;
    }

    ssize_t unpack (pfs::byte_string const & buffer, pfs::byte_string & payload)
    {
        uint8_t a, b, c, d;
        pfs::byte_string::size_type sz;
        pfs::byte_string_istream is(buffer);
        is >> a >> b >> sz;
        is >> pfs::byte_string_ref(payload, sz) >> c >> d;

        if (a == '\x10' && b == '\x01' && c == '\x10' && d == '\x02')
            return payload.size() + 4 + sizeof(sz);

        return -1;
    }
};

template <typename Protocol>
struct simple_transport
{
    simple_transport ()
        : _os(_buffer)
    {}

    ssize_t send (pfs::byte_string const & payload, pfs::error_code & ec)
    {
        pfs::byte_string packet = Protocol().pack(payload);
        _os << packet;
        return pfs::integral_cast_check<ssize_t>(packet.size());
    }

    ssize_t recv (pfs::byte_string & payload, pfs::error_code & ec)
    {
        ssize_t r = Protocol().unpack(_buffer, payload);
        _buffer.clear();
        return r;
    }

protected:
    pfs::byte_string _buffer;
    pfs::byte_string_ostream _os;
};

typedef pfs::json::json<> json_t;
typedef pfs::rpc<2, 0
        , pfs::json::rpc::id_generator<int32_t>
        , pfs::json::rpc::ubjson_serializer<json_t, int32_t>
        , simple_protocol
        , json_t::string_type> rpc_ns;

int integer ()
{
    std::cout << "integer()" << std::endl;
    return 123;
}

int echo (int n)
{
    std::cout << "echo(" << n << ')' << std::endl;
    return n;
}

int sum (int a, int b)
{
    std::cout << "sum(" << a << ',' << b << ')' << std::endl;
    return a + b;
}

void notify1 ()
{
    std::cout << "notify1()" << std::endl;
}

void notify2 (int n)
{
    std::cout << "notify2(" << n << ')' << std::endl;
}

namespace server {

struct device_manager_slots : pfs::sigslot<>::has_slots
{
    log_ns::logger & _logger;

    device_manager_slots (log_ns::logger & logger) : _logger(logger) {}

    void device_accepted (pfs::io::device d, pfs::io::server s)
    {
        _logger.info("device_accepted");
    }

    void device_ready_read (pfs::io::device d)
    {
        _logger.info("device_ready_read");
    }

    void device_disconnected (pfs::io::device d)
    {
        _logger.info("device_disconnected: " + d.url());
    }

    void device_opened (pfs::io::device d)
    {
        _logger.info("device_opened");
    }

    void device_opening (pfs::io::device d)
    {
        _logger.info("device_opening");
    }

    void device_open_failed (pfs::io::device d, pfs::error_code ec)
    {
        _logger.error("device_open_failed" + pfs::to_string(ec));
    }

    void server_opened (pfs::io::server s)
    {
        _logger.info("server_opened");
    }

    void server_opening (pfs::io::server s)
    {
        _logger.info("server_opening");
    }

    void server_open_failed (pfs::io::server s, pfs::error_code ec)
    {
        _logger.error("server_open_failed: " + pfs::to_string(ec));
    }

    void device_io_error (pfs::error_code ec)
    {
        _logger.error("device_io_error: " + pfs::to_string(ec));
    }
};

void run ()
{
    typedef pfs::log<> log_ns;
    log_ns::logger logger;
    log_ns::appender & cout_appender
            = logger.add_appender<log_ns::stdout_appender>();
    log_ns::appender & cerr_appender
            = logger.add_appender<log_ns::stderr_appender>();
    cout_appender.set_pattern("%d{ABSOLUTE} [%p]: %m");
    cerr_appender.set_pattern("%d{ABSOLUTE} [%p]: %m");

    logger.connect(log_ns::priority::trace   , cout_appender);
    logger.connect(log_ns::priority::debug   , cout_appender);
    logger.connect(log_ns::priority::info    , cout_appender);
    logger.connect(log_ns::priority::warn    , cerr_appender);
    logger.connect(log_ns::priority::error   , cerr_appender);
    logger.connect(log_ns::priority::critical, cerr_appender);

    pfs::io::device_manager<> devman(10);
    device_manager_slots devslots(logger);
    pfs::error_code ec;

    devman.accepted.connect           (& devslots, & device_manager_slots::device_accepted);
    devman.ready_read.connect         (& devslots, & device_manager_slots::device_ready_read);
    devman.opened.connect             (& devslots, & device_manager_slots::device_opened);
    devman.opening.connect            (& devslots, & device_manager_slots::device_opening);
    devman.open_failed.connect        (& devslots, & device_manager_slots::device_open_failed);
    devman.disconnected.connect       (& devslots, & device_manager_slots::device_disconnected);
    devman.server_opened.connect      (& devslots, & device_manager_slots::server_opened);
    devman.server_opening.connect     (& devslots, & device_manager_slots::server_opening);
    devman.server_open_failed.connect (& devslots, & device_manager_slots::server_open_failed);
    devman.error.connect              (& devslots, & device_manager_slots::device_io_error);

    pfs::io::server tcp_server = devman.new_server(
        pfs::io::open_params<pfs::io::tcp_server>(TCP_LISTENER_ADDR
                , TCP_LISTENER_PORT
                , TCP_LISTENER_DEFAULT_BACKLOG
                , pfs::io::read_write | pfs::io::non_blocking)
            , & ec);


    simple_transport<simple_protocol> server_transport;
    rpc_ns::server<simple_transport> server(server_transport);

    server.bind("integer", integer);
    server.bind("echo", echo);
    server.bind("sum", sum);
    server.bind("notify1", notify1);
    server.bind("notify2", notify2);

    //server.dispatch();

    devman.dispatch();
}

} // namespace client

namespace client {

struct device_manager_slots : pfs::sigslot<>::has_slots
{
    log_ns::logger & _logger;

    device_manager_slots (log_ns::logger & logger) : _logger(logger) {}

    void device_ready_read (pfs::io::device d)
    {
        _logger.info("device_ready_read");
    }

    void device_disconnected (pfs::io::device d)
    {
        _logger.info("device_disconnected: " + d.url());
    }

    void device_opened (pfs::io::device d)
    {
        _logger.info("device_opened");
    }

    void device_opening (pfs::io::device d)
    {
        _logger.info("device_opening");
    }

    void device_open_failed (pfs::io::device d, pfs::error_code ec)
    {
        _logger.error("device_open_failed: " + pfs::to_string(ec));
    }

    void device_io_error (pfs::error_code ec)
    {
        _logger.error("device_io_error: " + pfs::to_string(ec));
    }
};

void run ()
{
    log_ns::logger logger;
    log_ns::appender & cout_appender
            = logger.add_appender<log_ns::stdout_appender>();
    log_ns::appender & cerr_appender
            = logger.add_appender<log_ns::stderr_appender>();
    cout_appender.set_pattern("%d{ABSOLUTE} [%p]: %m");
    cerr_appender.set_pattern("%d{ABSOLUTE} [%p]: %m");

    logger.connect(log_ns::priority::trace   , cout_appender);
    logger.connect(log_ns::priority::debug   , cout_appender);
    logger.connect(log_ns::priority::info    , cout_appender);
    logger.connect(log_ns::priority::warn    , cerr_appender);
    logger.connect(log_ns::priority::error   , cerr_appender);
    logger.connect(log_ns::priority::critical, cerr_appender);

    pfs::error_code ec;
    pfs::io::device_manager<> devman(10);
    device_manager_slots devslots(logger);

    devman.ready_read.connect  (& devslots, & device_manager_slots::device_ready_read);
    devman.opened.connect      (& devslots, & device_manager_slots::device_opened);
    devman.opening.connect     (& devslots, & device_manager_slots::device_opening);
    devman.open_failed.connect (& devslots, & device_manager_slots::device_open_failed);
    devman.disconnected.connect(& devslots, & device_manager_slots::device_disconnected);
    devman.error.connect       (& devslots, & device_manager_slots::device_io_error);

    pfs::io::device tcp_socket = devman.new_device(
            pfs::io::open_params<pfs::io::tcp_socket>(TCP_LISTENER_ADDR
                        , TCP_LISTENER_PORT
                        , pfs::io::read_write | pfs::io::non_blocking)
                , & ec);

    if (ec) {
        logger.error("Open socket error: " + pfs::to_string(ec));
        return;
    }

    ADD_TESTS(3);
    simple_transport<simple_protocol> client_transport;
    rpc_ns::client<simple_transport> client(client_transport);

    TEST_OK(client.call("integer").result<int>() == 123);
    TEST_OK(client.call("echo")(425).result<int>() == 425);
    TEST_OK(client.call("sum")(1)(2).result<int>() == 3);

    //     client.notify("notify1").send();
//     client.notify("notify2")(123).send();
}

} // namespace client

static int const CLIENT_COUNT = 2;

int main ()
{
    BEGIN_TESTS(0);

    pfs::thread server_thread(& server::run);
    pfs::unique_ptr<pfs::thread> client_threads[CLIENT_COUNT];

//     for (int i = 0; i < CLIENT_COUNT; ++i)
//         client_threads[i] = pfs::make_unique<pfs::thread>(& client::run);

    server_thread.join();
/*
    for (int i = 0; i < CLIENT_COUNT; ++i)
        client_threads[i]->join();*/

    return END_TESTS;
}
