#pragma once
#include <pfs/type_traits.hpp>
#include <pfs/endian.hpp>
#include <pfs/byte_string.hpp>

namespace pfs {

template <typename DevicePtr>
class binary_istream
{
public:
    binary_istream (DevicePtr dev, int millis = -1, endian order = endian::network_order())
        : _dev(dev)
        , _timeout(millis)
        , _order(order)
    {}

    template <typename T>
    inline typename enable_if<is_integral<T>::value, binary_istream &>::type
    operator >> (T & v)
    {
        read_integral<T>(v, _timeout);
        return *this;
    }

    template <typename T>
    inline typename enable_if<is_floating_point<T>::value, binary_istream &>::type
    operator >> (T & v)
    {
        ssize_t r = read_float<T>(v, _timeout);
        return *this;
    }

    endian const & order () const
    {
        return _order;
    }

    binary_istream & operator >> (buffer_wrapper<byte_string::value_type> const & v)
    {
        _dev->read_wait(reinterpret_cast<char *>(v.p), v.max_size, _timeout);
        return *this;
    }

    binary_istream & operator >> (buffer_wrapper<char> const & v)
    {
        _dev->read_wait(v.p, v.max_size, _timeout);
        return *this;
    }

    template <int N>
    binary_istream & operator >> (byte_string_ref_n<N> const & v)
    {
        typename byte_string_ref_n<N>::size_type sz = 0;
        *this >> sz;
        _dev->read_wait(*v.p, sz, _timeout);
        return *this;
    }

    binary_istream & operator >> (byte_string_ref const & v)
    {
        _dev->read_wait(*v.p, v.max_size, _timeout);
        return *this;
    }

protected:
     template <typename Integral>
     ssize_t read_integral (Integral & v, int millis)
    {
        union u {
            Integral v;
            char b[sizeof(Integral)];
        } d;

        ssize_t result = _dev->read_wait(d.b, sizeof(Integral), millis);

        if (result > 0)
            v = _order.convert(d.v);

        return result;
    }

    template <typename Float>
    ssize_t read_float (Float & v, int millis)
    {
        ssize_t result = -1;

#ifdef PFS_HAVE_INT64
        if (sizeof(Float) == 8) {
            uint64_t d = 0;
            result = read_integral(d, millis);

            if (result > 0)
                v = *reinterpret_cast<Float *>(& d);
            return result;
        } else
#endif
        if (sizeof(Float) == 4) {
            uint32_t d = 0;
            result = read_integral(d, millis);

            if (result > 0)
                v = *reinterpret_cast<Float *>(& d);
            return result;
        } else if (sizeof(Float) == 2) {
            uint16_t d = 0;
            result = read_integral(d, millis);

            if (result > 0)
                v = *reinterpret_cast<Float *>(& d);
            return result;
        } else {
            PFS_ASSERT_X(false, "unsupported floating point (too big to fit in integer)");
        }

        return result;
    }

private:
    DevicePtr _dev;
    int       _timeout;
    endian    _order;
};

} //pfs

