#pragma once
#include <pfs/endian.hpp>

namespace pfs {

template <typename Device
        , typename IStreamTag>
struct binary_istream;

template <typename Device
        , typename IStreamTag
        , typename T>
binary_istream<Device, IStreamTag> & operator >> (binary_istream<Device, IStreamTag> &, T &);

template <typename Device, typename T>
ssize_t read_binary (Device & dev, endian order, T & v);

namespace details {
namespace integral {

template <typename Device, typename Integral>
ssize_t read_binary (Device & dev, endian order, Integral & v)
{
    union u
    {
        Integral v;
        char b[sizeof(Integral)];
    } d;

    ssize_t result = dev.read(d.b, sizeof(Integral));

    if (result < 0)
        return result;

    v = order.convert(d.v);

	return result;
}

}} // integral

namespace details {
namespace fp {

template <typename Device, typename Float>
ssize_t read_binary (Device & dev, endian order, Float & v)
{
    ssize_t result = -1;

#ifdef PFS_HAVE_INT64
    if (sizeof(Float) == 8) {
        uint64_t d = 0;
        result = details::integral::read_binary(dev, order, d);
        v = *reinterpret_cast<Float *>(& d);
        return result;
    } else
#endif
    if (sizeof(Float) == 4) {
        uint32_t d = 0;
        result = details::integral::read_binary(dev, order, d);
        v = *reinterpret_cast<Float *>(& d);
        return result;
    } else if (sizeof(Float) == 2) {
        uint16_t d = 0;
        result = details::integral::read_binary(dev, order, d);
        v = *reinterpret_cast<Float *>(& d);
        return result;
    } else {
        PFS_ASSERT_X(false, "unsupported floating point (too big to fit in integer)");
    }

	return result;
}

}} // details::fp

namespace details {
namespace sequence {

template <typename Device>
ssize_t read_binary (Device & dev, endian order, char * buffer, size_t n)
{
    return dev.read(buffer, n);
}

// TODO Reimplement using Allocator as template parameter
//
template <typename Device>
ssize_t read_binary (Device & dev, endian order, char ** buffer, size_t * n)
{
    size_t size = 0;
    ssize_t result = read_binary(dev, order, size);

    *buffer = 0;

    if (result > 0) {
        *n = size;

        *buffer = new char[size];

        ssize_t r1 = dev.read(*buffer, size);

        if (r1 >= 0) {
            result += r1;
        } else {
            result = r1;
        }
    }

    return result;
}

}} // details::sequence

template <typename Device>
inline ssize_t read_binary (Device & dev, endian order, bool & v)
{
    return details::integral::read_binary(dev, order, v);
}

template <typename Device>
inline ssize_t read_binary (Device & dev, endian order, char & v)
{
    return details::integral::read_binary(dev, order, v);
}

template <typename Device>
inline ssize_t read_binary (Device & dev, endian order, signed char & v)
{
    return details::integral::read_binary(dev, order, v);
}

template <typename Device>
inline ssize_t read_binary (Device & dev, endian order, unsigned char & v)
{
    return details::integral::read_binary(dev, order, v);
}

template <typename Device>
inline ssize_t read_binary (Device & dev, endian order, wchar_t & v)
{
    return details::integral::read_binary(dev, order, v);
}

template <typename Device>
inline ssize_t read_binary (Device & dev, endian order, short & v)
{
    return details::integral::read_binary(dev, order, v);
}

template <typename Device>
inline ssize_t read_binary (Device & dev, endian order, unsigned short & v)
{
    return details::integral::read_binary(dev, order, v);
}

template <typename Device>
inline ssize_t read_binary (Device & dev, endian order, int & v)
{
    return details::integral::read_binary(dev, order, v);
}

template <typename Device>
inline ssize_t read_binary (Device & dev, endian order, unsigned int & v)
{
    return details::integral::read_binary(dev, order, v);
}

template <typename Device>
inline ssize_t read_binary (Device & dev, endian order, long & v)
{
    return details::integral::read_binary(dev, order, v);
}

template <typename Device>
inline ssize_t read_binary (Device & dev, endian order, unsigned long & v)
{
    return details::integral::read_binary(dev, order, v);
}

#if PFS_HAVE_LONGLONG

template <typename Device>
inline ssize_t read_binary (Device & dev, endian order, long long & v)
{
    return details::integral::read_binary(dev, order, v);
}

template <typename Device>
inline ssize_t read_binary (Device & dev, endian order, unsigned long long & v)
{
    return details::integral::read_binary(dev, order, v);
}

#endif

template <typename Device>
inline ssize_t read_binary (Device & dev, endian order, float & v)
{
    return details::fp::read_binary(dev, order, v);
}

template <typename Device>
inline ssize_t read_binary (Device & dev, endian order, double & v)
{
    return details::fp::read_binary(dev, order, v);
}

#if PFS_HAVE_LONG_DOUBLE

template <typename Device>
inline ssize_t read_binary (Device & dev, endian order, long double & v)
{
    return details::fp::read_binary(dev, order, v);
}

#endif

} //pfs

