#include <ctime>
#include <pfs/test.hpp>
#include <pfs/map.hpp>
#include <pfs/list.hpp>
#include <pfs/set.hpp>
#include <pfs/vector.hpp>
#include <pfs/io/device_manager.hpp>
#include <pfs/io/inet_server.hpp>
#include <pfs/net/inet4_addr.hpp>

#include "device_manager_slots.hpp"

#if __cplusplus >= 201103L

using device_manager = pfs::io::device_manager<pfs::sigslot<>
        , pfs::list
        , pfs::vector
        , pfs::map
        , pfs::set>;

#else

typedef pfs::io::device_manager<pfs::sigslot<>
        , pfs::list
        , pfs::vector
        , pfs::map
        , pfs::set> device_manager;

#endif

static int const                  TCP_LISTENER_DEFAULT_BACKLOG = 50;
static pfs::net::inet4_addr const TCP_LISTENER_ADDR(127, 0, 0, 1);
static uint16_t const             TCP_LISTENER_PORT(9876);
static pfs::net::inet4_addr const TCP_DEFUNCT_LISTENER_ADDR(127, 0, 0, 1);
static uint16_t const             TCP_DEFUNCT_LISTENER_PORT(7654);

int main ()
{
    BEGIN_TESTS(2);

    pfs::error_code ec;
    device_manager_slots devslots;
    device_manager devman(10); // poll timeout is 10 milliseconds

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

    pfs::io::device tcp_defunct_socket = devman.new_device(
        pfs::io::open_params<pfs::io::tcp_socket>(TCP_DEFUNCT_LISTENER_ADDR
                    , TCP_DEFUNCT_LISTENER_PORT
                    , pfs::io::read_write | pfs::io::non_blocking)
            , & ec);

    TEST_FAIL2(ec.value() == static_cast<int>(pfs::io_errc::operation_in_progress)
            , "Defunct TCP socket opening in progress");

    pfs::io::server tcp_server = devman.new_server(
            pfs::io::open_params<pfs::io::tcp_server>(TCP_LISTENER_ADDR
                    , TCP_LISTENER_PORT
                    , TCP_LISTENER_DEFAULT_BACKLOG
                    , pfs::io::read_write | pfs::io::non_blocking)
                , & ec);

    TEST_FAIL2(!ec, "TCP listener opened");

    pfs::test::profiler watch;
    watch.start();

    while (watch.ellapsed() < 5.0f) {
        devman.dispatch();
	std::cout << "watch.ellapsed(): " << watch.ellapsed() << std::endl;
    }

    return END_TESTS;
}
