#pragma once
#include <pfs/io/exception.hpp>
#include <pfs/io/device.hpp>

namespace pfs {
namespace io {

class buffered_device
{
    device & _d;
    byte_t * _buffer;
    size_t _bufsz;
    size_t _pos;
    size_t _count;

private:
    ssize_t ensure_available (size_t count);
    ssize_t cache_bytes (size_t max_size);

public:
    buffered_device (device & d, size_t initial_size = 256);

    ~buffered_device ();

    ssize_t available () const
    {
        return _count + _d.available();
    }

    bool at_end () const
    {
        return available() == ssize_t(0);
    }

   /**
    * @brief Read at least @a n bytes from the device.
    */
    ssize_t read (byte_t * bytes, size_t n);

   /**
    * @brief Read at least @a n chars from the device.
    */
    ssize_t read (char * chars, size_t n)
    {
        return read(reinterpret_cast<byte_t *>(chars), n);
    }

    /**
     * @brief Read data from device and appends them to @a bytes.
     */
    bool read (byte_string & bytes)
    {
        return read(bytes, available());
    }

    /**
     * @brief Read at least @a n bytes from device and appends them to @a bytes.
     */
    bool read (byte_string & bytes, size_t n);

    /**
     * @brief Reads byte from the buffered device.
     * @details Reads one character from the device and stores it in @a c.
     *
     * @param c Reference to byte to store value read from device.
     * @param ex Reference to store error code.
     * @return @c true if byte read successfully,
     *         or @c false if error occurred or no more bytes available (i.e. at end of device).
     */
    bool read_byte (byte_t & c);

    bool peek_byte (byte_t & c);

    bool read_line (byte_string & line, size_t maxSize);
};

}} // pfs::io
