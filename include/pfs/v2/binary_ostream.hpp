#pragma once
#include <pfs/endian.hpp>

namespace pfs {

// template <typename Device
//         , typename OStreamTag>
// struct binary_ostream;
//     
// template <typename Device
//         , typename OStreamTag
//         , typename T>
// binary_ostream<Device, OStreamTag> & operator << (binary_ostream<Device, OStreamTag> &, T const &);

template <typename Device, typename T>
ssize_t write_binary (Device & dev, endian order, T const & v);

namespace details {
namespace integral {

template <typename Device, typename Integral>
ssize_t write_binary (Device & dev, endian order, Integral const & v)
{
	Integral a = order.convert(v);
	union u { Integral v; char b[sizeof(Integral)]; } d;
	d.v = a;
	return dev.write(d.b, sizeof(Integral));
}

}} // integral

namespace details {
namespace fp {

template <typename Device, typename Float>
ssize_t write_binary (Device & dev, endian order, Float const & v)
{
#ifdef PFS_HAVE_INT64
    if (sizeof(Float) == 8) {
        return details::integral::write_binary(dev, order, *reinterpret_cast<uint64_t const *>(& v));
    } else
#endif
    if (sizeof(Float) == 4) {
        return details::integral::write_binary(dev, order, *reinterpret_cast<uint32_t const *>(& v));
    } else if (sizeof(Float) == 2) {
        return details::integral::write_binary(dev, order, *reinterpret_cast<uint16_t const *>(& v));
    } else {
        union { Float v; char b[sizeof(Float)]; } d;

        if (order != endian::native_order()) {
            char b[sizeof(Float)];

            for (int i = 0, j = sizeof(Float) - 1; j >= 0; ++i, --j) {
                b[i] = d.b[j];
            }

            return dev.write(b, sizeof(Float));
        } else {
            return dev.write(d.b, sizeof(Float));
        }
    }

    return ssize_t(-1);
}

}} // details::fp

namespace details {
namespace sequence {

template <typename Device>
ssize_t write_binary (Device & dev, endian order, char const * data, size_t n)
{
    ssize_t result = write_binary(dev, order, n);

    if (result > 0) { // pack size of byte_string
        ssize_t r1 = dev.write(data, n);

        if (r1 >= 0)
            result += r1;
        else
            result = r1;
    }

    return result;
}

}} // details::sequence

template <typename Device>
inline ssize_t write_binary (Device & dev, endian order, bool const & v)
{
    return details::integral::write_binary(dev, order, v);
}

template <typename Device>
inline ssize_t write_binary (Device & dev, endian order, char const & v)
{
    return details::integral::write_binary(dev, order, v);
}

template <typename Device>
inline ssize_t write_binary (Device & dev, endian order, signed char const & v)
{
    return details::integral::write_binary(dev, order, v);
}

template <typename Device>
inline ssize_t write_binary (Device & dev, endian order, unsigned char const & v)
{
    return details::integral::write_binary(dev, order, v);
}

template <typename Device>
inline ssize_t write_binary (Device & dev, endian order, wchar_t const & v)
{
    return details::integral::write_binary(dev, order, v);
}

template <typename Device>
inline ssize_t write_binary (Device & dev, endian order, short const & v)
{
    return details::integral::write_binary(dev, order, v);
}

template <typename Device>
inline ssize_t write_binary (Device & dev, endian order, unsigned short const & v)
{
    return details::integral::write_binary(dev, order, v);
}

template <typename Device>
inline ssize_t write_binary (Device & dev, endian order, int const & v)
{
    return details::integral::write_binary(dev, order, v);
}

template <typename Device>
inline ssize_t write_binary (Device & dev, endian order, unsigned int const & v)
{
    return details::integral::write_binary(dev, order, v);
}

template <typename Device>
inline ssize_t write_binary (Device & dev, endian order, long const & v)
{
    return details::integral::write_binary(dev, order, v);
}

template <typename Device>
inline ssize_t write_binary (Device & dev, endian order, unsigned long const & v)
{
    return details::integral::write_binary(dev, order, v);
}

#if PFS_HAVE_LONGLONG

template <typename Device>
inline ssize_t write_binary (Device & dev, endian order, long long const & v)
{
    return details::integral::write_binary(dev, order, v);
}

template <typename Device>
inline ssize_t write_binary (Device & dev, endian order, unsigned long long const & v)
{
    return details::integral::write_binary(dev, order, v);
}

#endif

template <typename Device>
inline ssize_t write_binary (Device & dev, endian order, float const & v)
{
//    //return details::pack_ieee754(appender, real64_t(v), order, 32, 8);
    return details::fp::write_binary(dev, order, v);
}

template <typename Device>
inline ssize_t write_binary (Device & dev, endian order, double const & v)
{
//    //return details::pack_ieee754(appender, real64_t(v), order, 64, 11);
    return details::fp::write_binary(dev, order, v);
}

#if PFS_HAVE_LONG_DOUBLE

template <typename Device>
inline ssize_t write_binary (Device & dev, endian order, long double const & v)
{
    return details::fp::write_binary(dev, order, v);
}

#endif

} //pfs

