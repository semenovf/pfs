////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2019 Vladislav Trifochkin
//
// This file is part of [pfs](https://github.com/semenovf/pfs) library.
//
// Inspired from: pfs/operationsystem/posix/io/device_notifier_pool.hpp
// Changelog:
//      2019.09.11 Initial version
////////////////////////////////////////////////////////////////////////////////
#pragma once
#include <pfs/cxx11required.hpp>
// #include <pfs/limits.hpp>
// #include <pfs/mutex.hpp>
// #include <pfs/memory.hpp>
#include <mutex>
#include <system_error>
#include <utility>
#include <vector>
// #include <pfs/system_error.hpp>
// #include <pfs/utility.hpp>
// #include <pfs/io/device.hpp>
// #include <pfs/io/server.hpp>
// #include <pfs/compare.hpp>
#include <cassert>
#include <cerrno>
// #include <pfs/debug.hpp>
#include <poll.h>
#include <sys/types.h>
#include <sys/socket.h>

#if defined __USE_XOPEN || defined __USE_XOPEN2K8
#   define WR_EVENTS_XOR_MASK_XPG42 (POLLWRNORM | POLLWRBAND)
#else
#   define WR_EVENTS_XOR_MASK_XPG42 0
#endif

#define WR_EVENTS_XOR_MASK (POLLOUT | WR_EVENTS_XOR_MASK_XPG42)

////////////////////////////////////////////////////////////////////////////////
// |           |           |           |           |
//  15 14 13 12 11 10 09 08 07 06 05 04 03 02 01 00
//         |  |     |  |  |  |  |  |  |  |  |  |  |
//         |  |     |  |  |  |  |  |  |  |  |  |  ---- POLLIN     0x0001 - There is data to read.
//         |  |     |  |  |  |  |  |  |  |  |  ------- POLLPRI    0x0002 - There is urgent data to read
//         |  |     |  |  |  |  |  |  |  |  ---------- POLLOUT    0x0004 - Writing now will not block
//         |  |     |  |  |  |  |  |  |  ------------- POLLERR    0x0008 - Error condition
//         |  |     |  |  |  |  |  |  ---------------- POLLHUP    0x0010 - Hung up
//         |  |     |  |  |  |  |  ------------------- POLLNVAL   0x0020 - Invalid polling request
//         |  |     |  |  |  |  --------------------*- POLLRDNORM 0x0040 - Normal data may be read
//         |  |     |  |  |  -----------------------*- POLLRDBAND 0x0080 - Priority data may be read
//         |  |     |  |  --------------------------*- POLLWRNORM 0x0100 - Writing now will not block
//         |  |     |  -----------------------------*- POLLWRBAND 0x0200 - Priority data may be written
//         |  |     -------------------------------**- POLLMSG    0x0400
//         |  |
//         |  -------------------------------------**- POLLREMOVE 0x1000
//         ----------------------------------------**- POLLRDHUP  0x2000
//
// * - These values are defined in XPG4.2 (defined __USE_XOPEN || defined __USE_XOPEN2K8)
// **- These are extensions for Linux (defined __USE_GNU)
//
// References:
// 1. [select() - Synchronous I/O Multiplexing](http://beej.us/guide/bgnet/output/html/multipage/advanced.html)
// 2. [7.3. Handling Partial send()s](http://beej.us/guide/bgnet/output/html/multipage/advanced.html#sendall)
// 3. [Using poll() instead of select()](https://www-01.ibm.com/support/knowledgecenter/ssw_ibm_i_71/rzab6/poll.htm)
////////////////////////////////////////////////////////////////////////////////

namespace pfs {
namespace io {

enum notification_flags {
      notify_read  = 1 << 0
    , notify_write = 1 << 1
    , notify_all   = notify_read | notify_write
};

template <typename T>
using vector_type = std::vector<T>;

//
// Requirements:
// * SocketHandler must be static_cast'ed to NativeHandler
//

template <typename SocketHandler>
struct listener_checker
{
    bool operator () (SocketHandler * p) const;
};

template <>
bool listener_checker<int>::operator () (int * p) const
{
    assert(p);
    int val;
    socklen_t len = sizeof(val);

    if (p && getsockopt(*p, SOL_SOCKET, SO_ACCEPTCONN, & val, & len) == 0)
        if (val)
            return true;

    return false;
};

template <typename SocketHandler = int
        , template <typename> class ContiguousContainer = vector_type
        , typename BasicLockable = std::mutex>
class socket_notifier_pool_posix
{
    using native_handler_type = int;
    using socket_handler_type = SocketHandler;
    using socket_handler_pointer = SocketHandler *;
    using socket_handler_const_pointer = SocketHandler const *;
    using mutex_type = BasicLockable;

    using pollfd_type = ::pollfd;
    using pollfd_vector = ContiguousContainer<pollfd_type>;
    using index_vector  = ContiguousContainer<ssize_t>;
    using socket_vector = ContiguousContainer<socket_handler_pointer>;

    class pollfd_map
    {
    public:
        pollfd_vector _pollfds;
        socket_vector _devices;
        index_vector  _pollfds_indices;
        socket_vector _deferred_sockets; // candidates for insertion before the next iteration of poll

        class revents_iterator
        {
        public:
            typename pollfd_vector::iterator pollfd_it;
            typename pollfd_vector::iterator pollfd_end;
            typename socket_vector::iterator socket_it; // TODO rename to socket_it

            revents_iterator (typename pollfd_vector::iterator p
                    , typename pollfd_vector::iterator end
                    , typename socket_vector::iterator d)
                : pollfd_it(p)
                , pollfd_end(end)
                , socket_it(d)
            {}

            revents_iterator (revents_iterator const & rhs)
                : pollfd_it(rhs.pollfd_it)
                , pollfd_end(rhs.pollfd_end)
                , socket_it(rhs.socket_it)
            {}

            revents_iterator & operator = (revents_iterator const & rhs)
            {
                pollfd_it  = rhs.pollfd_it;
                pollfd_end = rhs.pollfd_end;
                socket_it  = rhs.socket_it;
                return *this;
            }

            void set_begin ()
            {
                while (pollfd_it != pollfd_end
                        && pollfd_it->revents == 0) {
                    ++pollfd_it;
                    ++socket_it;
                }
            }

            socket_handler_const_pointer socket_ptr () const
            {
                return *socket_it;
            }

            socket_handler_pointer socket_ptr ()
            {
                return *socket_it;
            }

            short operator * () const
            {
                return pollfd_it->revents;
            }

            pollfd_type & pollfd () const
            {
                return *pollfd_it;
            }

            revents_iterator & operator ++ ()
            {
                if (pollfd_it != pollfd_end) {
                    ++pollfd_it;
                    ++socket_it;
                }

                while (pollfd_it != pollfd_end && pollfd_it->revents == 0) {
                    ++pollfd_it;
                    ++socket_it;
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

        using poll_result_type = std::pair<revents_iterator, revents_iterator>;

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

        void insert_deferred (socket_handler_pointer d)
        {
            _deferred_sockets.push_back(d);
        }

        void insert (socket_handler_pointer d, short notify_events)
        {
            short poll_events = 0;

            // Allow if Linux version match
            // (since Linux 2.6.17)
            poll_events |= POLLRDHUP;

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
                _devices.push_back(nullptr); //_devices.push_back(basic_device_ptr());
                index = _pollfds.size() - 1;
            }

            _pollfds[index].fd = d->native_handle();
            _pollfds[index].events = poll_events;
            _devices[index] = d;
        }

        poll_result_type poll (int millis, std::error_code & ec)
        {
            // Insert deferred devices
            if (_deferred_sockets.size() > 0) {
                typename socket_vector::iterator first = _deferred_sockets.begin();
                typename socket_vector::iterator last = _deferred_sockets.end();

                while (first != last) {
                    insert(*first, notify_all);
                    ++first;
                }

                _deferred_sockets.clear();
            }

            size_t n = _pollfds.size();
            pollfd_type * pfds = _pollfds.data();

            int r = 0;

            do {
                errno = 0;
                r = ::poll(pfds, n, millis);
            } while (r <= 0 && errno == EINTR);

            if (r == 0)
                return std::make_pair(revents_end(), revents_end());

            if (r < 0) {
                ec = std::error_code(errno, std::generic_category());
                return std::make_pair(revents_end(), revents_end());
            }

            return std::make_pair(revents_begin(), revents_end());
        }

        ssize_t find (socket_handler_pointer p)
        {
            typename socket_vector::iterator first = _devices.begin();
            typename socket_vector::iterator last  = _devices.end();

            ssize_t index = 0;

            while (first != last) {
                if (*first
                        && static_cast<native_handler_type>(**first)
                                == static_cast<native_handler_type>(*p))
                    return index;

                ++first;
                ++index;
            }

            return -1;
        }

        void erase (revents_iterator pos)
        {
            ssize_t index = std::distance(_pollfds.begin(), pos.pollfd_it);
            if (index >= 0) clear(static_cast<size_t>(index));
        }

        void erase (socket_handler_pointer p)
        {
            ssize_t index = find(p);
            if (index >= 0) clear(static_cast<size_t>(index));
        }

    private:
        void clear (size_t index)
        {
            _pollfds[index].fd = -1;
            _pollfds[index].events = 0;
            _pollfds[index].revents = 0;
            _pollfds_indices.push_back(index);

            socket_handler_pointer tmp;
            _devices[index].swap(tmp);
        }
    }; // pollfd_map

public:
    using revents_iterator = typename pollfd_map::revents_iterator;
    using poll_result_type = typename pollfd_map::poll_result_type;

private:
    mutex_type _mtx;
    pollfd_map _pollfds;
    listener_checker<socket_handler_type> is_listener;

public:
    socket_notifier_pool_posix () {}

    void insert (socket_handler_pointer p, short notify_events = notify_all)
    {
        std::lock_guard<mutex_type> locker(_mtx);
        _pollfds.insert(p, notify_events);
    }

    // Do not use this method directly
    void erase (socket_handler_pointer p)
    {
        _pollfds.erase(p);
    }

    template <typename EventHandler>
    void dispatch (EventHandler & event_handler, int millis = 0)
    {
        std::lock_guard<mutex_type> locker(_mtx);
        std::error_code ec;
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
                            process_listener(first->socket_ptr(), event_handler);
                        }
                    } else {
                        process_socket(first->socket_ptr(), event_handler);
                    }
                }

                ++first;
            }
        }
    }

//     device_ptr front_device ()
//     {
//         pfs::lock_guard<pfs::mutex> locker(_mtx);
//
//         typename socket_vector::iterator first = _pollfds._devices.begin();
//         typename socket_vector::iterator last  = _pollfds._devices.end();
//
//         while (first != last) {
//             if (*first && !(*first)->is_server())
//                 return pfs::static_pointer_cast<device>(*first);
//             ++first;
//         }
//
//         return device_ptr();
//     }
//
//     device_ptr front_server ()
//     {
//         pfs::lock_guard<pfs::mutex> locker(_mtx);
//
//         typename socket_vector::iterator first = _pollfds._devices.begin();
//         typename socket_vector::iterator last  = _pollfds._devices.end();
//
//         while (first != last) {
//             if (*first && (*first)->is_server())
//                 return pfs::static_pointer_cast<server>(*first);
//             ++first;
//         }
//
//         return device_ptr();
//     }
//
//     template <typename UnaryFunction>
//     void for_each_device (UnaryFunction f)
//     {
//         pfs::lock_guard<pfs::mutex> locker(_mtx);
//
//         typename socket_vector::iterator first = _pollfds._devices.begin();
//         typename socket_vector::iterator last  = _pollfds._devices.end();
//
//         while (first != last) {
//             if (*first && !(*first)->is_server())
//                 f(pfs::static_pointer_cast<device>(*first));
//             ++first;
//         }
//     }
//
//     template <typename FilterFunction, typename UnaryFunction>
//     void for_each_device (FilterFunction filter, UnaryFunction f)
//     {
//         pfs::lock_guard<pfs::mutex> locker(_mtx);
//
//         typename socket_vector::iterator first = _pollfds._devices.begin();
//         typename socket_vector::iterator last  = _pollfds._devices.end();
//
//         while (first != last) {
//             if (*first && !(*first)->is_server() && filter(pfs::static_pointer_cast<device>(*first)))
//                 f(pfs::static_pointer_cast<device>(*first));
//             ++first;
//         }
//     }
//
//     template <typename UnaryFunction>
//     void for_each_server (UnaryFunction f)
//     {
//         pfs::lock_guard<pfs::mutex> locker(_mtx);
//
//         typename socket_vector::iterator first = _pollfds._devices.begin();
//         typename socket_vector::iterator last  = _pollfds._devices.end();
//
//         while (first != last) {
//             if (*first && (*first)->is_server())
//                 f(pfs::static_pointer_cast<server>(*first));
//             ++first;
//         }
//     }
//
//     template <typename FilterFunction, typename UnaryFunction>
//     void for_each_server (FilterFunction filter, UnaryFunction f)
//     {
//         pfs::lock_guard<pfs::mutex> locker(_mtx);
//
//         typename socket_vector::iterator first = _pollfds._devices.begin();
//         typename socket_vector::iterator last  = _pollfds._devices.end();
//
//         while (first != last) {
//             if (*first && (*first)->is_server() && filter(pfs::static_pointer_cast<server>(*first)))
//                 f(pfs::static_pointer_cast<server>(*first));
//             ++first;
//         }
//     }
//
private:

    template <typename EventHandler>
    void process_listener (socket_handler_pointer pserver, EventHandler & event_handler)
    {
        std::error_code ec;
        sockaddr addr;
        socklen_t addrlen = sizeof(sockaddr);
        int peer_fd = ::accept(static_cast<native_handler_type>(*pserver), & addr, & addrlen);

        if (peer_fd < 0) {
            // Acceptance failed
            event_handler.on_error(ec);
        } else {
            socket_handler_type peer(peer_fd);
            event_handler.accepted(peer, pserver);
            _pollfds.insert_deferred(peer);
        }
    }

    template <typename EventHandler>
    void process_socket (revents_iterator pos, EventHandler & event_handler)
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
        if (revents & POLLRDHUP) {
            event_handler.disconnected(d);
        }
    }
};

}} // namespace pfs::io
