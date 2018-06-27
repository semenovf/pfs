#include <ctime>
#include <iostream>
#include <pfs/test.hpp>
#include <pfs/map.hpp>
#include <pfs/list.hpp>
#include <pfs/set.hpp>
#include <pfs/vector.hpp>
#include <pfs/sigslot.hpp>
#include <pfs/io/device_manager.hpp>
#include <pfs/io/inet_server.hpp>
#include <pfs/net/inet4_addr.hpp>

typedef pfs::io::device_manager<> device_manager;

static int const                  TCP_LISTENER_DEFAULT_BACKLOG = 50;
static pfs::net::inet4_addr const TCP_LISTENER_ADDR(127, 0, 0, 1);
static uint16_t const             TCP_LISTENER_PORT(9876);
static pfs::net::inet4_addr const TCP_DEFUNCT_LISTENER_ADDR(127, 0, 0, 1);
static uint16_t const             TCP_DEFUNCT_LISTENER_PORT(7654);

struct event_handler : pfs::sigslot<>::has_slots
{
    void device_accepted (pfs::io::device_ptr d, pfs::io::server_ptr s)
    {
        std::cout << "device_accepted\n";
    }

    void device_ready_read (pfs::io::device_ptr d)
    {
        std::cout << "device_ready_read\n";
    }

    void device_disconnected (pfs::io::device_ptr d)
    {
        std::cout << "device_disconnected: "
                << d->url()
                << "\n";
    }

    void device_opened (pfs::io::device_ptr d)
    {
        std::cout << "device_opened\n";
    }

    void device_opening (pfs::io::device_ptr d)
    {
        std::cout << "device_opening\n";
    }

    void device_open_failed (pfs::io::device_ptr d, pfs::error_code const & ec)
    {
        std::cout << "device_open_failed\n";
    }

    void server_opened (pfs::io::server_ptr s)
    {
        std::cout << "server_opened\n";
    }

    void server_opening (pfs::io::server_ptr s)
    {
        std::cout << "server_opening\n";
    }

    void server_open_failed (pfs::io::server_ptr s, pfs::error_code const & ec)
    {
        std::cout << "server_open_failed\n";
    }

    void device_io_error (pfs::error_code const & ec)
    {
        std::cout << "device_io_error: " << pfs::to_string(ec) << std::endl;
    }
};

int main ()
{
    BEGIN_TESTS(3);

    pfs::error_code ec;
    event_handler eh;
    device_manager devman;

    devman.accepted.connect           (& eh, & event_handler::device_accepted);
    devman.ready_read.connect         (& eh, & event_handler::device_ready_read);
    devman.opened.connect             (& eh, & event_handler::device_opened);
    devman.opening.connect            (& eh, & event_handler::device_opening);
    devman.open_failed.connect        (& eh, & event_handler::device_open_failed);
    devman.disconnected.connect       (& eh, & event_handler::device_disconnected);
    devman.server_opened.connect      (& eh, & event_handler::server_opened);
    devman.server_opening.connect     (& eh, & event_handler::server_opening);
    devman.server_open_failed.connect (& eh, & event_handler::server_open_failed);
    devman.error.connect              (& eh, & event_handler::device_io_error);

    {
//         pfs::io::device_ptr tcp_defunct_socket = devman.new_device(
//             pfs::io::open_params<pfs::io::tcp_socket>(TCP_DEFUNCT_LISTENER_ADDR
//                         , TCP_DEFUNCT_LISTENER_PORT
//                         , pfs::io::read_write | pfs::io::non_blocking)
//                 , ec);
//
//         std::cout << pfs::to_string(ec) << std::endl;
//
//         TEST_FAIL2(ec == pfs::make_error_code(pfs::io_errc::operation_in_progress)
//                 , "Defunct TCP socket opening in progress");

        pfs::io::server_ptr tcp_server = devman.new_server(
                pfs::io::open_params<pfs::io::tcp_server>(TCP_LISTENER_ADDR
                        , TCP_LISTENER_PORT
                        , TCP_LISTENER_DEFAULT_BACKLOG
                        , pfs::io::read_write | pfs::io::non_blocking)
                        , ec);

        TEST_FAIL2(!ec, "TCP listener opened");

        pfs::io::device_ptr tcp_socket = devman.new_device(
                pfs::io::open_params<pfs::io::tcp_socket>(TCP_LISTENER_ADDR
                    , TCP_LISTENER_PORT
                    , pfs::io::read_write /*| pfs::io::non_blocking*/)
                    , ec);

        TEST_OK2(!ec, "TCP socket connected");

        devman.close(tcp_socket);

        TEST_OK(true);
    }

    // Do not work properly on TravisCI
    // pfs::test::profiler watch;
    // watch.start();
    // while (watch.ellapsed() < 5.0f) {
    time_t t = time(0);
    while (time(0) - t < 5) {
        devman.dispatch();
//        std::cout << "watch.ellapsed(): " << watch.ellapsed() << std::endl;
    }

    return END_TESTS;
}
