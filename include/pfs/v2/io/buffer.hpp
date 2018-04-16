#pragma once
#include <pfs/v2/io/device.hpp>
#include <pfs/v2/byte_string.hpp>

namespace pfs {
namespace io {

struct buffer
{};

template <>
struct open_params<buffer>
{
    byte_string & _bs;

    open_params (byte_string & bs) : _bs(bs) {}
};

/**
 * @fn bool open_device<buffer> (device & d, const open_params<buffer> & op)
 *
 * @brief Open buffer device.
 *
 * @param d Buffer device to open.
 * @param op Open device parameters.
 *      @li open_params<buffer>(byte_string & bs)
 *          Open buffer device with reference to byte_string instance.
 *
 * @return @c true if open is successful, @c false otherwise
 *         (i.e. buffer device is already opened).
 */
template <>
device open_device<buffer> (open_params<buffer> const & op, error_code & ec);

}} // pfs::io
