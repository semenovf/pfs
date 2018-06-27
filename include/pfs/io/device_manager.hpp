#pragma once
#include <ctime>
#include <pfs/cxxlang.hpp>
#include <pfs/sigslot.hpp>
#include <pfs/set.hpp>
#include <pfs/vector.hpp>
#include <pfs/io/device_notifier_pool.hpp>

namespace pfs {
namespace io {

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
        event_handler1 (device_manager * m, pool_type * p1, pool_type * p2)
            : _m(m)
            , _p1(p1)
            , _p2(p2)
        {}

    public:
        void accepted (device_ptr const & d, server_ptr const & server)
        {
            _m->accepted(d, server);
        }

        void ready_read (device_ptr const & d)
        {
            _m->ready_read(d);
        }

        void disconnected (device_ptr const & d)
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
        void disconnected (device_ptr const & d)
        {
            _m->disconnected(d);
        }

        void can_write (device_ptr const & d)
        {
            // call inside dispatcher that already lock mutex
            // so need to call version of erase without locking.
            _p2->erase(d);
            _p1->insert(d);
            _m->opened(d);
        }

        void on_error (error_code const & ex)
        {
            _m->error(ex);
        }
    };

private:
    // Main device pool (for valid (operational) devices)
    pool_type _p1;

    // Device pool for partially-operational devices: usually in 'connection in progress...' state)
    pool_type _p2;

    // Reconnection queue, contains devices waiting reconnection by timeout
    reopen_queue _rq;

    event_handler1 _evh1;
    event_handler2 _evh2;

private:
    device_manager (device_manager const &);
    device_manager & operator = (device_manager const &);

private:
    void insert_device (device_ptr const & d, pfs::error_code & ec)
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

    void insert_server (server_ptr const & s, pfs::error_code & ec)
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
        : _evh1(this, & _p1, & _p2)
        , _evh2(this, & _p1, & _p2)
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

    void push_deferred (device_ptr const & d, time_t reconn_timeout)
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

    void close (device_ptr const & d)
    {
        if (d) {
            _evh1.disconnected(d);
            _p1.erase(d);
        }
    }

    template <template <typename> class SequenenceContainer>
    void close (SequenenceContainer<device_ptr> & devices)
    {
        typename SequenenceContainer<device_ptr>::iterator first = devices.begin();
        typename SequenenceContainer<device_ptr>::iterator end = devices.end();

        for (;first != end; ++first) {
            close(*first);
        }
    }

    template <template <typename> class SequenenceContainer>
    void close (SequenenceContainer<server_ptr> & servers)
    {
        typename SequenenceContainer<server_ptr>::iterator first = servers.begin();
        typename SequenenceContainer<server_ptr>::iterator end = servers.end();

        for (;first != end; ++first) {
            close(*first);
        }
    }

    template <typename UnaryFunction>
    void for_each_device (UnaryFunction f)
    {
        _p1.template for_each_device<UnaryFunction>(f);
    }

    template <typename FilterFunction, typename UnaryFunction>
    void for_each_device (FilterFunction filter, UnaryFunction f)
    {
        _p1.template for_each_device<FilterFunction, UnaryFunction>(filter, f);
    }

    template <typename UnaryFunction>
    void for_each_server (UnaryFunction f)
    {
        _p1.template for_each_server<UnaryFunction>(f);
    }

    template <typename FilterFunction, typename UnaryFunction>
    void for_each_server (FilterFunction filter, UnaryFunction f)
    {
        _p1.template for_each_server<FilterFunction, UnaryFunction>(filter, f);
    }

    device_ptr front_device ()
    {
        return _p1.front_device();
    }

    server_ptr front_server ()
    {
        return _p1.front_server();
    }

    void dispatch (int millis = 0)
    {
        _p1.dispatch(_evh1, millis);

        //if (_p2.device_count() > 0)
        _p2.dispatch(_evh2, 0); //millis / 2 > 50 ? millis / 2 : 50);

        if (ready_deferred())
            reopen_deferred();
    }

public: // signals
    typename SigslotNS::template signal2<device_ptr, server_ptr>         accepted;     ///<! accept connection (for connection based server devices)
    typename SigslotNS::template signal1<device_ptr>                     ready_read;   ///<! device is ready for read
    typename SigslotNS::template signal1<device_ptr>                     opened;       ///<! opened (for regular files, servers) or connected (for connection based client devices)
    typename SigslotNS::template signal1<device_ptr>                     disconnected; ///<! disconnection for connection based devices, including peer devices
    typename SigslotNS::template signal1<device_ptr>                     opening;      ///<! open (connection) in progress (for connection based client devices)
    typename SigslotNS::template signal2<device_ptr, error_code const &> open_failed;
    typename SigslotNS::template signal1<server_ptr>                     server_opened;
    typename SigslotNS::template signal1<server_ptr>                     server_opening;
    typename SigslotNS::template signal2<server_ptr, error_code const &> server_open_failed;
    typename SigslotNS::template signal1<error_code const &>             error;
};

}} // pfs::io
