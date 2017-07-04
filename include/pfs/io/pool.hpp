/*
 * device_pool.hpp
 *
 *  Created on: Jan 13, 2016
 *      Author: wladt
 */

#ifndef __PFS_IO_POOL_HPP__
#define __PFS_IO_POOL_HPP__

#include <pfs/atomic.hpp>
//#include <pfs/memory.hpp>
//#include <pfs/traits/sequence_container.hpp>
#include <pfs/operationsystem.hpp>

#if PFS_OS_POSIX

#   include <pfs/io/posix/pool.hpp>

#endif

#include <pfs/debug.hpp>

namespace pfs {
namespace io {

enum poll_enum {
	  poll_in   = 0x0001  // POLLIN There is data to read.
	, poll_pri  = 0x0002  // POLLPRI There is urgent data to read (e.g., out-of-band data on TCP socket; pseudoterminal master in packet mode has seen state change in slave).
	, poll_out  = 0x0004  // POLLOUT Writing is now possible, though a write larger that the available space in a socket or pipe will still block (unless O_NONBLOCK is set).
	//, POLLRDHUP (since Linux 2.6.17) Stream socket peer closed connection, or shut down writing half of connection.  The _GNU_SOURCE feature test macro must be defined (before including any header files) in order to obtain this definition.
	, poll_err  = 0x0008  // POLLERR Error condition (output only).
	, poll_hup  = 0x0010  // POLLHUP Hang up (output only).
	, poll_nval = 0x0020  // POLLNVAL Invalid request: fd not open (output only).

	, poll_all  = 0xFFFF
};

template <template <typename> class SequenceContainer
        , template <typename> class ContigousContainer
        , template <typename> class AssociativeContainer>
class pool
{
public:
    typedef details::pool<SequenceContainer
            , ContigousContainer
            , AssociativeContainer>         pool_impl;
    
    
	enum value_enum
	{
		  Null
		, Listener
		, Device
	};

	class value
	{
		friend class pool;

		value_enum type;
		server s;
		device d;

	public:
		value ()
			: type(Null)
		{}

		value (const value & other)
			: type(other.type)
			, s(other.s)
			, d(other.d)
		{}

		value & operator = (const value & other)
		{
			type = other.type;
			s = other.s;
			d = other.d;
			return *this;
		}

		value (const server & other)
			: type(Listener)
			, s(other)
		{}

		value (const device & other)
			: type(Device)
			, d(other)
		{}

		server & get_server ()
		{
			return s;
		}

		server & get_listener ()
		{
			return s;
		}

		device & get_device ()
		{
			return d;
		}

		const server & get_server () const
		{
			return s;
		}

		const server & get_listener () const
		{
			return s;
		}

		const device & get_device () const
		{
			return d;
		}

		bool is_server () const
		{
			return type == Listener;
		}

		bool is_device () const
		{
			return type == Device;
		}

		bool operator == (const server & rhs) const
		{
			return type == Listener && s == rhs;
		}

		bool operator == (const device & rhs) const
		{
			return type == Device && d == rhs;
		}

		bool operator != (const server & rhs) const
		{
			return ! operator == (rhs);
		}

		bool operator != (const device & rhs) const
		{
			return ! operator == (rhs);
		}
	};

	class iterator
	{
		friend class pool;

		pool * _powner;
		shared_ptr<bits::pool_iterator> _d;

	protected:
		iterator (pool * owner, bits::pool_iterator * pd)
			: _powner(owner)
			, _d(pd)

		{}

	public:
		iterator ()
			: _powner(0)
		{}

		iterator & operator ++ ()
		{
			_d->next();
			return *this;
		}

        // No need to implement
//		iterator operator ++ (int)
//		{}

		value operator * () const
        {
            if (_d) {
                typename pool_impl::iterator * details_it 
                        = static_cast<typename pool_impl::iterator *> (_d.get());
                pool_impl * details_pool = static_cast<pool_impl *> (_powner->_d.get());

                typename pool_impl::native_handle_type fd = details_it->ptr->fd;
                PFS_ASSERT(fd >= 0);

                // Search through servers
                //
                {
                    pfs::lock_guard<pfs::mutex> locker(details_pool->mtx);

                    typename pool_impl::server_map_type::const_iterator it
                            = details_pool->server_map.find(fd);

                    if (it != details_pool->server_map.cend()) {
                        return pool::value(it->second);
                    }
                }

                // Search through devices
                //
                {
                    pfs::lock_guard<pfs::mutex> locker(details_pool->mtx);

                    typename pool_impl::device_map_type::const_iterator it
                            = details_pool->device_map.find(fd);

                    if (it != details_pool->device_map.cend()) {
                        return pool::value(it->second);
                    }
                }

                PFS_ASSERT_X(false, "Expected server or device found at pool");
            }

            return pool::value();
        }
        
		bool operator == (iterator const & rhs) const
        {
            typename pool_impl::iterator * it1 = 
                    static_cast<typename pool_impl::iterator *>(_d.get());
            typename pool_impl::iterator * it2 =
                    static_cast<typename pool_impl::iterator *>(rhs._d.get());

            if (it1 == 0 && it2 == 0)
                return true;

            PFS_ASSERT(it1);
            PFS_ASSERT(it2);
            return it1->eq(*it2);
        }        

		bool operator != (const iterator & rhs) const
		{
			return ! operator == (rhs);
		}

		short revents () const
        {
            typename pool_impl::iterator * it =
                    static_cast<typename pool_impl::iterator *> (_d.get());
            return it->revents();
        }
    };

private:
	shared_ptr<bits::pool> _d;

protected:
	pool (bits::pool * pd)
		: _d(pd)
	{}

    void swap (pool & other)
    {
    	_d.swap(other._d);
    }

public:
	pool ()
    	: _d(new pool_impl())
    {}


	size_t device_count () const
    {
        pool_impl * pdp = static_cast<pool_impl *>(_d.get());
        return pdp->device_map.size();
    }

	size_t server_count () const
    {
        pool_impl * pdp = static_cast<pool_impl *>(_d.get());
        return pdp->server_map.size();
    }

	void push_back (device d, int events = poll_all)
    {
    	PFS_ASSERT(_d);
        pool_impl * pdp = static_cast<pool_impl *>(_d.get());
        pdp->push_back(d, events);
    }

	void push_back (server s, int events = poll_all)
    {
        PFS_ASSERT(_d);
        pool_impl * pdp = static_cast<pool_impl *>(_d.get());
        pdp->push_back(s, events);
    }


	void delete_deferred (device d)
    {
        PFS_ASSERT(_d);
        pool_impl * pdp = static_cast<pool_impl *>(_d.get());
        pdp->delete_deferred(d);
    }

	void delete_deferred (server s)
    {
        PFS_ASSERT(_d);
        pool_impl * pdp = static_cast<pool_impl *> (_d.get());
        pdp->delete_deferred(s);
    }

	typename pool_impl::device_vector_type fetch_devices (
              bool (* filter) (device const & d, void * context)
            , void * context)
    {
        PFS_ASSERT(_d);
        pool_impl * pdp = static_cast<pool_impl *>(_d.get());
        return  pdp->fetch_devices(filter, context);
    }
    
	typename pool_impl::server_vector_type fetch_servers (
              bool (* filter) (server const & s, void * context)
            , void * context)
    {
        PFS_ASSERT(_d);
        pool_impl * pdp = static_cast<pool_impl *>(_d.get());
        return pdp->fetch_servers(filter, context);
    }

	typedef pfs::pair<pool::iterator, pool::iterator> poll_result_type;

	/**
	 * @brief Wait for some event on a file descriptor.
	 *
	 * @details The call will block until either
	 * 		@li a device becomes ready;
	 * 		@li the call is interrupted by a signal handler; or
	 * 		@li the timeout expires.
	 *
	 * @param filter_events
	 * @param millis The timeout argument specifies the number of milliseconds
	 * 		that poll() should block waiting for a device to become ready.
	 * 		Zero value Causes poll() to return immediately, even if no devices
	 * 		are ready.
	 * @param devices List of devices become ready.
	 * @param ex Pointer to store error code on error.
	 *
	 * @return On success, a positive number is returned;
	 * 		this is the number of descriptors with events or errors reported.
	 * 		A value of 0 indicates that the call timed out and no file
	 * 		descriptors were ready.
	 * 		On error, -1 is returned, and @a *ex is set appropriately.
	 */
	poll_result_type poll (int filter_events = poll_all
			 , int millis = 0
			 , error_code * ec = 0)
    {
        pool_impl * pdp = static_cast<pool_impl *> (_d.get());
        typename pool_impl::iterator * begin = 0;
        typename pool_impl::iterator * end = 0;

        int r = pdp->poll(& begin, & end, filter_events, millis, ec);

        if (r > 0) {
            return poll_result_type(pool::iterator(this, static_cast<bits::pool_iterator *> (begin))
                    , pool::iterator(this, static_cast<bits::pool_iterator *> (end)));
        }

        return poll_result_type(pool::iterator(), pool::iterator());
    }            

	class dispatcher_context2
	{
		friend class pool;

		int _millis;
		int _filter_events;

	public:
		dispatcher_context2 ()
			: _millis(0)
			, _filter_events(poll_all)
		{}

		dispatcher_context2 (int millis)
			: _millis(millis)
			, _filter_events(poll_all)
		{}

		dispatcher_context2 (int millis, int filter_events)
			: _millis(millis)
			, _filter_events(filter_events)
		{}

		virtual ~dispatcher_context2 () {}

	public:
		virtual void accepted (device &, server & listener) const {}
		virtual void ready_read (device &) const {}
		virtual void disconnected (device &) const {}
		virtual void can_write (device &) const {}
		virtual void on_error (error_code const &) const {}
	};

	void dispatch (dispatcher_context2 const & context)
    {
        pfs::error_code ex;
        poll_result_type result = this->poll(context._filter_events, context._millis, & ex);

        if (ex) {
            context.on_error(ex);
        } else if (result.first != result.second) {
            pool::iterator it = result.first;
            pool::iterator it_end = result.second;

            while (it != it_end) {
                pool::value value = *it;

                short revents = it.revents();

                if (value.is_server()) { // accept connection
                    // Servers wait incoming data (to establish connection)
                    // so ignore write events
                    //
                    if ((revents ^ WR_EVENTS_XOR_MASK) == 0) {
                        ; // TODO here may be need check opening/opened state
                    } else {
                        pfs::io::server server = value.get_server();
                        process_server(server, context, revents);
                    }
                } else {
                    pfs::io::device dev = value.get_device();
                    process_device(dev, context, revents);
                }

                ++it;
            }
        }
    }

private:
        // Used by dispatch(...)
    void process_server (pfs::io::server & server
            , pool::dispatcher_context2 const & context
            , short revents)
    {
        pfs::io::device client;
        pfs::error_code ex = server.accept(client, true);

        if (ex) {
            // Acceptance failed
            //
            context.on_error(ex);
        } else {
            // Accepted
            //
            context.accepted(client, server);

            switch (server.type()) {
            case pfs::io::server_udp:
                // Process peer device on the spot
                // and release it automatically
                process_device(client, context, revents);
                break;

            case pfs::io::server_tcp:
                this->push_back(client, context._filter_events);
                break;

            default:
                PFS_DEBUG(fprintf(stderr, "**WARN: untested server type: %d\n", server.type()));
                this->push_back(client, context._filter_events);
                break;
            }
        }
    }    
    
    // Used by dispatch(...)
    void process_device (pfs::io::device & dev
            , pool::dispatcher_context2 const & context
            , short revents)
    {
        if (dev.available() == 0
                && (revents & poll_in)) { // TODO Check if this event enough to decide to disconnect.
            this->delete_deferred(dev);
            dev.close();
            context.disconnected(dev);
        } else {
            // Error condition (output only).
            //
            // TODO Research this feature and implement handling
            //
            if (revents & poll_err) {
                PFS_DEBUG(puts("pfs::io::pool::dispatch(): device error condition"));
            }

            // Hang up (output only).
            //
            if (revents & poll_hup) {
                this->delete_deferred(dev);
                dev.close();
                context.disconnected(dev);
            }

            // There is urgent data to read (e.g., out-of-band data on TCP socket;
            // pseudo-terminal master in packet mode has seen state change in slave).
            //
            if (revents & poll_pri) {
                context.ready_read(dev);
            }

            // There is data to read
            //
            if (revents & poll_in) {
                context.ready_read(dev);
            }

            // Writing is now possible, though a write larger than the available space
            // in a socket or pipe will still block (unless O_NONBLOCK is set).
            //
            if (revents & poll_out) {
                context.can_write(dev);
            }

            // Invalid request: fd not open (output only).
            //
            if (revents & poll_nval) {
                this->delete_deferred(dev);
                dev.close();
                context.on_error(make_error_code(io_errc::bas_file_descriptor));
            }
        }
    }
};

}} // pfs::io

#endif /* __PFS_IO_POOL_HPP__ */
