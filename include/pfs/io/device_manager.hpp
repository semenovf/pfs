#pragma once
#include <ctime>
#include <pfs/cxxlang.hpp>
#include <pfs/sigslot.hpp>
#include <pfs/set.hpp>
#include <pfs/vector.hpp>
#include <pfs/io/device_notifier_pool.hpp>

#include <pfs/debug.hpp>

namespace pfs {
namespace io {

// All devices must be in non-blocking mode.

template <typename SigslotNS = pfs::sigslot<>
        , template <typename> class ContigousContainer = pfs::vector
        , typename BasicLockable = pfs::mutex
        , template <typename> class PriorityContainer = pfs::set>
class device_manager : SigslotNS::has_slots
{
    struct reopen_item
    {
        device_ptr d;
        time_t timeout; // reconnection timeout in seconds
        time_t start;   // start time point counting timeout from

        bool operator < (reopen_item const & x) const
        {
            return (start + timeout) <= (x.start + x.timeout);
        }
    };

    typedef device_notifier_pool<ContigousContainer, BasicLockable> pool_type;
    typedef PriorityContainer<reopen_item> reopen_queue;

    class event_handler1 : public default_event_handler
    {
        friend class device_manager;

        device_manager * _m;
        pool_type * _p1;
        pool_type * _p2;

    private:
        event_handler1 (/*int millis, short filter_events
                , */device_manager * m, pool_type * p1, pool_type * p2)
//            : pool_type::dispatcher_context (millis, filter_events)
            : _m(m)
            , _p1(p1)
            , _p2(p2)
        {}

    public:
        void accepted (device_ptr & d, server_ptr & server)
        {
            _m->accepted(d, server);
        }

        void ready_read (device_ptr & d)
        {
            _m->ready_read(d);
        }

        void disconnected (device_ptr & d)
        {
            _m->disconnected(d);
        }

        void on_error (error_code const & ex)
        {
            _m->error(ex);
        }
    };

    class event_handler2 : public default_event_handler
    {
        friend class device_manager;

        static int const default_millis = 1; // TODO Need it configurable ?!

        device_manager * _m;
        pool_type *      _p1;
        pool_type *      _p2;

    private:
        event_handler2 (device_manager * m, pool_type * p1, pool_type * p2)
//            : pool_type::dispatcher_context(default_millis, io::poll_all) // TODO Need to reduce number of events according to specialization of this pool
            : _m(m)
            , _p1(p1)
            , _p2(p2)
        {}

    public:
        void disconnected (device_ptr & d)
        {
            _m->disconnected(d);
        }

        void can_write (device_ptr & d)
        {
            _p2->remove(d);
            _p1->insert(d);

            _m->opened(d);
        }

        void on_error (error_code const & ex)
        {
            _m->error(ex);
        }
    };

// public:
//     typedef typename pool_type::device_sequence device_sequence;
//     typedef typename pool_type::server_sequence server_sequence;

private:
    // Main device pool (for valid (operational) devices)
    pool_type _p1;

    // Device pool for partially-operational devices: usually in 'connection in progress...' state)
    pool_type _p2;

    // Reconnection queue, contains devices waiting reconnection by timeout
    reopen_queue _rq;

    event_handler1 _ctx1;
    event_handler2 _ctx2;

private:
    device_manager (device_manager const &);
    device_manager & operator = (device_manager const &);

private:
    void insert_device (device_ptr d, pfs::error_code & ec)
    {
        if (!ec) {
            _p1.insert(d);
            opened(d);
        } else {
            if (ec == pfs::make_error_code(io_errc::operation_in_progress)) {
                _p2.insert(d);
                opening(d);
            } else {
                open_failed(d, ec);
            }
        }
    }

    void insert_server (server_ptr s, pfs::error_code & ec)
    {
        if (!ec) {
            _p1.insert(s);
            server_opened(s);
        } else {
            if (ec == pfs::make_error_code(io_errc::operation_in_progress)) {
                _p2.insert(s);
                server_opening(s);
            } else {
                server_open_failed(s, ec);
            }
        }
    }

public:
    /**
     * @brief Construct device manager.
     * @param millis        Poll timeout in milliseconds.
     * @param filter_events Accepted poll events.
     */
    device_manager ()
        : _ctx1(this, & _p1, & _p2)
        , _ctx2(this, & _p1, & _p2)
    {}

    template <typename DeviceTag>
    device_ptr new_device (open_params<DeviceTag> const & op, pfs::error_code & ec)
    {
        device_ptr d = pfs::io::open_device(op, ec);
        insert_device(d, ec);
        return d;
    }

    template <typename ServerTag>
    server_ptr new_server (open_params<ServerTag> const & op, pfs::error_code & ec)
    {
        server_ptr s = pfs::io::open_server(op, ec);
        insert_server(s, ec);
        return s;
    }

    void push_deferred (device_ptr d, time_t reconn_timeout)
    {
        //PFS_ASSERT(d.set_nonblocking(true));
        reopen_item item;
        item.d       = d;
        item.timeout = reconn_timeout;
        item.start   = ::time(0); // TODO may be need to use monotonic clock

        _rq.insert(item);
    }

    /**
     * @brief Checks if reopen timeout is exceeded for one or more devices.
     */
    bool ready_deferred () const
    {
        if (_rq.empty())
            return false;

        // Create temporary item to compare (device field does not matter in comparison)
        reopen_item item;
        item.timeout = 0;
        item.start   = ::time(0); // TODO may be need to use monotonic clock

        // Checking first item will be enough.
        return (*_rq.begin() < item) ? true : false;
    }

    /**
     * @brief Reopen 'ready' devices.
     */
    void reopen_deferred ()
    {
        if (_rq.empty())
            return;

        // Create temporary item to compare (device field does not matter in comparison)
        reopen_item item;
        item.timeout = 0;
        item.start   = ::time(0); // TODO may be need to use monotonic clock

        typename reopen_queue::const_iterator it = _rq.begin();
        typename reopen_queue::const_iterator last = _rq.end();

        while (*it < item && it != last) {
            device_ptr d = it->d;
            error_code ec = d->reopen();
            insert_device(d, ec);

            ++it;
        }

        _rq.erase(_rq.cbegin(), it);
    }

    void close (device_ptr & d)
    {
        _ctx1.disconnected(d);
        _p1.erase(d);
    }

    size_t count_devices (bool (* filter) (device_ptr const & d, void * context)
            , void * context = 0)
    {
        return _p1.count_devices(filter, context);
    }

    size_t count_devices ()
    {
        return _p1.count_devices(0, 0);
    }

    template <template <typename> class SequenenceContainer>
    size_t fetch_devices (SequenenceContainer<device_ptr> & devices
            , bool (* filter) (device_ptr const & d, void * context)
            , void * context = 0)
    {
        return _p1.fetch_devices(devices, filter, context);
    }

    template <template <typename> class SequenenceContainer>
    size_t fetch_devices (SequenenceContainer<device_ptr> & devices)
    {
        return _p1.fetch_devices(devices, 0, 0);
    }

    size_t count_servers (bool (* filter) (server_ptr const & s, void * context)
            , void * context = 0)
    {
        return _p1.count_servers(filter, context);
    }

    size_t count_servers ()
    {
        return _p1.count_servers(0, 0);
    }

    template <template <typename> class SequenenceContainer>
    size_t fetch_servers (SequenenceContainer<server_ptr> & servers
            , bool (* filter) (server_ptr const & s, void * context)
            , void * context = 0)
    {
        return _p1.fetch_servers(servers, filter, context);
    }

    template <template <typename> class SequenenceContainer>
    size_t fetch_servers (SequenenceContainer<server_ptr> & servers)
    {
        return _p1.fetch_servers(servers, 0, 0);
    }

    void dispatch (int millis = 0)
    {
        _p1.dispatch(_ctx1, millis);

        //if (_p2.device_count() > 0)
        _p2.dispatch(_ctx2, 0); //millis / 2 > 50 ? millis / 2 : 50);

        if (ready_deferred())
            reopen_deferred();
    }

public: // signals
    typename SigslotNS::template signal2<device_ptr &, server_ptr &>       accepted;     ///<! accept connection (for connection based server devices)
    typename SigslotNS::template signal1<device_ptr &>                     ready_read;   ///<! device is ready for read
    typename SigslotNS::template signal1<device_ptr &>                     opened;       ///<! opened (for regular files, servers) or connected (for connection based client devices)
    typename SigslotNS::template signal1<device_ptr &>                     disconnected; ///<! disconnection for connection based devices, including peer devices
    typename SigslotNS::template signal1<device_ptr &>                     opening;      ///<! open (connection) in progress (for connection based client devices)
    typename SigslotNS::template signal2<device_ptr &, error_code const &> open_failed;
    typename SigslotNS::template signal1<server_ptr &>                     server_opened;
    typename SigslotNS::template signal1<server_ptr &>                     server_opening;
    typename SigslotNS::template signal2<server_ptr &, error_code const &> server_open_failed;
    typename SigslotNS::template signal1<error_code const &>               error;
};

}} // pfs::io
