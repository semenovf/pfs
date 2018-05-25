#pragma once
#include <pfs/memory.hpp>
#include <pfs/io/bits/server.hpp>
#include <pfs/io/device.hpp>

namespace pfs {
namespace io {

template <typename ServerTag>
struct open_params;

typedef shared_ptr<details::server> server_ptr;

template <typename ServerTag>
server_ptr open_server (open_params<ServerTag> const &, error_code & ec);

}} // namespace pfs::io
