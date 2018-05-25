#pragma once
#include <pfs/algorithm.hpp>
#include <pfs/memory.hpp>
#include <pfs/utility.hpp>
#include <pfs/io/bits/device.hpp>
#include <pfs/byte_string.hpp>
#include <pfs/compiler.hpp>

// TODO Apply support of device types: RandomAccessDevice and StreamDevice

namespace pfs {
namespace io {

typedef details::device::open_mode_flags open_mode_flags;

template <typename DeviceTag>
struct open_params;

typedef shared_ptr<details::device> device_ptr;

ssize_t copy (device_ptr & dest, device_ptr & src, size_t chunkSize, error_code * ec = 0);

// //inline bool compress (device & src, device & dest, error_code * ex = 0)
// //{
// //  return compress(src, dest, zlib::DefaultCompression, 0x4000, ex);
// //}
// //
// //inline bool uncompress (device & src, device & dest, error_code * ex = 0)
// //{
// //	return uncompress(src, dest, 0x4000, ex);
// //}

template <typename DeviceTag>
device_ptr open_device (open_params<DeviceTag> const &, error_code & ec);

template <typename DeviceTag>
inline device_ptr open_device (open_params<DeviceTag> const & op)
{
    error_code ec;
    return open_device<DeviceTag>(op, ec);
}

template <typename DeviceTag>
inline device_ptr open_device ()
{
    error_code ec;
    open_params<DeviceTag> op;
    return open_device<DeviceTag>(op, ec);
}

}} // namespace pfs::io
