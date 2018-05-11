#pragma once

namespace pfs {
namespace io {

enum notification_flags {
      notify_read  = 1 << 0
    , notify_write = 1 << 1
    , notify_error = 1 << 2
    , notify_all   = notify_read | notify_write | notify_error
};

}} // namespace pfs::io

#include <pfs/operationsystem.hpp>
#include PFS_OS_HEADER(io/device_notifier_pool)
