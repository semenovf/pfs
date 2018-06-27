#pragma once
#include "pfs/io/file.hpp"

namespace test_standard_streams {

static int can_write_counter = 0;

struct event_handler
{
    void accepted (device_ptr d, server_ptr) { std::cout << "connected: " << d->url().utf8() << std::endl; }
    void disconnected (device_ptr d) { std::cout << "disconnected: " << d->url().utf8() << std::endl; }
    void ready_read (device_ptr d) { std::cout << "ready_read: " << d->url().utf8() << std::endl; }
    void can_write (device_ptr d)
    {
        std::cout << "can_write: " << d->url().utf8() << std::endl;
        can_write_counter++;
    }
    void on_error (pfs::error_code const & ec) { std::cerr << "on_error: " << pfs::to_string(ec).utf8() << std::endl; }
};

void run ()
{
    std::cout << "///////////////////////////////////////////////////////////////////////////\n";
    std::cout << "//                         Test standard streams                         //\n";
    std::cout << "///////////////////////////////////////////////////////////////////////////\n";

    ADD_TESTS(4);

    pfs::error_code ec;
    device_notifier_pool pool;

    pfs::io::device_ptr infile = pfs::io::open_device(pfs::io::open_params<pfs::io::file_stdin>(), ec);
    TEST_OK2(!ec, "Success stdin opened");

    pfs::io::device_ptr outfile = pfs::io::open_device(pfs::io::open_params<pfs::io::file_stdout>(), ec);
    TEST_OK2(!ec, "Success stdout opened");

    pfs::io::device_ptr errfile = pfs::io::open_device(pfs::io::open_params<pfs::io::file_stderr>(), ec);
    TEST_OK2(!ec, "Success stderr opened");

    pool.insert(infile, pfs::io::notify_read);
    pool.insert(outfile, pfs::io::notify_write);
    pool.insert(errfile, pfs::io::notify_all);

    event_handler eh;

    pool.dispatch(eh, 100);

    TEST_OK2(can_write_counter == 2, "Two streams can write");
}

} // namespace test_standard_streams
