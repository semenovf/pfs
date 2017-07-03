/*
 * buffer.cpp
 *
 *  Created on: Jul 16, 2013
 *      Author: wladt
 */

#include "pfs/traits/stdcxx/vector.hpp"
#include "pfs/io/buffer.hpp"

namespace pfs { namespace io { namespace details {

struct buffer : public bits::device
{
	static const size_t default_buffer_size = 256;

    typedef traits::stdcxx::vector<byte_t> buffer_type;

    buffer_type _buffer;
    size_t      _pos;

    buffer (byte_t a[], size_t n)
        : _buffer(a, n)
        , _pos(0)
    {}

    buffer (size_t sz)
        : _buffer(sz, true)
        , _pos(0)
    {}

    virtual error_code reopen ()
    {
    	return error_code();
    }

    virtual open_mode_flags open_mode () const
    {
    	return read_write | non_blocking;
    }

    virtual size_t  bytes_available () const
    {
    	 return _buffer.size() - _pos;
    }

    virtual ssize_t read (byte_t * bytes, size_t n, error_code * ex);

    virtual ssize_t write (const byte_t * bytes, size_t n, error_code * ex);

    virtual error_code close ()
    {
    	return error_code();
    }

    virtual bool opened () const
    {
    	return true;
    }

    virtual void flush ()
    { }

    virtual bool set_nonblocking (bool)
    {
    	return true;
    }

    virtual bool is_nonblocking () const
    {
    	return true;
    }

    virtual native_handle_type native_handle () const
    {
    	return -1;
    }

    virtual device_type type () const
    {
    	return device_buffer;
    }
    
    virtual std::string url () const
    {
        return std::string("buffer");
    }
};

ssize_t buffer::read (byte_t * bytes, size_t n, error_code *)
{
    if (_pos >= _buffer.size())
        return 0;

    n = pfs::min(n, _buffer.size() - _pos);
    //buffer_type::copy(bytes, _buffer,  _pos, n);
    std::copy(_buffer.cbegin() + _pos, _buffer.cbegin() + _pos + n, bytes);
    _pos += n;

    return integral_cast_check<ssize_t>(n);
}

ssize_t buffer::write (const byte_t * bytes, size_t n, error_code *)
{
    PFS_ASSERT(numeric_limits<size_t>::max() - _pos >= n);

    size_t size = _buffer.size() - _pos;

    if (size < n) {
        //_buffer.realloc(_buffer.size() + n - size);
        _buffer.resize(_buffer.size() + n - size);
    }

    //buffer_type::copy(_buffer, bytes, _pos, n);
    memcpy(& _buffer[0] + _pos, bytes, n);

    return integral_cast_check<ssize_t>(n);
}

}}} // pfs::io::details

namespace pfs { namespace io {

template <>
device open_device<buffer> (const open_params<buffer> & op, error_code & ex)
{
	device result;

    bits::device * p = 0;

    if (op.pbytes) {
    	p = new details::buffer(op.pbytes, op.size);
    } else if (op.size > 0) {
    	p = new details::buffer(op.size);
    } else {
    	p = new details::buffer(details::buffer::default_buffer_size);
    }

	shared_ptr<bits::device> d(p);
    result._d.swap(d);

    ex.reset();

    return result;
}

}} // pfs::io
