#pragma once
#include <pfs/v2/byte_string.hpp>
#include <pfs/v2/binary_ostream.hpp>

namespace pfs {

// template <>
// //        , typename OStreamTag>
// struct binary_ostream<byte_string, >;
    
struct byte_ostream
{
    //typedef byte_string::value_type char_type;
    typedef byte_string::const_pointer const_pointer;

    byte_ostream (byte_string & buffer, endian const & order = endian::native_order())
        : _buffer(buffer)
        , _o(order)
    {}

    endian const & order () const
    {
        return _o;
    }

    byte_string const & data () const
    {
        return _buffer;
    }
    
    ssize_t write (const_pointer p, size_t n)
    {
        _buffer.append(p, n);
    }


    template <typename T>
    void write (T const & v)
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

inline byte_ostream & operator << (byte_ostream & os, buffer_wrapper<byte_string::value_type const> const & v)
{
    os.write(v.p, v.max_size);
    return os;
}

inline byte_ostream & operator << (byte_ostream & os, buffer_wrapper<char const> const & v)
{
    os.write(v.p, v.max_size);
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
