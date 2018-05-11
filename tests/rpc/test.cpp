#include <iostream>
#include <pfs/test.hpp>
#include <pfs/system_error.hpp>
#include <pfs/byte_string.hpp>
#include <pfs/binary_istream.hpp>
#include <pfs/binary_ostream.hpp>
#include <pfs/net/inet4_addr.hpp>
#include <pfs/io/device_manager.hpp>
#include <pfs/io/inet_server.hpp>
#include <pfs/io/buffered_device.hpp>
#include <pfs/thread.hpp>
#include <pfs/logger.hpp>
#include "json/rpc.hpp"

typedef pfs::log<> log_ns;
static log_ns::logger g_logger;

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
    simple_transport (/*pfs::io::device & dev*/)
    {
        //dev.set_context(this);
    }

    ssize_t send (pfs::byte_string const & payload, pfs::error_code & ec)
    {
//         pfs::byte_string packet = Protocol().pack(payload);
//         return _dev.write(packet);



//         _os << packet;
//         return pfs::integral_cast_check<ssize_t>(packet.size());
        return -1;
    }

    ssize_t recv (pfs::byte_string & payload, pfs::error_code & ec)
    {
//         ssize_t r = Protocol().unpack(_buffer, payload);
//         _buffer.clear();
//         return r;
        return -1;
    }

// protected:
//     pfs::io::device _dev;
//     pfs::byte_string _buffer;
//     pfs::byte_string_ostream _os;
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
        _logger.info("Server: client accepted on: " + d.url());
        //d.set_context(new pfs::io::buffered_device(d));
    }

    void device_ready_read (pfs::io::device d)
    {
        _logger.info("Server: device_ready_read");
    }

    void device_disconnected (pfs::io::device d)
    {
        _logger.info("Server: client disconnected: " + d.url());
    }

    void device_opened (pfs::io::device d)
    {
        _logger.info("Server: device_opened");
    }

    void device_opening (pfs::io::device d)
    {
        _logger.info("Server: device_opening");
    }

    void device_open_failed (pfs::io::device d, pfs::error_code ec)
    {
        _logger.error("Server: device_open_failed" + pfs::to_string(ec));
    }

    void server_opened (pfs::io::server s)
    {
        _logger.info("Server: listen on: " + s.url());
    }

    void server_opening (pfs::io::server s)
    {
        _logger.info("Server: opening...");
    }

    void server_open_failed (pfs::io::server s, pfs::error_code ec)
    {
        _logger.error("Server: server_open_failed: " + pfs::to_string(ec));
    }

    void device_io_error (pfs::error_code ec)
    {
        _logger.error("Server: device_io_error: " + pfs::to_string(ec));
    }
};

void run ()
{
    pfs::io::device_manager<> devman(10);
    device_manager_slots devslots(g_logger);
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

//    while (true) {
        devman.dispatch();
        //pfs::this_thread::sleep_for(pfs::chrono::milliseconds(1));
//    }
}

} // namespace client

namespace client {

struct device_manager_slots : pfs::sigslot<>::has_slots
{
    log_ns::logger & _logger;

    device_manager_slots (log_ns::logger & logger) : _logger(logger) {}

    void device_ready_read (pfs::io::device d)
    {
        _logger.info("Client: device_ready_read");
    }

    void device_disconnected (pfs::io::device d)
    {
        _logger.info("Client: disconnected from: " + d.url());
    }

    void device_opened (pfs::io::device d)
    {
        _logger.info("Client: connected to: " + d.url());
        //d.set_context(new pfs::io::buffered_device(d));
    }

    void device_opening (pfs::io::device d)
    {
        _logger.info("Client: connecting to: " + d.url());
    }

    void device_open_failed (pfs::io::device d, pfs::error_code ec)
    {
        _logger.error("Client: device_open_failed: " + pfs::to_string(ec));
    }

    void device_io_error (pfs::error_code ec)
    {
        _logger.error("Client: device_io_error: " + pfs::to_string(ec));
    }
};

void run ()
{
    pfs::error_code ec;
    pfs::io::device_manager<> devman(10);
    device_manager_slots devslots(g_logger);

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

    if (ec)
        return;

    ADD_TESTS(3);
    simple_transport<simple_protocol> client_transport;
    rpc_ns::client<simple_transport> client(client_transport);

    //devman.dispatch();

    //tcp_socket.close();
//     TEST_OK(client.call("integer").result<int>() == 123);
//     TEST_OK(client.call("echo")(425).result<int>() == 425);
//     TEST_OK(client.call("sum")(1)(2).result<int>() == 3);

    //     client.notify("notify1").send();
//     client.notify("notify2")(123).send();
}

} // namespace client

static int const CLIENT_COUNT = 1;

int main ()
{
    BEGIN_TESTS(0);

    log_ns::appender & cout_appender
            = g_logger.add_appender<log_ns::stdout_appender>();
    log_ns::appender & cerr_appender
            = g_logger.add_appender<log_ns::stderr_appender>();
    cout_appender.set_pattern("%d{ABSOLUTE} [%p]: %m");
    cerr_appender.set_pattern("%d{ABSOLUTE} [%p]: %m");

    g_logger.connect(log_ns::priority::trace   , cout_appender);
    g_logger.connect(log_ns::priority::debug   , cout_appender);
    g_logger.connect(log_ns::priority::info    , cout_appender);
    g_logger.connect(log_ns::priority::warn    , cerr_appender);
    g_logger.connect(log_ns::priority::error   , cerr_appender);
    g_logger.connect(log_ns::priority::critical, cerr_appender);

    pfs::thread server_thread(& server::run);
    pfs::unique_ptr<pfs::thread> client_threads[CLIENT_COUNT];

    // Wait for server fully initialized
    pfs::this_thread::sleep_for(pfs::chrono::seconds(1));

    for (int i = 0; i < CLIENT_COUNT; ++i)
        client_threads[i] = pfs::make_unique<pfs::thread>(& client::run);

    pfs::this_thread::sleep_for(pfs::chrono::seconds(1));

    for (int i = 0; i < CLIENT_COUNT; ++i)
        client_threads[i]->join();

    server_thread.join();

    return END_TESTS;
}
