#include <cstdlib>
#include <pfs/io/buffered_device.hpp>

namespace pfs {
namespace io {

buffered_device::buffered_device (device & d, size_t initialSize)
: _d (d)
, _count (0)
, _cursor (0)
{
    _buffer = static_cast<byte_t *> (std::malloc(initialSize * sizeof (byte_t)));
    _bufsz = initialSize;
}

buffered_device::~buffered_device ()
{
    if (_buffer)
        std::free(_buffer);
}

ssize_t buffered_device::upload_bytes (size_t max_size)
{
    if (max_size == 0)
        return 0;

    if (max_size > _bufsz - _count) {
        _bufsz += max_size - (_bufsz - _count);
        byte_t * tmp = static_cast<byte_t *> (std::realloc(_buffer, _bufsz));
        PFS_ASSERT(tmp);
        _buffer = tmp;
    }

    ssize_t r = _d.read(_buffer + _count, max_size);

    if (r > 0) {
        _count += r;
    }

    return r;
}

bool buffered_device::can_read (size_t count)
{
    if (_cursor + count < _count) {
        return true;
    }

    if (_cursor > 0 and _cursor == _count) {
        _cursor = 0;
    }

    ssize_t n = upload_bytes(count < 256 ? 256 : count);

    if (n < static_cast<ssize_t> (count)) {
        return false;
    }

    return true;
}

bool buffered_device::read_byte (byte_t & c)
{
    if (can_read(1)) {
        c = _buffer[_cursor++];
        return true;
    }
    return false;
}

bool buffered_device::peek_byte (byte_t & c)
{
    if (can_read(1)) {
        c = _buffer[_cursor];
        return true;
    }
    return false;
}

//void buffered_device::unread_byte (byte_t c)
//{
//	if (_cursor > 0) {
//		_buffer[--_cursor] = c;
//	} else {
//		_buffer.insert(_buffer.begin(), c);
//	}
//}

bool buffered_device::read_line (byte_string & line, size_t maxSize)
{
    size_t n = 0;
    byte_t c;

    while (n < maxSize && read_byte(c)) {
        ++n;
        line.append(1, c);

        if (c == byte_t('\n')) { // FIXME for Windows and MacOS
            break;
        }
    }

    return _d.errorcode() == error_code();
}

}} // pfs::io
