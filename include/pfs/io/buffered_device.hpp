#pragma once
#include <cstdlib>
#include <pfs/cxxlang.hpp>
#include <pfs/io/exception.hpp>
#include <pfs/io/device.hpp>

namespace pfs {
namespace io {

template <typename DevicePtr>
class buffered_device
{
    //
    // Return: > 0 if there is `count` bytes available;
    //           0 if there is no requested bytes available;
    //         < 0 if there is an error occured.
    //
    ssize_t ensure_available (size_t count, error_code & ec)
    {
        if (count <= _count)
            return integral_cast_check<ssize_t>(count);

        ssize_t n = cache_bytes(count < 256 ? count * 2 : count, ec);

        // Error
        if (n < 0)
            return -1;

        // Enough bytes available
        if (count <= _count)
            return integral_cast_check<ssize_t>(count);

        return integral_cast_check<ssize_t>(_count);
    }

    ssize_t cache_bytes (size_t max_size, error_code & ec)
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

        ssize_t r = _d->read(_buffer + (_pos + _count), max_size, ec);

        if (r > 0) {
            _count += r;
        }

        return r;
    }

public:
    buffered_device (DevicePtr d, size_t initial_size = 256)
        : _d (d)
        , _pos(0)
        , _count(0)
    {
        _buffer = static_cast<byte_t *>(std::malloc(initial_size * sizeof(byte_t)));
        _bufsz = initial_size;
    }


    ~buffered_device ()
    {
        if (_buffer)
            std::free(_buffer);
    }


    ssize_t available () const
    {
        return _count + _d->available();
    }

    bool at_end () const
    {
        return available() == ssize_t(0);
    }

   /**
    * @brief Read at least @a n bytes from the device.
    */
    ssize_t read (byte_t * bytes, size_t n, error_code & ec) pfs_noexcept
    {
        ssize_t result = ensure_available(n, ec);

        if (result <= 0)
            return result;

        std::memcpy(bytes, _buffer + _pos, integral_cast_check<size_t>(result));
        _pos += integral_cast_check<size_t>(result);
        _count -= integral_cast_check<size_t>(result);
        return result;
    }


   /**
    * @brief Read at least @a n chars from the device.
    */
    ssize_t read (char * chars, size_t n, error_code & ec) pfs_noexcept
    {
        return this->read(reinterpret_cast<byte_t *>(chars), n, ec);
    }

    /**
     * @throw pfs::io_exception
     */
    ssize_t read (byte_t * bytes, size_t n)
    {
        error_code ec;
        ssize_t r = this->read(bytes, n, ec);
        if (r < 0)
            throw io_exception(ec);
        return r;
    }

    /**
     * @throw pfs::io_exception
     */
    ssize_t read (char * chars, size_t n)
    {
        return this->read(reinterpret_cast<byte_t *>(chars), n);
    }

    /**
     * @brief Read at least @a n bytes from device and appends them to @a bytes.
     */
    ssize_t read (byte_string & bytes, size_t n, error_code & ec) pfs_noexcept
    {
        size_t initial_size = bytes.size();
        bytes.resize(initial_size + n);
        ssize_t result = this->read(bytes.data() + initial_size, n, ec);

        if (result > 0)
            bytes.resize(initial_size + integral_cast_check<size_t>(result));

        return result;
    }


    /**
     * @brief Read data from device and appends them to @a bytes.
     */
    ssize_t read (byte_string & bytes, error_code & ec) pfs_noexcept
    {
        return this->read(bytes, available(), ec);
    }

    /**
     * @throw pfs::io_exception
     */
    ssize_t read (byte_string & bytes, size_t n)
    {
        error_code ec;
        ssize_t r = this->read(bytes, n, ec);
        if (r < 0)
            throw io_exception(ec);
        return r;
    }

    /**
     * @throw pfs::io_exception
     */
    ssize_t read (byte_string & bytes)
    {
        return this->read(bytes, available());
    }

    /**
     * @brief Reads byte from the buffered device.
     * @details Reads one character from the device and stores it in @a c.
     *
     * @param c Reference to byte to store value read from device.
     * @param ex Reference to store error code.
     * @return @c true if byte read successfully,
     *         or @c false if error occurred or no more bytes available (i.e. at end of device).
     */
    bool read_byte (byte_t & c, error_code & ec) pfs_noexcept
    {
        if (ensure_available(1, ec) > 0)
            c = _buffer[_pos++];
        return !is_error(ec);
    }

    bool read_byte (byte_t & c)
    {
        error_code ec;
        bool r = this->read_byte(c, ec);
        if (is_error(ec))
            throw io_exception(ec);
        return r;
    }

    bool peek_byte (byte_t & c, error_code & ec) pfs_noexcept
    {
        if (ensure_available(1, ec) > 0)
            c = _buffer[_pos];
        return !is_error(ec);
    }

    bool peek_byte (byte_t & c)
    {
        error_code ec;
        bool r = this->peek_byte(c, ec);
        if (is_error(ec))
            throw io_exception(ec);
        return r;
    }

    ssize_t read_line (byte_string & line, size_t max_size, error_code & ec) pfs_noexcept
    {
        ssize_t n = 0;
        byte_t c;

        while (n < max_size && read_byte(c, ec)) {
            ++n;
            line.append(1, c);

            if (c == byte_t('\n')) { // FIXME for Windows and MacOS
                break;
            }
        }

        return is_error(ec) ? -1 : n;
    }

    ssize_t read_line (byte_string & line, size_t max_size)
    {
        error_code ec;
        ssize_t r = this->read_line(line, max_size, ec);
        if (r < 0)
            throw io_exception(ec);
        return r;
    }

        /**
     * @brief Write bytes to the device.
     */
    ssize_t write (const byte_t * bytes, size_t n, error_code & ec) pfs_noexcept
    {
        return _d->write(bytes, n, ec);
    }

    ssize_t write (const char * chars, size_t n, error_code & ec) pfs_noexcept
    {
        return this->write(reinterpret_cast<const byte_t *>(chars), n, ec);
    }

    ssize_t write (const byte_t * bytes, size_t n)
    {
        error_code ec;
        ssize_t r = write(bytes, n, ec);
        if (r < 0)
            throw io_exception(ec);
        return r;
    }

    ssize_t write (const char * chars, size_t n)
    {
        return this->write(reinterpret_cast<const byte_t *>(chars), n);
    }

    ssize_t write (byte_string const & bytes, size_t n, error_code & ec) pfs_noexcept
    {
        return this->write(bytes.data(), pfs::min(n, bytes.size()), ec);
    }

    ssize_t write (byte_string const & bytes, error_code & ec) pfs_noexcept
    {
        return this->write(bytes.data(), bytes.size(), ec);
    }

    ssize_t write (byte_string const & bytes, size_t n)
    {
        error_code ec;
        ssize_t r = write(bytes, n, ec);
        if (r < 0)
            throw io_exception(ec);
        return r;
    }

    ssize_t write (byte_string const & bytes)
    {
        return this->write(bytes.data(), bytes.size());
    }

private:
    DevicePtr _d;
    byte_t *  _buffer;
    size_t    _bufsz;
    size_t    _pos;
    size_t    _count;
};

}} // pfs::io
