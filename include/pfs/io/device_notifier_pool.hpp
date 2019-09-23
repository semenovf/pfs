#pragma once

namespace pfs {
namespace io {

enum notification_flags {
      notify_read  = 1 << 0
    , notify_write = 1 << 1
    , notify_all   = notify_read | notify_write
};

}}

#include <pfs/operationsystem.hpp>
#include PFS_OS_HEADER(io/device_notifier_pool)

namespace pfs {
namespace io {

struct default_event_handler
{
    void accepted (device_ptr, server_ptr) {}
    void disconnected (device_ptr) {}
    void ready_read (device_ptr) {}
    void can_write (device_ptr) {}
    void on_error (error_code const &) {}
};

template <template <typename> class ContiguousContainer = pfs::vector
        , typename BasicLockable = pfs::mutex>
class device_notifier_pool : public details::device_notifier_pool<ContiguousContainer, BasicLockable>
{
    typedef details::device_notifier_pool<ContiguousContainer, BasicLockable> base_class;
//     typedef typename base_class::iterator iterator;
//     typedef typename base_class::poll_result_type poll_result_type;

public:
    device_notifier_pool () : base_class () {}

    template <typename EventHandler>
    void dispatch (EventHandler & event_handler, int millis = 0)
    {
        base_class::dispatch(event_handler, millis);
    }
};

}} // namespace pfs::io
