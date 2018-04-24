#pragma once
#include <pfs/byte_string.hpp>
#include <pfs/binary_istream.hpp>
#include <pfs/iterator.hpp>
#include <pfs/algorithm.hpp>
#include <pfs/limits.hpp>

namespace pfs {

class byte_string_istream : public binary_istream<byte_string_istream>
{
    typedef binary_istream<byte_string_istream> base_class;

public:
    byte_string_istream (byte_string & buffer, endian order = endian::network_order())
        : base_class(*this, order)
        , _first(buffer.cbegin())
        , _last(buffer.cend())
    {}

    byte_string_istream (byte_string::const_iterator first
            , byte_string::const_iterator last
            , endian order = endian::network_order())
        : base_class(*this, order)
        , _first(first)
        , _last(last)
    {}

    ssize_t read (char * s, size_t n)
    {
        n = pfs::min(pfs::distance(_first, _last), integral_cast_check<ssize_t>(n));
        pfs::copy(_first, _first + n, s);
        _first += n;
        return static_cast<ssize_t>(n);
    }

    byte_string::difference_type available () const
    {
        return pfs::distance(_first, _last);
    }


private:
    byte_string::const_iterator _first;
    byte_string::const_iterator _last;
};

inline byte_string_istream & operator >> (byte_string_istream & is, buffer_wrapper<byte_string::value_type> const & v)
{
    is.read(reinterpret_cast<char *>(v.p), v.max_size);
    return is;
}

inline byte_string_istream & operator >> (byte_string_istream & is, buffer_wrapper<char> const & v)
{
    is.read(v.p, v.max_size);
    return is;
}

} // pfs
