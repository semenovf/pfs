#ifndef __PFS_JSON_UBJSON_ISTREAM_HPP__
#define __PFS_JSON_UBJSON_ISTREAM_HPP__

//#include <pfs/algorithm.hpp>
//#include <pfs/limits.hpp>
#include <pfs/endian.hpp>
#include <pfs/byte_string.hpp>
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
        pfs::error_code ex = read(j, UBJSON_CHAR_UNSPEC);
        if (ex)
            throw json_exception(ex);
        return *this;
    }

    pfs::error_code read (json_type & j, int8_t type);

private:
    int8_t read_type ();
    pfs::error_code read_integer (integer_type & n, int8_t type);
    pfs::error_code read_string (string_type & s, int8_t type);
    pfs::error_code read_key (string_type & s, int8_t type);
    pfs::error_code read_array (json_type & j);
    pfs::error_code read_object (json_type & j);

private:
    IStreamType & _is;
    endian        _order;
};

template <typename IStreamType, typename JsonType>
int8_t ubjson_istream<IStreamType, JsonType>::read_type ()
{
    int8_t type = UBJSON_CHAR_UNSPEC;

    do {
        _is >> type;
    } while (type == UBJSON_CHAR_NOOP);

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
        read_array(j);
        break;

    case UBJSON_CHAR_OBJECT_BEGIN:
        read_object(j);
        break;

    default:
        return make_error_code(json_errc::bad_json);
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
        return make_error_code(json_errc::range);
#endif
        break;
    }

    default:
        return make_error_code(json_errc::range);
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
    j = JsonType::make_array();

    int8_t type = UBJSON_CHAR_UNSPEC;

    while ((type = read_type()) != UBJSON_CHAR_ARRAY_END) {
        json_type child;

        pfs::error_code ex = read(child, type);

        if (ex)
            return ex;

        j.push_back(child);
    }

    return pfs::error_code();
}

template <typename IStreamType, typename JsonType>
pfs::error_code ubjson_istream<IStreamType, JsonType>::read_object (json_type & j)
{
    j = JsonType::make_object();

    int8_t type = UBJSON_CHAR_UNSPEC;

    while ((type = read_type()) != UBJSON_CHAR_OBJECT_END) {
        string_type key;

        pfs::error_code ex = read_key(key, type);

        if (ex)
            return ex;

        json_type child;

        ex = read(child, UBJSON_CHAR_UNSPEC);

        if (ex)
            return ex;

        j[key] = child;
    }
}

template <typename JsonType>
JsonType from_ubjson (pfs::byte_string const & bs, pfs::error_code & ex)
{
    typedef ubjson_istream<pfs::byte_istream, JsonType> ubjson_istream_t;

    JsonType j;

    pfs::byte_istream bis(bs.cbegin(), bs.cend(), endian::native_order());
    ex = ubjson_istream_t(bis).read(j, UBJSON_CHAR_UNSPEC);

    return j;
}

template <typename JsonType>
JsonType from_ubjson (pfs::byte_string const & bs)
{
    pfs::error_code ex;
    JsonType j = from_ubjson<JsonType>(bs, ex);

    if (ex)
        throw json_exception(ex);

    return j;
}

}} // pfs::json

#endif /* __PFS_JSON_UBJSON_ISTREAM_HPP__ */

