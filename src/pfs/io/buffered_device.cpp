#include <cstdlib>
#include <pfs/io/buffered_device.hpp>

namespace pfs {
namespace io {

buffered_device::buffered_device (device_ptr d, size_t initial_size)
    : _d (d)
    , _pos(0)
    , _count(0)
{
    _buffer = static_cast<byte_t *>(std::malloc(initial_size * sizeof(byte_t)));
    _bufsz = initial_size;
}

buffered_device::~buffered_device ()
{
    if (_buffer)
        std::free(_buffer);
}

ssize_t buffered_device::cache_bytes (size_t max_size)
{
    if (max_size == 0)
        return 0;

    if (max_size > _bufsz - _count) {
        _bufsz += max_size - (_bufsz - _count);
        byte_t * tmp = static_cast<byte_t *> (std::realloc(_buffer, _bufsz));
        PFS_ASSERT(tmp);
        _buffer = tmp;
    }

    if (max_size > _bufsz - (_pos + _count)) {
        std::memmove(_buffer, _buffer + _pos, _count);
        _pos = 0;
    }

    ssize_t r = _d->read(_buffer + (_pos + _count), max_size);

    if (r > 0) {
        _count += r;
    }

    return r;
}

//
// Return: > 0 if there is `count` bytes available;
//           0 if there is no requested bytes available;
//         < 0 if there is an error occured.
//
ssize_t buffered_device::ensure_available (size_t count)
{
    if (count <= _count)
        return integral_cast_check<ssize_t>(count);

    ssize_t n = cache_bytes(count < 256 ? count * 2 : count);

    // Error
    if (n < 0)
        return -1;

    // Enough bytes available
    if (count <= _count)
        return integral_cast_check<ssize_t>(count);

    return integral_cast_check<ssize_t>(_count);
}

ssize_t buffered_device::read (byte_t * bytes, size_t n)
{
    ssize_t result = ensure_available(n);

    if (result <= 0)
        return result;

    std::memcpy(bytes, _buffer + _pos, integral_cast_check<size_t>(result));
    _pos += integral_cast_check<size_t>(result);
    _count -= integral_cast_check<size_t>(result);
    return result;
}

bool buffered_device::read (byte_string & bytes, size_t n)
{
    size_t initial_size = bytes.size();
    bytes.resize(initial_size + n);
    ssize_t result = read(bytes.data() + initial_size, n);

    if (result < 0)
        return false;

    bytes.resize(initial_size + integral_cast_check<size_t>(result));
    return true;
}

bool buffered_device::read_byte (byte_t & c)
{
    if (ensure_available(1) > 0) {
        c = _buffer[_pos++];
        return true;
    }
    return false;
}

bool buffered_device::peek_byte (byte_t & c)
{
    if (ensure_available(1) > 0) {
        c = _buffer[_pos];
        return true;
    }
    return false;
}

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

    return _d->errorcode() == error_code();
}

}} // pfs::io
