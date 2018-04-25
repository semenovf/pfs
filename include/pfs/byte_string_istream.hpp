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

    /**
     * @brief Reset stream (reinitialize with new buffer)
     */
    void reset (byte_string & buffer)
    {
        _first = buffer.cbegin();
        _last = buffer.cend();
    }

    ssize_t read (char * s, size_t n)
    {
        n = pfs::min(pfs::distance(_first, _last), integral_cast_check<ssize_t>(n));
        pfs::copy(_first, _first + n, s);
        _first += n;
        return static_cast<ssize_t>(n);
    }

    ssize_t read (byte_string & v, byte_string::size_type n)
    {
        n = pfs::min(pfs::distance(_first, _last), integral_cast_check<ssize_t>(n));
        v.append(_first, _first + n);
        _first += n;
        return static_cast<ssize_t>(n);
    }

    byte_string::difference_type available () const
    {
        return pfs::distance(_first, _last);
    }

    bool at_end () const
    {
        return available() == ssize_t(0);
    }

    byte_string::const_iterator begin () const
    {
        return _first;
    }

    byte_string::const_iterator end () const
    {
        return _last;
    }

    byte_string::const_iterator cbegin () const
    {
        return _first;
    }

    byte_string::const_iterator cend () const
    {
        return _last;
    }

    /**
     * @return Actually skipped bytes.
     */
    size_t skip (size_t n)
    {
        n = pfs::min(n, integral_cast_check<size_t>(available()));
        pfs::advance(_first, n);
        return n;
    }

    endian const & order () const
    {
        return base_class::order();
    }

    void read_tail (byte_string & v)
    {
        v.append(_first, _last);
        _first = _last;
    }

    template <typename T>
    inline typename enable_if<is_integral<T>::value, byte_string_istream &>::type
    operator >> (T & v)
    {
        base_class::operator >> <T> (v);
        return *this;
    }

    template <typename T>
    inline typename enable_if<is_floating_point<T>::value, byte_string_istream &>::type
    operator >> (T & v)
    {
        base_class::operator >> <T> (v);
        return *this;
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

template <int N>
inline byte_string_istream & operator >> (byte_string_istream & is, byte_string_ref_n<N> const & v)
{
    typename byte_string_ref_n<N>::size_type sz = 0;
    is >> sz;
    is.read(*v.p, sz);
    return is;
}

inline byte_string_istream & operator >> (byte_string_istream & is, byte_string_ref const & v)
{
    is.read(*v.p, v.max_size);
    return is;
}


} // pfs
