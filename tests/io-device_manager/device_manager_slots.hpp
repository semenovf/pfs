#pragma once

#include <pfs/sigslot.hpp>
#include <iostream>

struct device_manager_slots : pfs::sigslot<>::has_slots
{
    void device_accepted (pfs::io::device d, pfs::io::server s)
    {
        std::cout << "device_accepted\n";
    }

    void device_ready_read (pfs::io::device d)
    {
        std::cout << "device_ready_read\n";
    }

    void device_disconnected (pfs::io::device d)
    {
        std::cout << "device_disconnected: "
                << d.url()
                << "\n";
    }

    void device_opened (pfs::io::device d)
    {
        std::cout << "device_opened\n";
    }

    void device_opening (pfs::io::device d)
    {
        std::cout << "device_opening\n";
    }

    void device_open_failed (pfs::io::device d, pfs::error_code ec)
    {
        std::cout << "device_open_failed\n";
    }

    void server_opened (pfs::io::server s)
    {
        std::cout << "server_opened\n";
    }

    void server_opening (pfs::io::server s)
    {
        std::cout << "server_opening\n";
    }

    void server_open_failed (pfs::io::server s, pfs::error_code ec)
    {
        std::cout << "server_open_failed\n";
    }

    void device_io_error (pfs::error_code ec)
    {
        std::cout << "device_io_error\n";
    }
};
