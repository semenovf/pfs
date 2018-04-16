#ifndef __PFS_IO_DEVICE_MANAGER_HPP__
#define __PFS_IO_DEVICE_MANAGER_HPP__

#include <ctime>
#include <pfs/cxxlang.hpp>
#include <pfs/sigslot.hpp>
#include <pfs/traits/stdcxx/set.hpp>
#include <pfs/io/pool.hpp>

namespace pfs {
namespace io {

// All devices must be in non-blocking mode.

template <typename SigslotNS
        , template <typename> class SequenceContainerImpl
        , template <typename> class ContigousContainerImpl
        , template <typename> class AssociativeContainerImpl>
class device_manager : SigslotNS::has_slots
{
    struct reopen_item
    {
        device d;
        time_t timeout; // reconnection timeout in seconds
        time_t start;   // start time point counting timeout from

        bool operator < (reopen_item const & x) const
        {
            return (start + timeout) <= (x.start + x.timeout);
        }
    };

    typedef pool<SequenceContainerImpl
            , ContigousContainerImpl
            , AssociativeContainerImpl> pool_type;
    typedef stdcxx::set<reopen_item>    reopen_queue;

    class dispatcher_context1 : public pool_type::dispatcher_context
    {
        friend class device_manager;

        device_manager * _m;
        pool_type * _p1;
        pool_type * _p2;

    private:
        dispatcher_context1 (int millis, short filter_events
                , device_manager * m, pool_type * p1, pool_type * p2)
            : pool_type::dispatcher_context (millis, filter_events)
            , _m (m)
            , _p1 (p1)
            , _p2 (p2)
        {}

    public:
        virtual void accepted (device & d, server & listener) pfs_override
        {
            _m->accepted(d, listener);
        }

        virtual void ready_read (device & d) pfs_override
        {
            _m->ready_read(d);
        }

        virtual void disconnected (device & d) pfs_override
        {
            _m->disconnected(d);
        }

        virtual void on_error (error_code const & ex) pfs_override
        {
            _m->error(ex);
        }
    };

    class dispatcher_context2 : public pool_type::dispatcher_context
    {
        friend class device_manager;

        static int const default_millis = 1; // TODO Need it configurable ?!

        device_manager * _m;
        pool_type *      _p1;
        pool_type *      _p2;

    private:
        dispatcher_context2 (device_manager * m, pool_type * p1, pool_type * p2)
            : pool_type::dispatcher_context(default_millis, io::poll_all) // TODO Need to reduce number of events according to specialization of this pool
            , _m(m)
            , _p1(p1)
            , _p2(p2)
        {}

    public:
        virtual void disconnected (device & d) pfs_override
        {
            _m->disconnected(d);
        }

        virtual void can_write (device & d) pfs_override
        {
            _p2->delete_deferred(d);
            _p1->push_back(d);

            _m->opened(d);
        }

        virtual void on_error (error_code const & ex) pfs_override
        {
            _m->error(ex);
        }
    };

public:
    typedef typename pool_type::device_sequence device_sequence;
    typedef typename pool_type::server_sequence server_sequence;

private:
    // Main device pool (for valid (operational) devices)
    pool_type _p1;

    // Device pool for partially-operational devices: usually in 'connection in progress...' state)
    pool_type _p2;

    // Reconnection queue, contains devices waiting reconnection by timeout
    reopen_queue _rq;

    dispatcher_context1 _ctx1;
    dispatcher_context2 _ctx2;

private:
    device_manager (device_manager const &);
    device_manager & operator = (device_manager const &);

private:
    void push_device (device const & d, pfs::error_code const & ec)
    {
        if (!ec) {
            _p1.push_back(d);
            opened(d);
        } else {
            if (ec == pfs::make_error_code(io_errc::operation_in_progress)) {
                _p2.push_back(d);
                opening(d);
            } else {
                open_failed(d, ec);
            }
        }
    }

    void push_server (server const & s, pfs::error_code const & ec)
    {
        if (!ec) {
            _p1.push_back(s);
            server_opened(s);
        } else {
            if (ec == pfs::make_error_code(io_errc::operation_in_progress)) {
                _p2.push_back(s);
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
    device_manager (int millis, int filter_events = io::poll_all)
        : _ctx1 (millis, filter_events, this, & _p1, & _p2)
        , _ctx2 (this, & _p1, & _p2)
    {}

    template <typename DeviceTag>
    device new_device (open_params<DeviceTag> const & op, pfs::error_code * pec = 0)
    {
        pfs::error_code ec;
        device d = pfs::io::open_device(op, ec);
        push_device(d, ec);

        if (pec)
            *pec = ec;

        return d;
    }

    template <typename ServerTag>
    server new_server (open_params<ServerTag> const & op, pfs::error_code * pec = 0)
    {
        pfs::error_code ec;
        server s = pfs::io::open_server(op, ec);
        push_server(s, ec);

        if (pec)
            *pec = ec;

        return s;
    }

    void push_deferred (device d, time_t reconn_timeout)
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
            device d = it->d;
            d.reopen();
            push_device(d, d.errorcode());

            ++it;
        }

        _rq.erase(_rq.cbegin(), it);
    }

    void close (device d)
    {
        _ctx1.disconnected(d);
        _p1.delete_deferred(d);
        d.close();
    }

    void fetch_devices (device_sequence & devices
            , bool (* filter) (device const & d, void * context)
            , void * context)
    {
        _p1.fetch_devices(devices, filter, context);
    }

    // TODO DEPRECATED
    device_sequence fetch_devices (
              bool (* filter) (device const & d, void * context)
            , void * context)
    {
        return _p1.fetch_devices(filter, context);
    }

    void fetch_servers (server_sequence & servers
            , bool (* filter) (server const & s, void * context)
            , void * context)
    {
        _p1.fetch_servers(servers, filter, context);
    }

    // TODO DEPRECATED
	server_sequence fetch_servers (
              bool (* filter) (server const & s, void * context)
            , void * context)
    {
        return _p1.fetch_servers(filter, context);
    }

    void dispatch ()
    {
        //if (_p1.server_count() > 0 || _p1.device_count())
        _p1.dispatch(_ctx1);

        if (_p2.device_count() > 0)
            _p2.dispatch(_ctx2);

        if (ready_deferred())
            reopen_deferred();
    }

public: // signals
    typename SigslotNS::template signal2<device, server>     accepted;     ///<! accept connection (for connection based server devices)
    typename SigslotNS::template signal1<device>             ready_read;   ///<! device is ready for read
    typename SigslotNS::template signal1<device>             opened;       ///<! opened (for regular files, servers) or connected (for connection based client devices)
    typename SigslotNS::template signal1<device>             disconnected; ///<! disconnection for connection based devices, including peer devices
    typename SigslotNS::template signal1<device>             opening;      ///<! open (connection) in progress (for connection based client devices)
    typename SigslotNS::template signal2<device, error_code> open_failed;
    typename SigslotNS::template signal1<server>             server_opened;
    typename SigslotNS::template signal1<server>             server_opening;
    typename SigslotNS::template signal2<server, error_code> server_open_failed;
    typename SigslotNS::template signal1<error_code>         error;
};

}} // pfs::io

#endif /* __PFS_IO_DEVICE_MANAGER_HPP__ */
