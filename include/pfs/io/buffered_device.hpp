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
    size_t _count;
    size_t _cursor;

private:
    bool can_read (size_t count);

    ssize_t upload_bytes (size_t max_size);

public:
    buffered_device (device & d, size_t initialSize = 256);

    ~buffered_device ();

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

    //void unread_byte (byte_t c);

    bool read_line (byte_string & line, size_t maxSize);
};

}} // pfs::io
