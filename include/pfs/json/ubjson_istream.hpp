#pragma once
#include <pfs/endian.hpp>
#include <pfs/byte_string.hpp>
#include <pfs/byte_string_istream.hpp>
#include <pfs/json/json.hpp>

//
// [Universal Binary JSON Specification](http://ubjson.org)
//

namespace pfs {
namespace json {

// The Universal Binary JSON specification requires that all numeric values
// be written in Big-Endian order.

template <typename IStreamType, typename JsonType>
struct ubjson_istream
{
    typedef JsonType                         json_type;
    typedef typename json_type::integer_type integer_type;
    typedef typename json_type::string_type  string_type;
    typedef typename json_type::array_type   array_type;
    typedef typename json_type::object_type  object_type;

    ubjson_istream (IStreamType & is, endian order = endian::big_endian)
        : _is(is)
        , _order(order)
    {}

    ubjson_istream & operator >> (json_type & j)
    {
        error_code ec = read(j, UBJSON_CHAR_UNSPEC);
        if (ec)
            PFS_THROW(json_exception(ec));
        return *this;
    }

    error_code read (json_type & j, int8_t type = UBJSON_CHAR_UNSPEC);

private:
    int8_t read_type (bool ignore_noop = true);
    error_code read_integer (integer_type & n, int8_t type);
    error_code read_string (string_type & s, int8_t type);
    error_code read_key (string_type & s, int8_t type);
    error_code read_array (json_type & j);
    error_code read_nonoptimized_array (json_type & j, int8_t ch);
    error_code read_optimized_array (json_type & j, integer_type count, int8_t type);
    error_code read_object (json_type & j);
    error_code read_nonoptimized_object (json_type & j, int8_t ch);
    error_code read_optimized_object (json_type & j, integer_type count, int8_t type);

private:
    IStreamType & _is;
    endian        _order;
};

template <typename IStreamType, typename JsonType>
int8_t ubjson_istream<IStreamType, JsonType>::read_type (bool ignore_noop)
{
    int8_t type = UBJSON_CHAR_UNSPEC;

    do {
        _is >> type;
    } while (ignore_noop && type == UBJSON_CHAR_NOOP);

    return type;
}

template <typename IStreamType, typename JsonType>
pfs::error_code ubjson_istream<IStreamType, JsonType>::read (json_type & j, int8_t type)
{
    if (type == UBJSON_CHAR_UNSPEC)
        type = read_type();

    switch (type) {
    case UBJSON_CHAR_NULL:
        j = json_type();
        break;

    case UBJSON_CHAR_TRUE:
        j = true;
        break;

    case UBJSON_CHAR_FALSE:
        j = false;
        break;

    case UBJSON_CHAR_INT8:
    case UBJSON_CHAR_UINT8:
    case UBJSON_CHAR_INT16:
    case UBJSON_CHAR_INT32:
    case UBJSON_CHAR_INT64: {
        integer_type n;
        read_integer(n, type);
        j = n;
        break;
    }

    case UBJSON_CHAR_REAL32: {
        union { real32_t f; int32_t d; } u;
        _is >> u.d;
        u.d = _order.convert(u.d);
        j = u.f;
        break;
    }

    case UBJSON_CHAR_REAL64: {
        union { real64_t f; int64_t d; } u;
        _is >> u.d;
        u.d = _order.convert(u.d);
        j = u.f;
        break;
    }

    case UBJSON_CHAR_CHAR:
    case UBJSON_CHAR_STRING: {
        string_type s;
        read_string(s, type);
        j = s;
        break;
    }

    case UBJSON_CHAR_ARRAY_BEGIN:
        return read_array(j);

    case UBJSON_CHAR_OBJECT_BEGIN:
        return read_object(j);

    default:
        return make_error_code(json_errc::ubjson_parse);
    }

    return pfs::error_code();
}

template <typename IStreamType, typename JsonType>
pfs::error_code ubjson_istream<IStreamType, JsonType>::read_integer (integer_type & r, int8_t type)
{
    if (type == UBJSON_CHAR_UNSPEC)
        _is >> type;

    switch (type) {
    case UBJSON_CHAR_INT8: {
        int8_t n;
        _is >> n;
        r = n;
        break;
    }

    case UBJSON_CHAR_UINT8: {
        uint8_t n;
        _is >> n;
        r = n;
        break;
    }

    case UBJSON_CHAR_INT16: {
        int16_t n;
        _is >> n;
        r = _order.convert(n);
        break;
    }

    case UBJSON_CHAR_INT32: {
        int32_t n;
        _is >> n;
        r = _order.convert(n);
        break;
    }

    case UBJSON_CHAR_INT64: {
#if PFS_HAVE_INT64
        int64_t n;
        _is >> n;
        r = _order.convert(n);
#else
        return make_error_code(json_errc::ubjson_parse);
#endif
        break;
    }

    default:
        return make_error_code(json_errc::ubjson_parse);
    }

    return pfs::error_code();
}

template <typename IStreamType, typename JsonType>
pfs::error_code ubjson_istream<IStreamType, JsonType>::read_string (string_type & s, int8_t type)
{
    if (type == UBJSON_CHAR_UNSPEC)
        _is >> type;

    switch (type) {
    case UBJSON_CHAR_CHAR: {
        char c;
        _is >> c;
        s = string_type(1, c);
        break;
    }

    case UBJSON_CHAR_STRING: {
        integer_type n;
        read_integer(n, UBJSON_CHAR_UNSPEC);

        byte_string bs;

        while (n--) {
            byte_t c;
            _is >> c;
            bs.push_back(c);
        }

        s = string_type(bs.c_str());

        break;
    }}

    return pfs::error_code();
}

template <typename IStreamType, typename JsonType>
pfs::error_code ubjson_istream<IStreamType, JsonType>::read_key (string_type & s, int8_t type)
{
    if (type == UBJSON_CHAR_UNSPEC)
        _is >> type;

    integer_type n;
    pfs::error_code ex = read_integer(n, type);

    if (ex)
        return ex;

    byte_string bs;
    byte_t c = 0;

    while (n--) {
        _is >> c;
        bs.push_back(c);
    }

    s = string_type(bs.c_str());

    return pfs::error_code();
}

template <typename IStreamType, typename JsonType>
pfs::error_code ubjson_istream<IStreamType, JsonType>::read_array (json_type & j)
{
    int8_t ch = read_type();

    if (ch == UBJSON_CHAR_TYPE) {
        int8_t type = read_type(false); // NO-OP can be valid type
        ch = read_type();

        if (ch == UBJSON_CHAR_SIZE) {
            integer_type count = -1;
            pfs::error_code ex  = read_integer(count, UBJSON_CHAR_UNSPEC);

            if (ex)
                return ex;

            return read_optimized_array(j, count, type);
        } else {
            return make_error_code(json_errc::ubjson_parse);
        }
    } else if (ch == UBJSON_CHAR_SIZE) {
        integer_type count = -1;
        pfs::error_code ex = read_integer(count, UBJSON_CHAR_UNSPEC);

        if (ex)
            return ex;

        return read_optimized_array(j, count, UBJSON_CHAR_UNSPEC);
    } else {
        return read_nonoptimized_array(j, ch);
    }
}

template <typename IStreamType, typename JsonType>
pfs::error_code
ubjson_istream<IStreamType, JsonType>::read_nonoptimized_array (json_type & j
        , int8_t ch)
{
    j = JsonType::make_array();

    while (ch != UBJSON_CHAR_ARRAY_END) {
        json_type child;

        pfs::error_code ex = read(child, ch);

        if (ex)
            return ex;

        j.push_back(child);

        ch = read_type();
    }

    return pfs::error_code();
}

template <typename IStreamType, typename JsonType>
pfs::error_code
ubjson_istream<IStreamType, JsonType>::read_optimized_array (json_type & j
        , integer_type count
        , int8_t type)
{
    // Special cases: strongly-typed arrays of null, no-op and boolean values

    // Ignore values
    if (type == UBJSON_CHAR_NOOP)
        return pfs::error_code();

    j = JsonType::make_array();

    if (type == UBJSON_CHAR_NULL) {
        // Simple add null values
        while (count-- > 0)
            j.push_back(json_type());
    } else if (type == UBJSON_CHAR_TRUE) {
        // Simple add boolean true values
        while (count-- > 0)
            j.push_back(json_type(true));
    } else if (type == UBJSON_CHAR_FALSE) {
        // Simple add boolean false values
        while (count-- > 0)
            j.push_back(json_type(false));
    } else {
        while (count-- > 0) {
            json_type child;

            pfs::error_code ex = read(child, type);

            if (ex)
                return ex;

            j.push_back(child);
        }
    }

    return pfs::error_code();
}

template <typename IStreamType, typename JsonType>
pfs::error_code ubjson_istream<IStreamType, JsonType>::read_object (json_type & j)
{
    int8_t ch = read_type();

    if (ch == UBJSON_CHAR_TYPE) {
        int8_t type = read_type(false); // NO-OP can be valid type
        ch = read_type();

        if (ch == UBJSON_CHAR_SIZE) {
            integer_type count = -1;
            pfs::error_code ex  = read_integer(count, UBJSON_CHAR_UNSPEC);

            if (ex)
                return ex;

            return read_optimized_object(j, count, type);
        } else {
            return make_error_code(json_errc::ubjson_parse);
        }
    } else if (ch == UBJSON_CHAR_SIZE) {
        integer_type count = -1;
        pfs::error_code ex = read_integer(count, UBJSON_CHAR_UNSPEC);

        if (ex)
            return ex;

        return read_optimized_object(j, count, UBJSON_CHAR_UNSPEC);
    } else {
        return read_nonoptimized_object(j, ch);
    }
}

template <typename IStreamType, typename JsonType>
pfs::error_code
ubjson_istream<IStreamType, JsonType>::read_nonoptimized_object (json_type & j
        , int8_t ch)
{
    j = JsonType::make_object();

    while (ch != UBJSON_CHAR_OBJECT_END) {
        string_type key;

        pfs::error_code ex = read_key(key, ch);

        if (ex)
            return ex;

        json_type child;

        ex = read(child, UBJSON_CHAR_UNSPEC);

        if (ex)
            return ex;

        j[key] = child;

        ch = read_type();
    }
    return pfs::error_code();
}

template <typename IStreamType, typename JsonType>
pfs::error_code
ubjson_istream<IStreamType, JsonType>::read_optimized_object (json_type & j
        , integer_type count
        , int8_t type)
{
    // Special cases: strongly-typed arrays of null, no-op and boolean values

    // Ignore values
    if (type == UBJSON_CHAR_NOOP) {
        while (count-- > 0) {
            string_type key;
            pfs::error_code ex = read_key(key, UBJSON_CHAR_UNSPEC);
            if (ex) return ex;
        }

        return pfs::error_code();
    }

    j = JsonType::make_object();

    if (type == UBJSON_CHAR_NULL) {
        while (count-- > 0) {
            string_type key;
            pfs::error_code ex = read_key(key, UBJSON_CHAR_UNSPEC);
            if (ex) return ex;
            j[key] = json_type();
        }
    } else if (type == UBJSON_CHAR_TRUE) {
        while (count-- > 0) {
            string_type key;
            pfs::error_code ec = read_key(key, UBJSON_CHAR_UNSPEC);
            if (ec) return ec;
            j[key] = json_type(true);
        }
    } else if (type == UBJSON_CHAR_FALSE) {
        while (count-- > 0) {
            string_type key;
            pfs::error_code ec = read_key(key, UBJSON_CHAR_UNSPEC);
            if (ec) return ec;
            j[key] = json_type(false);
        }
    } else {
        while (count-- > 0) {
            string_type key;

            pfs::error_code ec = read_key(key, UBJSON_CHAR_UNSPEC);

            if (ec)
                return ec;

            json_type child;

            ec = read(child, type);

            if (ec)
                return ec;

            j[key] = child;
        }
    }

    return pfs::error_code();
}

template <typename JsonType>
JsonType from_ubjson (pfs::byte_string::const_iterator first
        , pfs::byte_string::const_iterator last
        , pfs::error_code & ec)
{
    typedef ubjson_istream<byte_string_istream, JsonType> ubjson_istream_t;

    JsonType j;

    // ubjson_istream_t has own order-------------------------
    //                                                       |
    //                                                       v
    byte_string_istream bis(first, last, endian::native_order());
    ec = ubjson_istream_t(bis).read(j);

    return j;
}

template <typename JsonType>
inline JsonType from_ubjson (pfs::byte_string const & bs, pfs::error_code & ec)
{
    return from_ubjson<JsonType>(bs.cbegin(), bs.cend(), ec);
}

template <typename JsonType>
JsonType from_ubjson (pfs::byte_string::const_iterator first
        , pfs::byte_string::const_iterator last)
{
    pfs::error_code ec;
    JsonType j = from_ubjson<JsonType>(first, last, ec);

    if (ec)
        PFS_THROW(json_exception(ec));

    return j;
}

template <typename JsonType>
JsonType from_ubjson (pfs::byte_string const & bs)
{
    pfs::error_code ec;
    JsonType j = from_ubjson<JsonType>(bs, ec);

    if (ec)
        PFS_THROW(json_exception(ec));

    return j;
}

}} // pfs::json
