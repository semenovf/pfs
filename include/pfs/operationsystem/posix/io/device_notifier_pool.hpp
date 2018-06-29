#pragma once
#include <poll.h>
#include <cerrno>
#include <pfs/limits.hpp>
#include <pfs/mutex.hpp>
#include <pfs/memory.hpp>
#include <pfs/vector.hpp>
#include <pfs/system_error.hpp>
#include <pfs/utility.hpp>
#include <pfs/io/device.hpp>
#include <pfs/io/server.hpp>
#include <pfs/debug.hpp>

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
    typedef ::pollfd                             pollfd_type;
    typedef shared_ptr<basic_device>             basic_device_ptr;
    typedef BasicLockable                        mutex_type;
    typedef ContigousContainer<pollfd_type>      pollfd_vec_type;
    typedef ContigousContainer<ssize_t>          index_vec_type;
    typedef ContigousContainer<basic_device_ptr> device_vec_type;

    class pollfd_map
    {
    public:
        pollfd_vec_type _pollfds;
        device_vec_type _devices;
        index_vec_type  _pollfds_indices;
        device_vec_type _deferred_devices; // candidates for insertion before the next iteration of poll

        class revents_iterator
        {
        public:
            typename pollfd_vec_type::iterator pollfd_it;
            typename pollfd_vec_type::iterator pollfd_end;
            typename device_vec_type::iterator basic_device_it;

            revents_iterator (typename pollfd_vec_type::iterator p
                    , typename pollfd_vec_type::iterator end
                    , typename device_vec_type::iterator d)
                : pollfd_it(p)
                , pollfd_end(end)
                , basic_device_it(d)
            {}

            revents_iterator (revents_iterator const & rhs)
                : pollfd_it(rhs.pollfd_it)
                , pollfd_end(rhs.pollfd_end)
                , basic_device_it(rhs.basic_device_it)
            {}

            revents_iterator & operator = (revents_iterator const & rhs)
            {
                pollfd_it = rhs.pollfd_it;
                pollfd_end = rhs.pollfd_end;
                basic_device_it = rhs.basic_device_it;
                return *this;
            }

            void set_begin ()
            {
                while (pollfd_it != pollfd_end
                        && pollfd_it->revents == 0) {
                    ++pollfd_it;
                    ++basic_device_it;
                }
            }

            bool is_server () const
            {
                return (*basic_device_it)->is_server();
            }

            basic_device const * basic_device_ptr () const
            {
                return basic_device_it->get();
            }

            short operator * () const
            {
                return pollfd_it->revents;
            }

            device_ptr device () const
            {
                PFS_ASSERT(!(*basic_device_it)->is_server());
                return pfs::static_pointer_cast<details::device>(*basic_device_it);
            }

            server_ptr server () const
            {
                PFS_ASSERT((*basic_device_it)->is_server());
                return pfs::static_pointer_cast<details::server>(*basic_device_it);
            }

            pollfd_type & pollfd () const
            {
                return *pollfd_it;
            }

            revents_iterator & operator ++ ()
            {
                if (pollfd_it != pollfd_end) {
                    ++pollfd_it;
                    ++basic_device_it;
                }

                while (pollfd_it != pollfd_end
                        && pollfd_it->revents == 0) {
                    ++pollfd_it;
                    ++basic_device_it;
                }

                return *this;
            }

            revents_iterator operator ++ (int)
            {
                revents_iterator r = *this;
                ++(*this);
                return r;
            }

            bool operator == (revents_iterator const & rhs) const
            {
                return pollfd_it == rhs.pollfd_it;
            }
        };

        typedef pfs::pair<revents_iterator, revents_iterator> poll_result_type;

        pollfd_map () {}

        ~pollfd_map () {}

        revents_iterator revents_begin ()
        {
            revents_iterator it(_pollfds.begin(), _pollfds.end(), _devices.begin());
            it.set_begin();
            return it;
        }

        revents_iterator revents_end ()
        {
            return revents_iterator(_pollfds.end(), _pollfds.end(), _devices.end());
        }

        void insert_deferred (device_ptr const & d)
        {
            _deferred_devices.push_back(pfs::static_pointer_cast<basic_device>(d));
        }

        void insert (basic_device_ptr const & d, short notify_events)
        {
            short poll_events = 0;

            // FIXME Allow if Linux version match
            // (since Linux 2.6.17)
            //poll_events |= POLLRDHUP;

            if (notify_events & notify_read)
                poll_events |= POLLIN;

            if (notify_events & notify_write)
                poll_events |= POLLOUT;

            ssize_t index = -1;

            if (! _pollfds_indices.empty()) {
                index = _pollfds_indices.back();
                _pollfds_indices.pop_back();
            } else {
                pollfd_type emptyfd = { -1, 0, 0 };
                _pollfds.push_back(emptyfd);
                _devices.push_back(basic_device_ptr());
                index = _pollfds.size() - 1;
            }

            _pollfds[index].fd = d->native_handle();
            _pollfds[index].events = poll_events;
            _devices[index] = d;
        }

        poll_result_type poll (int millis, error_code & ec)
        {
            // Insert deferred devices
            if (_deferred_devices.size() > 0) {
                typename device_vec_type::iterator first = _deferred_devices.begin();
                typename device_vec_type::iterator last = _deferred_devices.end();

                while (first != last) {
                    insert(*first, notify_all);
                    ++first;
                }

                _deferred_devices.clear();
            }

            size_t n = _pollfds.size();
            pollfd_type * pfds = _pollfds.data();

            int r = 0;

            do {
                r = ::poll(pfds, n, millis);
            } while (r <= 0 && errno == EINTR);

            if (r == 0)
                return pfs::make_pair(revents_end(), revents_end());

            if (r < 0) {
                ec = get_last_system_error();
                return pfs::make_pair(revents_end(), revents_end());
            }

            return pfs::make_pair(revents_begin(), revents_end());
        }

        ssize_t find (basic_device_ptr const & d)
        {
            typename device_vec_type::iterator first = _devices.begin();
            typename device_vec_type::iterator last  = _devices.end();

            ssize_t r = 0;

            while (first != last) {
                if (*first && (*first)->native_handle() == d->native_handle())
                    return r;
                ++first;
                ++r;
            }

            return -1;
        }

        void erase (revents_iterator pos)
        {
            ssize_t index = pfs::distance(_pollfds.begin(), pos.pollfd_it);
            if (index >= 0) clear(pfs::integral_cast_check<size_t>(index));
        }

        void erase (basic_device_ptr const & d)
        {
            ssize_t index = find(d);
            if (index >= 0) clear(pfs::integral_cast_check<size_t>(index));
        }

    private:
        void clear (size_t index)
        {
            _pollfds[index].fd = -1;
            _pollfds[index].events = 0;
            _pollfds[index].revents = 0;
            _pollfds_indices.push_back(index);

            basic_device_ptr tmp;
            _devices[index].swap(tmp);
        }
    };

public:
    typedef typename pollfd_map::revents_iterator revents_iterator;
    typedef typename pollfd_map::poll_result_type poll_result_type;

public:
    device_notifier_pool () {}

    void insert (server_ptr const & s, short notify_events = notify_all)
    {
        pfs::lock_guard<mutex_type> locker(_mtx);
        _pollfds.insert(pfs::static_pointer_cast<basic_device>(s), notify_events);
    }

    void insert (device_ptr const & d, short notify_events = notify_all)
    {
        pfs::lock_guard<mutex_type> locker(_mtx);
        _pollfds.insert(pfs::static_pointer_cast<basic_device>(d), notify_events);
    }

    // Do not use this method directly
    void erase (device_ptr const & d)
    {
        _pollfds.erase(pfs::static_pointer_cast<basic_device>(d));
    }

    // Do not use this method directly
    void erase (server_ptr const & s)
    {
        _pollfds.erase(pfs::static_pointer_cast<basic_device>(s));
    }

    template <typename EventHandler>
    void dispatch (EventHandler & event_handler, int millis = 0)
    {
        pfs::lock_guard<mutex_type> locker(_mtx);
        pfs::error_code ec;
        poll_result_type result = _pollfds.poll(millis, ec);

        if (ec) {
            event_handler.on_error(ec);
        } else if (result.first != result.second) {
            revents_iterator first = result.first;
            revents_iterator last  = result.second;

            while (first != last) {
                short revents = *first;

                if (revents != 0) {
                    if (first.is_server()) { // accept connection
                        // Servers wait incoming data (to establish connection)
                        // so ignore write events
                        //
                        if ((revents ^ WR_EVENTS_XOR_MASK) == 0) {
                            ; // TODO here may be need check opening/opened state
                        } else {
                            process_server(first, event_handler);
                        }
                    } else {
                        process_device(first, event_handler);
                    }
                }

                ++first;
            }
        }
    }

    device_ptr front_device ()
    {
        pfs::lock_guard<pfs::mutex> locker(_mtx);

        typename device_vec_type::iterator first = _pollfds._devices.begin();
        typename device_vec_type::iterator last  = _pollfds._devices.end();

        while (first != last) {
            if (*first && !(*first)->is_server())
                return pfs::static_pointer_cast<device>(*first);
            ++first;
        }

        return device_ptr();
    }

    device_ptr front_server ()
    {
        pfs::lock_guard<pfs::mutex> locker(_mtx);

        typename device_vec_type::iterator first = _pollfds._devices.begin();
        typename device_vec_type::iterator last  = _pollfds._devices.end();

        while (first != last) {
            if (*first && (*first)->is_server())
                return pfs::static_pointer_cast<server>(*first);
            ++first;
        }

        return device_ptr();
    }

    template <typename UnaryFunction>
    void for_each_device (UnaryFunction f)
    {
        pfs::lock_guard<pfs::mutex> locker(_mtx);

        typename device_vec_type::iterator first = _pollfds._devices.begin();
        typename device_vec_type::iterator last  = _pollfds._devices.end();

        while (first != last) {
            if (*first && !(*first)->is_server())
                f(pfs::static_pointer_cast<device>(*first));
            ++first;
        }
    }

    template <typename FilterFunction, typename UnaryFunction>
    void for_each_device (FilterFunction filter, UnaryFunction f)
    {
        pfs::lock_guard<pfs::mutex> locker(_mtx);

        typename device_vec_type::iterator first = _pollfds._devices.begin();
        typename device_vec_type::iterator last  = _pollfds._devices.end();

        while (first != last) {
            if (*first && !(*first)->is_server() && filter(pfs::static_pointer_cast<device>(*first)))
                f(pfs::static_pointer_cast<device>(*first));
            ++first;
        }
    }

    template <typename UnaryFunction>
    void for_each_server (UnaryFunction f)
    {
        pfs::lock_guard<pfs::mutex> locker(_mtx);

        typename device_vec_type::iterator first = _pollfds._devices.begin();
        typename device_vec_type::iterator last  = _pollfds._devices.end();

        while (first != last) {
            if (*first && (*first)->is_server())
                f(pfs::static_pointer_cast<server>(*first));
            ++first;
        }
    }

    template <typename FilterFunction, typename UnaryFunction>
    void for_each_server (FilterFunction filter, UnaryFunction f)
    {
        pfs::lock_guard<pfs::mutex> locker(_mtx);

        typename device_vec_type::iterator first = _pollfds._devices.begin();
        typename device_vec_type::iterator last  = _pollfds._devices.end();

        while (first != last) {
            if (*first && (*first)->is_server() && filter(pfs::static_pointer_cast<server>(*first)))
                f(pfs::static_pointer_cast<server>(*first));
            ++first;
        }
    }

private:

    template <typename EventHandler>
    void process_server (revents_iterator pos, EventHandler & event_handler)
    {
        pfs::error_code ec;
        server_ptr server = pos.server();
        details::device * peer = server->accept(true, ec);

        if (ec) {
            // Acceptance failed
            //
            event_handler.on_error(ec);
        } else {
            device_ptr pdev(peer);
            event_handler.accepted(pdev, server);

            switch (server->type()) {
            case server_udp:
                // TODO
                // Process peer device on the spot
                // and release it automatically
                //process_device(client, event_handler, revents);
                break;

            case server_tcp:
                _pollfds.insert_deferred(pdev);
                break;

            default:
                PFS_DEBUG(fprintf(::stderr, "**WARN: untested server type: %d\n", server->type()));
                _pollfds.insert_deferred(pdev);
                break;
            }
        }
    }

    template <typename EventHandler>
    void process_device (revents_iterator pos, EventHandler & event_handler)
    {
        device_ptr d = pos.device();
        short revents = *pos;

        if (!d->opened()) {
            if (revents & POLLNVAL) {
                ; // May occurred when device was closed before;
            }
            return;
        }

        // Error condition (output only).
        //
        // TODO Research this feature and implement handling
        //
        // 1. Occured while tcp socket attempts to connect to non-existance server socket
        // 2. ... ?
        if (revents & POLLERR) {
            if (d->type() == device_tcp_socket) {
                event_handler.on_error(make_error_code(io_errc::connection_refused));
            } else {
                PFS_DEBUG(printf("pfs::io::device_notifier_pool::process_device(): device error condition at %s:%d\n", __FILE__, __LINE__));
            }
        }

        // Invalid request: fd not open (output only).
        if (revents & POLLNVAL) {
            event_handler.on_error(make_error_code(io_errc::bad_file_descriptor));
            _pollfds.erase(pos);
            return;
        }

        // Hang up (output only).
        //
        // Contexts:
        // a. Attempt to connect to defunct server address/port
        // b. ...
        if (revents & POLLHUP) {
            event_handler.disconnected(d);
            _pollfds.erase(pos);
            return;
        }

        // There is data to read
        if (revents & POLLIN) {
            // TODO Check if this event enough to decide to disconnect.
            if (d->available() == 0) {
                event_handler.ready_read(d); // May be pending data
                event_handler.disconnected(d);
                _pollfds.erase(pos);
                return;
            } else {
                event_handler.ready_read(d);
            }
        }

        // There is urgent data to read (e.g., out-of-band data on TCP socket;
        // pseudo-terminal master in packet mode has seen state change in slave).
        if (revents & POLLPRI)
            event_handler.ready_read(d);

        // Writing is now possible, though a write larger than the available space
        // in a socket or pipe will still block (unless O_NONBLOCK is set).
        if (revents & POLLOUT)
            event_handler.can_write(d);

            // TODO
            // (since Linux 2.6.17)
//             if (revents & POLLRDHUP) {
//                 //
//                 event_handler.disconnected(d);
//             }
    }

private:
    mutex_type _mtx;
    pollfd_map _pollfds;
};

}}} // namespace pfs::io::details
