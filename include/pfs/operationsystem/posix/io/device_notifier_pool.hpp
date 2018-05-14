#pragma once
#include <poll.h>
#include <cerrno>
#include <pfs/limits.hpp>
#include <pfs/mutex.hpp>
#include <pfs/memory.hpp>
#include <pfs/vector.hpp>
#include <pfs/system_error.hpp>
#include <pfs/utility.hpp>
#include <pfs/io/bits/device.hpp>
#include <pfs/io/bits/server.hpp>

#if defined __USE_XOPEN || defined __USE_XOPEN2K8
#   define WR_EVENTS_XOR_MASK_XPG42 (POLLWRNORM | POLLWRBAND)
#else
#   define WR_EVENTS_XOR_MASK_XPG42 0
#endif

#define WR_EVENTS_XOR_MASK (POLLOUT | WR_EVENTS_XOR_MASK_XPG42)

#if __COMMENT__

|           |           |           |           |
 15 14 13 12 11 10 09 08 07 06 05 04 03 02 01 00
        |  |     |  |  |  |  |  |  |  |  |  |  |
        |  |     |  |  |  |  |  |  |  |  |  |  ---- POLLIN     0x0001 - There is data to read.
        |  |     |  |  |  |  |  |  |  |  |  ------- POLLPRI    0x0002 - There is urgent data to read
        |  |     |  |  |  |  |  |  |  |  ---------- POLLOUT    0x0004 - Writing now will not block
        |  |     |  |  |  |  |  |  |  ------------- POLLERR    0x0008 - Error condition
        |  |     |  |  |  |  |  |  ---------------- POLLHUP    0x0010 - Hung up
        |  |     |  |  |  |  |  ------------------- POLLNVAL   0x0020 - Invalid polling request
        |  |     |  |  |  |  --------------------*- POLLRDNORM 0x0040 - Normal data may be read
        |  |     |  |  |  -----------------------*- POLLRDBAND 0x0080 - Priority data may be read
        |  |     |  |  --------------------------*- POLLWRNORM 0x0100 - Writing now will not block
        |  |     |  -----------------------------*- POLLWRBAND 0x0200 - Priority data may be written
        |  |     -------------------------------**- POLLMSG    0x0400
        |  |
        |  -------------------------------------**- POLLREMOVE 0x1000
        ----------------------------------------**- POLLRDHUP  0x2000

* - These values are defined in XPG4.2 (defined __USE_XOPEN || defined __USE_XOPEN2K8)
**- These are extensions for Linux (defined __USE_GNU)

References:
1. [select() - Synchronous I/O Multiplexing](http://beej.us/guide/bgnet/output/html/multipage/advanced.html)
2. [7.3. Handling Partial send()s](http://beej.us/guide/bgnet/output/html/multipage/advanced.html#sendall)
3. [Using poll() instead of select()](https://www-01.ibm.com/support/knowledgecenter/ssw_ibm_i_71/rzab6/poll.htm)

#endif

namespace pfs {
namespace io {
namespace details {

template <template <typename> class ContigousContainer = pfs::vector
        , typename BasicLockable = pfs::mutex>
class device_notifier_pool
{
    typedef ::pollfd                           pollfd_type;
    typedef BasicLockable                      mutex_type;
    typedef ContigousContainer<pollfd_type>    pollfd_vec_type;
    typedef ContigousContainer<ssize_t>        index_vec_type;
    typedef ContigousContainer<basic_device *> device_vec_type;

    struct pollfd_map
    {
        pollfd_vec_type _pollfds;
        device_vec_type _devices;
        index_vec_type  _pollfds_indices;

        struct iterator
        {
            basic_device * device () const;
        };

        typedef pfs::pair<iterator, iterator> poll_result_type;

        pollfd_map () {}

        void insert (basic_device * dev, short notify_events)
        {
            short poll_events = 0;

            if (notify_events & notify_read)
                poll_events |= POLLIN;

            if (notify_events & notify_write)
                poll_events |= POLLOUT;

            // Only for semantic reason (ignored by poll on setup).
            // This events unconditionally returned by poll.
            if (notify_events & notify_error)
                poll_events |= POLLERR | POLLHUP | POLLNVAL;

            ssize_t index = -1;

            if (! _pollfds_indices.empty()) {
                index = _pollfds_indices.back();
                _pollfds_indices.pop_back();
            } else {
                pollfd_type emptyfd = { -1, 0, 0 };
                _pollfds.push_back(emptyfd);
                _devices.push_back(0);
                index = _pollfds.size() - 1;
            }

            _pollfds[index].fd = dev->native_handle();
            _pollfds[index].events = poll_events;
            _devices[index] = dev;
        }

        void erase (iterator pos)
        {
            pos.device().close();
        }

        poll_result_type poll (int filter_events, int millis, error_code & ec)
        {
            size_t n = _pollfds.size();
            pollfd_type * pfds = _pollfds.data();

            int r = 0;

            do {
                r = ::poll(pfds, n, millis);
            } while (r <= 0 and errno == EINTR);

            if (r == 0)
                return r;

            if (r < 0) {
                ec = get_last_system_error();
                return r;
            }

            return r;
        }
    };

public:
    typedef typename pollfd_map::iterator iterator;
    typedef typename pollfd_map::poll_result_type poll_result_type;

public:
    device_notifier_pool () {}

    int poll (int millis, error_code & ec)
    {
        pfs::lock_guard<mutex_type> locker(_mtx);
        return _pollfds->poll(millis, ec);
    }

    void insert (basic_device * dev, short notify_events = notify_all)
    {
        pfs::lock_guard<mutex_type> locker(_mtx);
        _pollfds.insert(dev, notify_events);
    }

    template <typename EventHandler>
    void dispatch (EventHandler & event_handler, int millis = 0)
    {
        pfs::error_code ec;
        poll_result_type result = this->poll(millis, ec);

        if (ec) {
            event_handler.on_error(ec);
        } else if (result.first != result.second) {
            iterator first = result.first;
            iterator last  = result.second;

            while (first != last) {
                details::basic_device * pdev = first.device();
                PFS_ASSERT(pdev);

                short revents = first.revents();

                if (pdev->is_server()) { // accept connection
                    // Servers wait incoming data (to establish connection)
                    // so ignore write events
                    //
                    if ((revents ^ WR_EVENTS_XOR_MASK) == 0) {
                        ; // TODO here may be need check opening/opened state
                    } else {
                        process_server(first, event_handler, revents);
                    }
                } else {
                    process_device(first, event_handler, revents);
                }

                ++first;
            }
        }
    }

private:

    template <typename EventHandler>
    void process_server (iterator pos, EventHandler & event_handler, short revents)
    {
        details::device * client;
        details::server * pserver = reinterpret_cast<details::server *>(pos.device());
        pfs::error_code ec = pserver->accept(& client, true);

        if (ec) {
            // Acceptance failed
            //
            event_handler.on_error(ec);
        } else {
            // Accepted
            //
            event_handler.template accepted(*client, *pserver);

            switch (pserver->type()) {
            case server_udp:
                // Process peer device on the spot
                // and release it automatically
                process_device(client, event_handler, revents);
                break;

            case server_tcp:
                insert(client, notify_all);
                break;

            default:
                PFS_DEBUG(fprintf(stderr, "**WARN: untested server type: %d\n", pserver->type()));
                insert(client, notify_all);
                break;
            }
        }
    }

    template <typename EventHandler>
    void process_device (iterator pos, EventHandler & event_handler, short revents)
    {
        details::device * pdev = reinterpret_cast<details::device *>(pos.device());

        if (!pdev->opened()) {
            if (revents & POLLNVAL) {
                // May occurred when device was closed before;
            }
            return;
        }

        // TODO Check if this event enough to decide to disconnect.
        if (pdev->available() == 0 && (revents & POLLIN)) {
            event_handler.disconnected(*pdev);
            _pollfds.erase(pos);
        } else {
            // Error condition (output only).
            //
            // TODO Research this feature and implement handling
            //
            if (revents & POLLERR) {
                PFS_DEBUG(puts("pfs::io::pool::dispatch(): device error condition"));
            }

            // Hang up (output only).
            //
            if (revents & POLLHUP) {
                event_handler.disconnected(*pdev);
                _pollfds.erase(pos);
            }

            // There is urgent data to read (e.g., out-of-band data on TCP socket;
            // pseudo-terminal master in packet mode has seen state change in slave).
            if (revents & POLLPRI)
                event_handler.ready_read(*pdev);

            // There is data to read
            if (revents & POLLIN)
                event_handler.ready_read(*pdev);

            // Writing is now possible, though a write larger than the available space
            // in a socket or pipe will still block (unless O_NONBLOCK is set).
            if (revents & POLLOUT)
                event_handler.can_write(*pdev);

            // Invalid request: fd not open (output only).
            if (revents & POLLNVAL) {
                event_handler.on_error(make_error_code(io_errc::bad_file_descriptor));
                _pollfds.erase(pos);
            }
        }
    }

private:
    mutex_type _mtx;
    pollfd_map _pollfds;
};

}}} // namespace pfs::io::details

#if __COMMENT__

#pragma once
#include <pfs/io/bits/pool.hpp>
#include <pfs/list.hpp>
#include <pfs/map.hpp>


namespace pfs {
namespace io {
namespace details {

template <template <typename> class SequenceContainer = pfs::list
        , template <typename> class ContigousContainer = pfs::vector
        , template <typename, typename> class AssociativeContainer = pfs::map>
struct pool : public bits::pool
{
    void fetch_devices (device_sequence & result
            , bool (* filter) (device const & d, void * context)
            , void * context)
    {
        pfs::lock_guard<pfs::mutex> locker(_mtx);

        if (_device_map.size() > 0) {
            typename device_map::const_iterator first = _device_map.cbegin();
            typename device_map::const_iterator last = _device_map.cend();

            while (first != last) {
                if (filter) {
                    if (filter(first->second, context))
                        result.push_back(first->second);
                } else {
                    result.push_back(first->second);
                }
                ++first;
            }
        }
    }

    void fetch_servers (server_sequence & servers
            , bool (* filter) (server const & s, void * context)
            , void * context)
    {
        pfs::lock_guard<pfs::mutex> locker(_mtx);

        if (_server_map.size() > 0) {
            typename server_map::const_iterator first = _server_map.cbegin();
            typename server_map::const_iterator last = _server_map.cend();

            while (first != last) {
                if (filter) {
                    if (filter(first->second, context))
                        servers.push_back(first->second);
                } else {
                    servers.push_back(first->second);
                }
                ++first;
            }
        }
    }
};

}}} // pfs::io::details


#endif // __COMMENT__
