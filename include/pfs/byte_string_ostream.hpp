#pragma once
#include <pfs/byte_string.hpp>
#include <pfs/binary_ostream.hpp>

namespace pfs {

class byte_string_ostream : public binary_ostream<byte_string_ostream *>
{
    typedef binary_ostream<byte_string_ostream *> base_class;

public:
    byte_string_ostream (byte_string & buffer, endian order = endian::network_order())
        : base_class(this, order)
        , _buffer(buffer)
    {}

    inline ssize_t write (char const * s, size_t n)
    {
        _buffer.append(s, n);
    }

    template <typename T>
    inline typename enable_if<is_integral<T>::value, byte_string_ostream &>::type
    operator << (T const & v)
    {
        base_class::operator << <T> (v);
        return *this;
    }

    template <typename T>
    inline typename enable_if<is_floating_point<T>::value, byte_string_ostream &>::type
    operator << (T const & v)
    {
        base_class::operator << <T> (v);
        return *this;
    }

    byte_string_ostream & operator << (char const * s)
    {
        base_class::operator << (s);
        return *this;
    }

    byte_string_ostream & operator << (std::string const & s)
    {
        base_class::operator << (s);
        return *this;
    }

    byte_string_ostream & operator << (byte_string const & s)
    {
        base_class::operator << (s);
        return *this;
    }

    endian const & order () const
    {
        return base_class::order();
    }

    byte_string const & data () const
    {
        return _buffer;
    }

private:
    byte_string & _buffer;
};

inline byte_string_ostream & operator << (byte_string_ostream & os, buffer_wrapper<byte_string::value_type const> const & v)
{
    os.write(reinterpret_cast<char const *>(v.p), v.max_size);
    return os;
}

inline byte_string_ostream & operator << (byte_string_ostream & os, buffer_wrapper<char const> const & v)
{
    os.write(v.p, v.max_size);
    return os;
}

} // pfs
