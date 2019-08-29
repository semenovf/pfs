////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2014-2019 Vladislav Trifochkin
//
// This file is part of [pfs](https://github.com/semenovf/pfs) library.
//
// Changelog:
////////////////////////////////////////////////////////////////////////////////
#pragma once
#include <pfs/cxx11required.hpp>
#include <algorithm>
#include <chrono>
#include <cassert>
#include <cerrno>
#include <cstdlib>
#include <cstring>
#include <utility>

#if HAVE_WINSOCK2_H
// TODO Implement for Windows
#else
#   include <unistd.h>
#   include <fcntl.h>
// #   include <sys/types.h>
// #   include <sys/socket.h>
#endif

namespace pfs {
namespace io {

/**
 * Return @c true if file descriptor @a fd is in non-blocking mode,
 * @c false otherwise.
 */
inline
bool is_nonblocking (int fd)
{
#if HAVE_WINSOCK2_H
#   error Unimplemeted yet
#else
    int r = fcntl(fd, F_GETFL, 0);
    assert(r >= 0);
    return (r & O_NONBLOCK);
#endif
}

/**
 * Enable / disable file descriptor @a fd to non-blocking / blocking mode.
 */
inline
bool set_nonblocking (int fd, bool on)
{
#if HAVE_WINSOCK2_H
#   error Unimplemented yet
    //u_long param = enable;
    //return ioctlsocket(socket, FIONBIO, & param);
#else
    int flags = ::fcntl(fd, F_GETFL, 0);

    if (on)
        flags |= O_NONBLOCK;
    else
        flags &= ~O_NONBLOCK;

    return ::fcntl(fd, F_SETFL, flags) >= 0;
#endif
}

/**
 * Receive a message from file descriptor until @a len or timeout in @a millis
 * milliseconds exceeded or error occured.
 *
 * @return Result as pair, where @c first is a total bytes read and @c second
 * is result of last operation :
 *      @arg  < 0 Error occured (negative @c errno value),
 *      @arg == 0 On end-of-file (for stream sockets, when it's peer has
 *      performed an orderly shutdown
 *      @arg  > 0 Number of bytes read by last call
 */

template <typename ReaderFunc, typename CharT = char>
class buffered_input
{
    static const int CHUNK_SIZE = 256;

public:
    static_assert(std::is_trivial<CharT>() && sizeof(CharT) == 1
            , "A buffered_input can only hold one-byte-sized trivial chars");

    using size_type = size_t;
    using ssize_type = ssize_t;
    using value_type = CharT;
    using pointer = CharT *;
    using const_pointer = CharT const *;
    using reference = CharT &;
    using const_reference = CharT const &;
    using iterator = pointer;
    using const_iterator = const_pointer;

private:
    int       _fd = -1;
    pointer   _pbuf = nullptr;
    size_type _capacity = 0;
    size_type _head = 0;
    size_type _count = 0;
    ReaderFunc _reader;

private:
    ssize_type cache_data (size_t max_size, int millis = 0)
    {
        using clock = std::chrono::steady_clock;

        ssize_type total = 0;
        size_t chunk_size = max_size;
        auto start = clock::now();

        if (_count == 0)
            _head = 0;

        if (max_size == 0)
            return 0;

        if (_capacity == 0) {
            _pbuf = static_cast<pointer>(std::malloc(max_size * sizeof(value_type)));
            assert(_pbuf);
            _capacity = max_size;
        } else if (max_size > _capacity - _count) {
            _capacity += max_size - (_capacity - _count);
            pointer tmp = static_cast<pointer>(std::realloc(_pbuf, _capacity));
            assert(tmp);
            _pbuf = tmp;
        }

        if (max_size > _capacity - (_head + _count)) {
            std::memmove(_pbuf, _pbuf + _head, _count);
            _head = 0;
        }

        int ntries = 5;
        ssize_type n = 0;

        do {
            errno = 0;

            if (max_size - total < chunk_size)
                chunk_size = max_size - total;

            n = _reader(_fd, _pbuf + (_head + _count), chunk_size, 0);

            if (n < 0) {
                if (errno == EAGAIN
#if defined(EWOULDBLOCK) && EWOULDBLOCK != EAGAIN
                        || e == EWOULDBLOCK
#endif
                        || errno == EINTR) {

                    if (ntries--)
                        continue;
                }

                if (total == 0)
                    total = n;

                break;
            }

            if (n > 0) {
                _count += n;
                total += n;

                if (millis >= 0)
                    start = clock::now();
            } else {
                if (millis >= 0) {
                    auto end = clock::now();
                    auto diff = std::chrono::duration_cast<
                            std::chrono::milliseconds>(end - start).count();

                    if (diff >= millis) {
                        break;
                    }
                }
            }

        } while (total < max_size);

        return total;
    }

public:
    buffered_input ()
        : _fd{-1}
    {}

    buffered_input (int fd)
        : _fd{fd}
    {}

    ~buffered_input ()
    {
        if (_pbuf)
            std::free(_pbuf);
    }

    iterator begin () { return _pbuf + _head; }
    iterator end () { return _pbuf + _head + _count; }
    const_iterator begin () const { return _pbuf + _head; }
    const_iterator end () const { return _pbuf + _head + _count; }
    const_iterator cbegin () const { return _pbuf + _head; }
    const_iterator cend () const { return _pbuf + _head + _count; }

    size_type size () const
    {
        return _count;
    }

   /**
    * @brief Read at least @a n bytes from socket descriptor.
    */
    ssize_t read (CharT * bytes, size_t n) noexcept
    {
        if (n == 0)
            return 0;

        if (_count < n) {
            auto n1 = cache_data(n);

            if (n1 <= 0 && _count == 0)
                return n1;
        }

        n = std::min(n, _count);
        std::memcpy(bytes, _pbuf + _head, n);
        _head += n;
        _count -= n;
        return n;
    }

    /**
     * Read single value
     */
    std::pair<int, value_type> read () noexcept
    {
        if (_count == 0) {
            auto n = cache_data(CHUNK_SIZE);

            if (n <= 0)
                return std::make_pair(n, value_type{0});
        }

        auto ch = _pbuf[_head++];
        --_count;
        return std::make_pair(1, ch);
    }

    /**
     * Read line and stores data in @a out.
     *
     * If line read successfully, end-of-line symbol '\n' will be written
     * at the end.
     */
    template <typename OutputIterator>
    ssize_t read_line (OutputIterator out
            , size_t max_size = std::numeric_limits<size_t>::max()) noexcept
    {
        ssize_t total_written = 0;

        // Check if buffer contains end-of-line
        bool found = false;

        do {
            auto p = _pbuf + _head;
            auto last = _pbuf + _head + _count;

            if (p == last && _count < max_size) {
                auto n = cache_data(max_size > CHUNK_SIZE ? CHUNK_SIZE : max_size);

                if (n < 0)
                    return n;

                if (n == 0)
                    return total_written;

                p = _pbuf + _head;
                last = _pbuf + _head + _count;
            }

            while (!found && p != last && max_size--) {
                if (*p == '\r') {
                    ++p;
                    _head++;
                    _count--;

                    // Found Windows and MacOS end-of-line
                    if (p != last && *p == '\n') {
                        *out++ = *p;
                        found = true;
                    }
                } else {
                    // Found *nix end-of-line
                    if (*p == '\n')
                        found = true;

                    *out++ = *p;
                }

                ++p;
                _head++;
                _count--;
                total_written++;
            }
        } while (!found);

        return total_written;
    }
};

}} // namespace pfs::io
