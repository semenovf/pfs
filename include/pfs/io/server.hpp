#pragma once
#include <pfs/memory.hpp>
#include <pfs/io/bits/server.hpp>
#include <pfs/io/device.hpp>

namespace pfs {
namespace io {

template <typename ServerImpl>
struct open_params;

class server
{
public:
	typedef device::native_handle_type native_handle_type;
	typedef device::context_type       context_type;

protected:
    shared_ptr<bits::server> _d;

protected:
    server (bits::server * p)
		: _d(p)
	{}

public:
    server ()
        : _d()
    {}

    ~server ()
    {}

    native_handle_type native_handle () const
    {
        return _d->native_handle();
    }

    operator bool () const
    {
        return opened();
    }

    bool is_error () const
    {
        return _d->is_error();
    }

    error_code errorcode () const
    {
        return _d->errorcode();
    }

    bool is_null () const
    {
        return !_d;
    }

    bool opened () const
    {
        return _d ? _d->opened() : false;
    }

    bool set_nonblocking (bool on)
    {
        return _d->set_nonblocking(on);
    }

    bool is_nonblocking () const
    {
        return _d->is_nonblocking();
    }

    /**
     * @brief Close device.
     * @return @c true if device closed successfully,
     *         @c false if error occurred while closing.
     *         In latter case error will be stored in the internal
     *         notification storage
     * @see    device::notification()
     */
    bool close ();

    /**
     *
     * @param
     * @param non_blocking Sets non-blocking to the accepted device.
     *		Using this flag saves extra calls to set_non_blocking() call.
     *
     * @return
     */
    error_code accept (device & peer, bool non_blocking);

    server_type type () const
    {
        return _d->type();
    }

    void set_context (context_type * ctx)
    {
        _d->set_context(ctx);
    }

    const context_type * context () const
    {
        return _d->context();
    }

    context_type * context ()
    {
        return _d->context();
    }

    std::string url () const
    {
        return _d->url();
    }

    bool operator == (const server & other)
    {
        return _d == other._d;
    }

    bool operator != (const server & other)
    {
        return ! operator == (other);
    }

    template <typename ServerTag>
    friend server open_server (open_params<ServerTag> const &, error_code & ec);
};

template <typename ServerTag>
server open_server (open_params<ServerTag> const &, error_code & ec);

}} // pfs::io
