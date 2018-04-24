#pragma once
#include <pfs/byte_string.hpp>

namespace pfs {

struct byte_ostream
{
    typedef byte_string::value_type char_type;

    byte_ostream (byte_string & buffer, endian const & order = endian::native_order())
        : _buffer(buffer)
        , _o(order)
    {}


    template <typename Integral>
    void write_integral (Integral v)
    {
        Integral a = _o.convert(v);

        union
        {
            Integral v;
            byte_string::value_type b[sizeof (Integral)];
        } u;
        u.v = a;
        _buffer.append(byte_string(u.b, sizeof (Integral)));
    }

    template <typename Real>
    void write_real (Real v)
    {
#ifdef PFS_HAVE_INT64
        if (sizeof (Real) == 8) {

            union
            {
                uint64_t i;
                real64_t f;
            } u;
            u.f = v;
            return write_integral(u.i);
        } else
#endif
            if (sizeof(Real) == 4) {

            union
            {
                uint32_t i;
                real32_t f;
            } u;
            u.f = v;
            return write_integral(u.i);
        } else {

            union
            {
                Real v;
                byte_string::value_type b[sizeof(Real)];
            } d;

            if (_o != endian::native_order()) {
                byte_string::value_type b[sizeof(Real)];

                for (int i = 0, j = sizeof(Real) - 1; j >= 0; ++i, --j) {
                    b[i] = d.b[j];
                }

                _buffer.append(byte_string(b, sizeof(Real)));
            } else {
                _buffer.append(byte_string(d.b, sizeof(Real)));
            }
        }
    }

    void write (byte_string const & v, byte_string::size_type n = byte_string::npos)
    {
        _buffer.append(v, 0, n);
    }

    void write (byte_string::const_pointer v, byte_string::size_type n = byte_string::npos)
    {
        _buffer.append(v, n);
    }

    void write (char const * v, byte_string::size_type n = byte_string::npos)
    {
        _buffer.append(v, n);
    }

private:
    byte_string & _buffer;
    endian _o;
};

inline std::ostream & operator << (std::ostream & os, byte_string const & v)
{
    os << v.c_str();
    return os;
}

template <int N>
inline byte_ostream & operator << (byte_ostream & os, byte_string_ref_n<N> const & v)
{
    typename byte_string_ref_n<N>::size_type n = pfs::min(v.max_size()
            , static_cast<typename byte_string_ref_n<N>::size_type> (v.p->size()));
    os.write_integral(n);
    os.write(*v.p, n);
    return os;
}

inline byte_ostream & operator << (byte_ostream & os, byte_string_ref const & v)
{
    os.write(*v.p, v.max_size);
    return os;
}

inline byte_ostream & operator << (byte_ostream & os, byte_string const & v)
{
    os.write(v);
    return os;
}

inline byte_ostream & operator << (byte_ostream & os, bool v)
{
    os.write_integral(v);
    return os;
}

inline byte_ostream & operator << (byte_ostream & os, char v)
{
    os.write_integral(v);
    return os;
}

inline byte_ostream & operator << (byte_ostream & os, signed char v)
{
    os.write_integral(v);
    return os;
}

inline byte_ostream & operator << (byte_ostream & os, unsigned char v)
{
    os.write_integral(v);
    return os;
}

inline byte_ostream & operator << (byte_ostream & os, wchar_t v)
{
    os.write_integral(v);
    return os;
}

inline byte_ostream & operator << (byte_ostream & os, short v)
{
    os.write_integral(v);
    return os;
}

inline byte_ostream & operator << (byte_ostream & os, unsigned short v)
{
    os.write_integral(v);
    return os;
}

inline byte_ostream & operator << (byte_ostream & os, int v)
{
    os.write_integral(v);
    return os;
}

inline byte_ostream & operator << (byte_ostream & os, unsigned int v)
{
    os.write_integral(v);
    return os;
}

inline byte_ostream & operator << (byte_ostream & os, long v)
{
    os.write_integral(v);
    return os;
}

inline byte_ostream & operator << (byte_ostream & os, unsigned long v)
{
    os.write_integral(v);
    return os;
}

#if PFS_HAVE_LONGLONG

inline byte_ostream & operator << (byte_ostream & os, long long v)
{
    os.write_integral(v);
    return os;
}

inline byte_ostream & operator << (byte_ostream & os, unsigned long long v)
{
    os.write_integral(v);
    return os;
}

#endif

inline byte_ostream & operator << (byte_ostream & os, float v)
{
    os.write_real(v);
    return os;
}

inline byte_ostream & operator << (byte_ostream & os, double v)
{
    os.write_real(v);
    return os;
}

} // pfs
