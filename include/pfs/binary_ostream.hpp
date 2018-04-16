#pragma once
#include <pfs/type_traits.hpp>
#include <pfs/endian.hpp>

namespace pfs {

template <typename Device>
class binary_ostream
{
public:
    binary_ostream (Device & dev, endian order = endian::network_order())
        : _dev(dev)
        , _order(order)
    {}

    template <typename T>
    inline typename enable_if<is_integral<T>::value, binary_ostream &>::type
    operator << (T const & v)
    {
        ssize_t r = write_integral<T>(v);
        return *this;
    }

    template <typename T>
    inline typename enable_if<is_floating_point<T>::value, binary_ostream &>::type
    operator << (T const & v)
    {
        ssize_t r = write_float<T>(v);
        return *this;
    }

protected:
    template <typename Integral>
    ssize_t write_integral (Integral const & v)
    {
        Integral a = _order.convert(v);
        union u { Integral v; char b[sizeof(Integral)]; } d;
        d.v = a;
        return _dev.write(d.b, sizeof(Integral));
    }

    template <typename Float>
    ssize_t write_float (Float const & v)
    {
#ifdef PFS_HAVE_INT64
        if (sizeof(Float) == 8) {
            return write_integral(*reinterpret_cast<uint64_t const *>(& v));
        } else
#endif
        if (sizeof(Float) == 4) {
            return write_integral(*reinterpret_cast<uint32_t const *>(& v));
        } else if (sizeof(Float) == 2) {
            return write_integral(*reinterpret_cast<uint16_t const *>(& v));
        } else {
            return write_float<double>(static_cast<double>(v));
        }

        return ssize_t(-1);
    }

private:
    Device & _dev;
    endian   _order;
};

} //pfs

