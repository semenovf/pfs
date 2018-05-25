#include <iostream>
#include <pfs/byte_string.hpp>
#include <pfs/string.hpp>
#include <pfs/traits/stdcxx/string.hpp>
#include <pfs/traits/stdcxx/list.hpp>
#include <pfs/traits/stdcxx/vector.hpp>
#include <pfs/traits/stdcxx/map.hpp>
#include <pfs/safeformat.hpp>
#include <pfs/v2/sigslot.hpp>
#include <pfs/io/device_manager.hpp>
#include <pfs/io/inet_server.hpp>
#include <pfs/net/inet4_addr.hpp>

using std::cout;
using std::cerr;
using std::endl;

typedef pfs::string<pfs::stdcxx::string> string;
typedef pfs::safeformat<string> fmt;
typedef pfs::io::device_manager<pfs::stdcxx::list
        , pfs::stdcxx::vector
        , pfs::stdcxx::map> device_manager;
//
// Use this command (netcat) to test UDP server work properly
//
// $ echo -n "hello" | nc -w 1 -u -4 localhost 10000
//

#define __SIMPLE_SERVER__ 0
#define __POLL_SERVER__   0

extern "C" int simple_udp_server (int argc, char ** argv);
extern "C" int poll_udp_server (int argc, char ** argv);

struct handlers : pfs::has_slots<>
{
    void device_ready_read (pfs::io::device d)
    {
        //cout << fmt("%s: device ready to read)")(d.url()).str() << endl;
        pfs::byte_string data;

        if (! d.read(data)) {
            cerr << fmt("read device error: %s") % pfs::to_string<string>(d.errorcode())
                  << endl;
        } else {
            cout << "read: " << pfs::to_string<string>(data) << endl;
        }
    }

    void device_opened (pfs::io::device d)
    {
        cout << fmt("%s: device successfully opened/connected") % d.url() << endl;
    }

    void device_opening (pfs::io::device d)
    {
        cout << fmt("%s: open/connection device in progress") % d.url() << endl;
    }

    void device_open_failed (pfs::io::device d, pfs::error_code ex)
    {
        if (! d.is_null()) {
            cerr << fmt("%s: open/connection device error: %s")
                    (d.url())(pfs::to_string<string>(ex)).str() << endl;
        } else {
            cerr << fmt("open device error: %s") % pfs::to_string<string>(ex) << endl;
        }
    }

    void device_disconnected (pfs::io::device d)
    {
        cerr << fmt("%s: device closed/disconnected") % d.url() << endl;
    }

    void server_opened (pfs::io::server s)
    {
        cout << fmt("%s: server successfully opened") % s.url() << endl;
    }

    void server_opening (pfs::io::server s)
    {
        cout << fmt("%s: open server in progress") % s.url() << endl;
    }

    void server_open_failed (pfs::io::server s, pfs::error_code ex)
    {
        if (! s.is_null()) {
            cerr << fmt("%s: open server error: %s")
                    % s.url() % pfs::to_string<string>(ex) << endl;
        } else {
            cerr << fmt("open server error: %s") % pfs::to_string<string>(ex) << endl;
        }
    }

    void error (pfs::error_code ex)
    {
        cerr << fmt("I/O error: %s") % pfs::to_string<string>(ex) << endl;
    }
};

int main ()
{
#if __SIMPLE_SERVER__

    return simple_udp_server(argc, argv);

#elif __POLL_SERVER__

    return poll_udp_server(argc, argv);

#else
    int const millis = 100;
    device_manager devman(millis);
    pfs::net::inet4_addr ip(pfs::net::inet4_addr::any_addr_value);
    uint16_t port = 10000;
    handlers h;

    devman.ready_read.connect(& h, & handlers::device_ready_read);
    devman.opened.connect(& h, & handlers::device_opened);
    devman.opening.connect(& h, & handlers::device_opening);
    devman.open_failed.connect(& h, & handlers::device_open_failed);
    devman.disconnected.connect(& h, & handlers::device_disconnected);
    devman.server_opened.connect(& h, & handlers::server_opened);
    devman.server_opening.connect(& h, & handlers::server_opening);
    devman.server_open_failed.connect(& h, & handlers::server_open_failed);
    devman.error.connect(& h, & handlers::error);

    devman.new_server(pfs::io::open_params<pfs::io::udp_server>(ip, port
        , pfs::io::read_write | pfs::io::non_blocking));

    while (true) {
        devman.dispatch();
    }

    return 0;

#endif
}
