#pragma once
#include <pfs/filesystem.hpp>
#include <pfs/io/device.hpp>

namespace pfs {
namespace io {

/**
 * @struct pfs::io::file
 * @brief File device implementation.
 * @see pfs::io::device.
 */
struct file {};
struct file_stdin {};
struct file_stdout {};
struct file_stderr {};

template <>
struct open_params<file>
{
    static filesystem::perms const default_create_perms; /* = filesystem::perms::owner_read
            | filesystem::perms::owner_write
            | filesystem::perms::group_read
            | filesystem::perms::others_read;*/

    filesystem::path path;
    open_mode_flags oflags;
    filesystem::perms permissions;

    open_params (filesystem::path const & s, open_mode_flags of, filesystem::perms perms)
        : path (s)
        , oflags (of)
        , permissions (perms)
    {}

    open_params (filesystem::path const & s, open_mode_flags of)
        : path (s)
        , oflags (of)
        , permissions (default_create_perms)
    {}

    open_params (filesystem::path const & s)
        : path (s)
        , oflags (read_write)
        , permissions (default_create_perms)
    {}
};

template <>
struct open_params<file_stdin> {};

template <>
struct open_params<file_stdout> {};

template <>
struct open_params<file_stderr> {};

/**
 * @brief Open file device.
 *
 * @param op Open device parameters.
 * @param ec Error code to store resulting code while open device.
 *
 * @return device opened.
 */
template <>
device_ptr open_device<file> (open_params<file> const & op, error_code & ec);

template <>
device_ptr open_device<file_stdin> (open_params<file_stdin> const & op, error_code & ec);

template <>
device_ptr open_device<file_stdout> (open_params<file_stdout> const & op, error_code & ec);

template <>
device_ptr open_device<file_stderr> (open_params<file_stderr> const & op, error_code & ec);

}} // pfs::io
