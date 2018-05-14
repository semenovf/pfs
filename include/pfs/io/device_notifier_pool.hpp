#pragma once

namespace pfs {
namespace io {

enum notification_flags {
      notify_read  = 1 << 0
    , notify_write = 1 << 1
    , notify_error = 1 << 2
    , notify_all   = notify_read | notify_write | notify_error
};

}}

#include <pfs/operationsystem.hpp>
#include PFS_OS_HEADER(io/device_notifier_pool)

namespace pfs {
namespace io {

template <template <typename> class ContigousContainer = pfs::vector
        , typename BasicLockable = pfs::mutex>
class device_notifier_pool : public details::device_notifier_pool<ContigousContainer, BasicLockable>
{
    typedef details::device_notifier_pool<ContigousContainer, BasicLockable> base_class;
    typedef typename base_class::iterator iterator;
    typedef typename base_class::poll_result_type poll_result_type;
// public:
//     class dispatcher_context
//     {
//         friend class device_notifier_pool;
//         int _millis;
//
//     public:
//         dispatcher_context (int millis = 0)
//             : _millis(millis)
//         {}
//
//         virtual ~dispatcher_context () {}
//
//     public:
//         virtual void accepted (device &, server &) {}
//         virtual void ready_read (device &) {}
//         virtual void disconnected (device &) {}
//         virtual void can_write (device &) {}
//         virtual void on_error (error_code const &) {}
//     };

public:
    device_notifier_pool () : base_class () {}

    template <typename Listener>
    void dispatch (Listener & listener, int millis = 0)
    {
        base_class::dispatch(listener, millis);
    }
};

}} // namespace pfs::io
