#pragma once
#include <pfs/byte_string.hpp>
#include <pfs/binary_ostream.hpp>

namespace pfs {

class byte_string_ostream : public binary_ostream<byte_string_ostream>
{
    typedef binary_ostream<byte_string_ostream> base_class;

public:
    byte_string_ostream (byte_string & buffer, endian order = endian::network_order())
        : base_class(*this, order)
        , _buffer(buffer)
    {}

    inline ssize_t write (char const * s, size_t n)
    {
        _buffer.append(s, n);
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
