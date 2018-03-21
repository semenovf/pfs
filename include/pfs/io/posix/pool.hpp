#ifndef __PFS_IO_POSIX_POOL_HPP__
#define __PFS_IO_POSIX_POOL_HPP__

#include <poll.h>
#include <cerrno>
#include <pfs/mutex.hpp>
#include <pfs/io/bits/pool.hpp>
#include <pfs/io/device.hpp>
#include <pfs/io/server.hpp>
#include <pfs/traits/associative_container.hpp>
#include <pfs/traits/contigous_container.hpp>
#include <pfs/traits/sequence_container.hpp>

#if __COMMENT__ // from bits/poll.h (linux)

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

#endif

/* References:
 * 1. [select()—Synchronous I/O Multiplexing](http://beej.us/guide/bgnet/output/html/multipage/advanced.html)
 * 2. [7.3. Handling Partial send()s](http://beej.us/guide/bgnet/output/html/multipage/advanced.html#sendall)
 * 2. [Using poll() instead of select()](https://www-01.ibm.com/support/knowledgecenter/ssw_ibm_i_71/rzab6/poll.htm)
 */

#if defined __USE_XOPEN || defined __USE_XOPEN2K8
#   define WR_EVENTS_XOR_MASK_XPG42 (POLLWRNORM | POLLWRBAND)
#else
#   define WR_EVENTS_XOR_MASK_XPG42 0
#endif

#define WR_EVENTS_XOR_MASK (POLLOUT | WR_EVENTS_XOR_MASK_XPG42)

namespace pfs { namespace io { namespace details {

template <template <typename> class SequenceContainerImpl
        , template <typename> class ContigousContainerImpl
        , template <typename> class AssociativeContainerImpl>
struct pool : public bits::pool
{
    struct iterator;

    typedef ::pollfd pollfd_type;
    typedef io::device::native_handle_type native_handle_type;

    typedef traits::contigous_container<
            pollfd_type, ContigousContainerImpl>           pollfd_vector;
    typedef traits::associative_container<
            traits::kv<native_handle_type, io::device>
                , AssociativeContainerImpl>                device_map;
    typedef traits::associative_container<
            traits::kv<native_handle_type, io::server>
                , AssociativeContainerImpl>                server_map;
    typedef traits::sequence_container<io::device
            , SequenceContainerImpl>                       device_sequence;
    typedef traits::sequence_container<io::server
            , SequenceContainerImpl>                       server_sequence;

    pfs::mutex _mtx;
    device_map _device_map;
    server_map _server_map;

    pollfd_vector _pollfds;
    bool _update;  // need updated 'pollfds' before poll() system call.

    pool ()
        : _update(true)
    {}

    void update_pollfd (short events)
    {
        pfs::lock_guard<pfs::mutex> locker(_mtx);

        _pollfds.clear();
        _pollfds.reserve(_server_map.size() + _device_map.size());

        if (_server_map.size() > 0) {
            typename server_map::const_iterator it = _server_map.cbegin();
            typename server_map::const_iterator it_end = _server_map.cend();

            while (it != it_end) {
                pollfd_type pfd;
                pfd.fd = it->second.native_handle();
                pfd.events = events;

                PFS_ASSERT(pfd.fd >= 0);

                _pollfds.push_back(pfd);

                ++it;
            }
        }

        if (_device_map.size() > 0) {
            typename device_map::const_iterator it = _device_map.cbegin();
            typename device_map::const_iterator it_end = _device_map.cend();

            while (it != it_end) {
                pollfd_type pfd;
                pfd.fd = it->second.native_handle();
                pfd.events = events;

                PFS_ASSERT(pfd.fd >= 0);

                _pollfds.push_back(pfd);

                ++it;
            }
        }
    }

    void push_back (io::device d, short /*events*/)
    {
        pfs::lock_guard<pfs::mutex> locker(_mtx);
        _device_map.insert(d.native_handle(), d);
        _update = true;
    }

	void push_back (io::server s, short /*events*/)
	{
		pfs::lock_guard<pfs::mutex> locker(_mtx);
		_server_map.insert(s.native_handle(), s);
		_update = true;
	}

	void delete_deferred (io::device d)
	{
		pfs::lock_guard<pfs::mutex> locker(_mtx);
		_device_map.erase(d.native_handle());
		_update = true;
	}

	void delete_deferred (io::server s)
	{
		pfs::lock_guard<pfs::mutex> locker(_mtx);
		_server_map.erase(s.native_handle());
		_update = true;
	}

	int poll (iterator ** begin
			, iterator ** end
			, short filter_events
			, int millis
			, error_code * ex)
    {
        if (_update) {
            update_pollfd(filter_events);
            _update = false;
        }

        size_t n = _pollfds.size();
        pollfd_type * pfds = _pollfds.data();

        int r = 0;

        do {
            r = ::poll(pfds, n, millis);
        } while (r <= 0 and errno == EINTR);

        *begin = 0;
        *end = 0;

        if (r > 0) {
            *begin = iterator::alloc_begin(filter_events, *this);
            *end   = iterator::alloc_end(filter_events, *this);
        } else if (r < 0) {
            if (ex)
                *ex = error_code(errno, pfs::generic_category());
        }

        return r;
    }

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

    // TODO DEPRECATED
    device_sequence fetch_devices (bool (* filter) (device const & d, void * context)
            , void * context)
    {
        device_sequence result;
        fetch_devices(result, filter, context);
        return result;
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

    // TODO DEPRECATED
    server_sequence fetch_servers (bool (* filter) (server const & s, void * context)
            , void * context)
    {
        server_sequence result;
        fetch_servers(result, filter, context);
        return result;
    }

    struct iterator : public bits::pool_iterator
    {
    public:
        typedef typename pool::pollfd_vector::const_iterator pointer;

        short filter_events;
        pointer ptr;
        pointer ptr_end;

    protected:
        iterator (short events, pointer begin, pointer end)
            : filter_events(events)
            , ptr(begin)
            , ptr_end(end)
        {}

    public:
        virtual ~iterator () {}

        virtual void next ()
        {
            while (++ptr != ptr_end) {
                if (ptr->revents & filter_events)
                    break;
            }
        }

        static iterator * alloc_begin (short filter_events, pool const & p)
        {
            pointer first = p._pollfds.cbegin();
            pointer last   = p._pollfds.cend();

            while (first != last) {
                if (first->revents & filter_events)
                    break;
                ++first;
            }

            return new iterator(filter_events, first, last);
        }

        static iterator * alloc_end (short filter_events, pool const & p)
        {
            return new iterator(filter_events
                , p._pollfds.cend()
                , p._pollfds.cend());
        }

        bool eq (iterator & rhs) const
        {
            return ptr == rhs.ptr;
        }

        short revents () const
        {
            return ptr->revents;
        }
    }; // iterator
};

}}} // pfs::io::details

#endif
