/* 
 * File:   device_manager.hpp
 * Author: wladt
 *
 * Created on November 14, 2016, 4:29 PM
 */

#ifndef __PFS_IO_DEVICE_MANAGER_HPP__
#define __PFS_IO_DEVICE_MANAGER_HPP__

#include <ctime>
#include <pfs/sigslot.hpp>
#include <pfs/traits/stdcxx/set.hpp>
#include <pfs/io/pool.hpp>

namespace pfs {
namespace io {

// All devices must be in non-blocking mode.

template <template <typename> class SequenceContainer
        , template <typename> class ContigousContainer
        , template <typename> class AssociativeContainer>
class device_manager : has_slots<>
{
    typedef pool<SequenceContainer
            , ContigousContainer
            , AssociativeContainer> pool_type;

    struct reopen_item
    {
        device d;
        time_t timeout; // reconnection timeout in seconds
        time_t start; // start time point counting timeout from

        bool operator< (reopen_item const & x) const
        {
            return (start + timeout) < (x.start + x.timeout);
        }
    };

    typedef traits::stdcxx::set<reopen_item> reopen_queue_type;

    class dispatcher_context1 : public pool_type::dispatcher_context2
    {
        friend class device_manager;

        device_manager * _m;
        pool_type * _p1;
        pool_type * _p2;

    private:

        dispatcher_context1 (int millis, short filter_events
                , device_manager * m, pool_type * p1, pool_type * p2)
            : pool_type::dispatcher_context2 (millis, filter_events)
            , _m (m)
            , _p1 (p1)
            , _p2 (p2)
        {}

    public:

        virtual void accepted (device & d, server & listener) const
        {
            _m->accepted(d, listener);
        }

        virtual void ready_read (device & d) const
        {
            _m->ready_read(d);
        }

        virtual void disconnected (device & d) const
        {
            _m->disconnected(d);
        }

        virtual void on_error (error_code const & ex) const
        {
            _m->error(ex);
        }
    };

    class dispatcher_context2 : public pool_type::dispatcher_context2
    {
        friend class device_manager;

        static int const default_millis = 1; // TODO Need it configurable ?!

        device_manager * _m;
        pool_type * _p1;
        pool_type * _p2;

    private:
        dispatcher_context2 (device_manager * m, pool_type * p1, pool_type * p2)
        : pool_type::dispatcher_context2 (default_millis, io::poll_all) // TODO Need to reduce number of events according to specialization of this pool
        , _m (m)
        , _p1 (p1)
        , _p2 (p2)
        {
        }

    public:
        virtual void disconnected (device & d) const
        {
            _m->disconnected(d);
        }

        virtual void can_write (device & d) const
        {
            _p2->delete_deferred(d);
            _p1->push_back(d);

            _m->opened(d);
        }

        virtual void on_error (error_code const & ex) const
        {
            _m->error(ex);
        }
    };

    // Main device pool (for valid (operational) devices)
    pool_type _p1;

    // Device pool for partially-operational devices: usually in 'connection in progress...' state)
    pool_type _p2;

    // Reconnection queue, contains devices waiting reconnection by timeout
    reopen_queue_type _rq;

    dispatcher_context1 _ctx1;
    dispatcher_context2 _ctx2;

private:
    device_manager (device_manager const &);
    device_manager & operator= (device_manager const &);

private:

    void push_device (device d, pfs::error_code const & ec)
    {
        if (!ec) {
            _p1.push_back(d);
            opened(d);
        } else {
            if (ec == io_errc::operation_in_progress) {
                _p2.push_back(d);
                opening(d);
            } else {
                open_failed(d, ec);
            }
        }
    }

    void push_server (server s, pfs::error_code const & ec)
    {
        if (!ec) {
            _p1.push_back(s);
            server_opened(s);
        } else {
            if (ec == io_errc::operation_in_progress) {
                _p2.push_back(s);
                server_opening(s);
            } else {
                server_open_failed(s, ec);
            }
        }
    }

public:
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
        PFS_ASSERT(d.set_nonblocking(true));
        reopen_item item;
        item.d = d;
        item.timeout = reconn_timeout;
        item.start = time(0); // TODO may be need to use monotonic clock

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
        item.start = time(0); // TODO may be need to use monotonic clock

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
        item.start = time(0); // TODO may be need to use monotonic clock

        typename reopen_queue_type::const_iterator it = _rq.begin();
        typename reopen_queue_type::const_iterator last = _rq.end();

        while (*it < item && it != last) {
            device d = it->d;
            pfs::error_code ec = d.reopen();
            push_device(d, ec);

            ++it;
        }

        _rq.erase(_rq.cbegin(), it);
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
    signal2<device, server> accepted; // accept connection (for connection based server devices)
    signal1<device> ready_read;
    signal1<device> opened; // opened (for regular files, servers) or connected (for connection based client devices)
    signal1<device> disconnected; // disconnection for connection based devices, including peer devices
    signal1<device> opening; // open (connection) in progress (for connection based client devices)
    signal2<device, error_code> open_failed;
    signal1<server> server_opened;
    signal1<server> server_opening;
    signal2<server, error_code> server_open_failed;
    signal1<error_code> error;
};

}} // pfs::io

#endif /* __PFS_IO_DEVICE_MANAGER_HPP__ */
