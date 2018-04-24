#pragma once
#include <pfs/type_traits.hpp>
#include <pfs/endian.hpp>

namespace pfs {

template <typename Device>
class binary_istream
{
public:
    binary_istream (Device & dev, endian order = endian::network_order())
        : _dev(dev)
        , _order(order)
    {}

    template <typename T>
    inline typename enable_if<is_integral<T>::value, binary_istream &>::type
    operator >> (T & v)
    {
        ssize_t r = read_integral<T>(v);
        return *this;
    }

    template <typename T>
    inline typename enable_if<is_floating_point<T>::value, binary_istream &>::type
    operator >> (T & v)
    {
        ssize_t r = read_float<T>(v);
        return *this;
    }

    endian const & order () const
    {
        return _order;
    }

protected:
     template <typename Integral>
     ssize_t read_integral (Integral & v)
    {
        union u {
            Integral v;
            char b[sizeof(Integral)];
        } d;

        ssize_t result = _dev.read(d.b, sizeof(Integral));

        if (result < 0)
            return result;

        v = _order.convert(d.v);

        return result;
    }

    template <typename Float>
    ssize_t read_float (Float & v)
    {
        ssize_t result = -1;

#ifdef PFS_HAVE_INT64
        if (sizeof(Float) == 8) {
            uint64_t d = 0;
            result = read_integral(d);
            v = *reinterpret_cast<Float *>(& d);
            return result;
        } else
#endif
        if (sizeof(Float) == 4) {
            uint32_t d = 0;
            result = read_integral(d);
            v = *reinterpret_cast<Float *>(& d);
            return result;
        } else if (sizeof(Float) == 2) {
            uint16_t d = 0;
            result = read_integral(d);
            v = *reinterpret_cast<Float *>(& d);
            return result;
        } else {
            PFS_ASSERT_X(false, "unsupported floating point (too big to fit in integer)");
        }

        return result;
    }

private:
    Device & _dev;
    endian   _order;
};

} //pfs

