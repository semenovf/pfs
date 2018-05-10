#pragma once
#include <pfs/io/device.hpp>
#include <pfs/filesystem.hpp>

namespace pfs { namespace io {

/**
 * @struct pfs::io::local_socket
 * @brief Local (unix) socket device implementation.
 * @see pfs::io::device.
 */
struct local_socket {};

template <>
struct open_params<local_socket>
{
    device::open_mode_flags oflags;
    filesystem::path path;

    open_params (filesystem::path const & filename, device::open_mode_flags of)
        : path(filename)
        , oflags(of)
    {}

    open_params (filesystem::path const & filename)
        : path(filename)
        , oflags(read_write | non_blocking)
    {}

};

template <>
device open_device<local_socket> (open_params<local_socket> const & op, error_code & ex);

}} // pfs::io

