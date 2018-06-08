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
#include <pfs/json/rpc.hpp>

typedef pfs::log<> log_ns;
static log_ns::logger g_logger;

static int const                  TCP_LISTENER_DEFAULT_BACKLOG = 50;
static pfs::net::inet4_addr const TCP_LISTENER_ADDR(127, 0, 0, 1);
static uint16_t const             TCP_LISTENER_PORT(9876);

//template <typename OStreamT, typename IStreamT>
struct simple_protocol
{
    simple_protocol () {}

    template <typename OStream>
    bool send (OStream & out, pfs::byte_string const & payload) const
    {
        try {
            out << '\x10' << '\x01' << payload.size() << payload << '\x10' << '\x02';
        } catch (pfs::exception const & ex) {
            g_logger.error("failed to send RPC packet: " + pfs::string(ex.message()));
            return false;
        }

        return true;
    }

    template <typename IStream>
    bool recv (IStream & in, pfs::byte_string & payload)
    {
        uint8_t a, b, c, d;

        try {
            pfs::byte_string::size_type sz;
            in >> a >> b >> sz;
            in >> pfs::byte_string_ref(payload, sz) >> c >> d;
        } catch (pfs::exception const & ex) {
            g_logger.error("failed to receive RPC packet: " + pfs::string(ex.message()));
            return false;
        }

        if (!(a == '\x10' && b == '\x01' && c == '\x10' && d == '\x02')) {
            g_logger.error("bad RPC packet received");
            return false;
        }

        return true;
    }
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

///////////////////////////////////////////////////////////////////////////////
// Client                                                                    //
///////////////////////////////////////////////////////////////////////////////
namespace client {

struct transport
{
    static int const DEFAULT_INPUT_TIMEOUT = 10;
    typedef pfs::binary_istream<pfs::io::device_ptr> istream_type;
    typedef pfs::binary_ostream<pfs::io::device_ptr> ostream_type;

    transport (pfs::io::device_ptr d) : _in(d, DEFAULT_INPUT_TIMEOUT), _out(d) {}

    istream_type & reader () { return _in; }
    ostream_type & writer () { return _out; }

private:
    istream_type _in;
    ostream_type _out;
};

void run ()
{
    ADD_TESTS(3);

    pfs::error_code ec;
    pfs::io::device_ptr d = pfs::io::open_device(
                  pfs::io::open_params<pfs::io::tcp_socket>(
                          TCP_LISTENER_ADDR
                        , TCP_LISTENER_PORT
                        , pfs::io::read_write), ec);

    if (ec) {
        g_logger.error("failed to initialize client transport: " + pfs::to_string(ec));
        return;
    }

    transport client_transport(d);
    rpc_ns::client<transport> client(client_transport);

    try {
        TEST_OK(client.call("integer").result<int>() == 123);
//     TEST_OK(client.call("echo")(425).result<int>() == 425);
//     TEST_OK(client.call("sum")(1)(2).result<int>() == 3);

    //     client.notify("notify1").send();
//     client.notify("notify2")(123).send();
    } catch (pfs::exception const & ex) {
        g_logger.error("RPC client failed: " + pfs::string(ex.message()));
    }

    d->close();
}

} // namespace client

namespace server {

struct device_manager_slots : pfs::sigslot<>::has_slots
{
    device_manager_slots () {}

    void device_connected (pfs::io::device_ptr & d, pfs::io::server_ptr & s)
    {
        g_logger.info("Server: client accepted on: " + d->url());
        //d.set_context(new pfs::io::buffered_device(d));
    }

    void device_ready_read (pfs::io::device_ptr & d)
    {
        //_logger.info("Server: device_ready_read");
        pfs::byte_string bytes;
        d->read(bytes);
        g_logger.info("Server: " + pfs::to_string(bytes.size()) + " bytes read");
    }

    void device_disconnected (pfs::io::device_ptr & d)
    {
        g_logger.info("Server: client disconnected: " + d->url());
    }

    void device_opened (pfs::io::device_ptr & d)
    {
        g_logger.info("Server: device_opened");
    }

    void device_opening (pfs::io::device_ptr & d)
    {
        g_logger.info("Server: device_opening");
    }

    void device_open_failed (pfs::io::device_ptr & d, pfs::error_code const & ec)
    {
        g_logger.error("Server: device_open_failed" + pfs::to_string(ec));
    }

    void server_opened (pfs::io::server_ptr & s)
    {
        g_logger.info("Server: listen on: " + s->url());
    }

    void server_opening (pfs::io::server_ptr & s)
    {
        g_logger.info("Server: opening...");
    }

    void server_open_failed (pfs::io::server_ptr & s, pfs::error_code const & ec)
    {
        g_logger.error("Server: server_open_failed: " + pfs::to_string(ec));
    }

    void device_io_error (pfs::error_code const & ec)
    {
        g_logger.error("Server: device_io_error: " + pfs::to_string(ec));
    }
};

struct transport
{
    transport (/*pfs::io::device & dev*/)
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

void run ()
{
    pfs::io::device_manager<> devman;
    device_manager_slots devslots;
    pfs::error_code ec;

    devman.connected.connect          (& devslots, & device_manager_slots::device_connected);
    devman.ready_read.connect         (& devslots, & device_manager_slots::device_ready_read);
    devman.opened.connect             (& devslots, & device_manager_slots::device_opened);
    devman.opening.connect            (& devslots, & device_manager_slots::device_opening);
    devman.open_failed.connect        (& devslots, & device_manager_slots::device_open_failed);
    devman.disconnected.connect       (& devslots, & device_manager_slots::device_disconnected);
    devman.server_opened.connect      (& devslots, & device_manager_slots::server_opened);
    devman.server_opening.connect     (& devslots, & device_manager_slots::server_opening);
    devman.server_open_failed.connect (& devslots, & device_manager_slots::server_open_failed);
    devman.error.connect              (& devslots, & device_manager_slots::device_io_error);

    pfs::io::server_ptr tcp_server = devman.new_server(
        pfs::io::open_params<pfs::io::tcp_server>(TCP_LISTENER_ADDR
                , TCP_LISTENER_PORT
                , TCP_LISTENER_DEFAULT_BACKLOG
                , pfs::io::read_write | pfs::io::non_blocking)
            , ec);


    transport server_transport;
    rpc_ns::server<transport> server(server_transport);

    server.bind("integer", integer);
    server.bind("echo", echo);
    server.bind("sum", sum);
    server.bind("notify1", notify1);
    server.bind("notify2", notify2);

    while (devman.count_devices() == 0) {
        devman.dispatch();
        pfs::this_thread::sleep_for(pfs::chrono::milliseconds(10));
    }

    while (devman.count_devices() > 0) {
        devman.dispatch();
        pfs::this_thread::sleep_for(pfs::chrono::milliseconds(10));
    }
}

} // namespace server

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

    for (int i = 0; i < CLIENT_COUNT; ++i) {
        client_threads[i]->join();
        pfs::this_thread::sleep_for(pfs::chrono::microseconds(100));
    }

    server_thread.join();

    return END_TESTS;
}
