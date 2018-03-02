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
        read(j);
        return *this;
    }

    void read (json_type & j);

private:
    void read_integer (integer_type & n, int8_t type = -1);
    void read_string (string_type & s, int8_t type = -1);
    void read_key (string_type & s);
    void read_array (json_type & j);
    void read_object (json_type & j);

private:
    IStreamType & _is;
    endian        _order;
};

template <typename IStreamType, typename JsonType>
void ubjson_istream<IStreamType, JsonType>::read (json_type & j)
{
    int8_t type = 0;
    _is >> type;

    switch (type) {
    case static_cast<int8_t>('Z'):
        j = json_type();
        break;

    case static_cast<int8_t>('T'):
        j = true;
        break;

    case static_cast<int8_t>('F'):
        j = false;
        break;

    case static_cast<int8_t>('i'):
    case static_cast<int8_t>('U'):
    case static_cast<int8_t>('I'):
    case static_cast<int8_t>('l'):
    case static_cast<int8_t>('L'): {
        integer_type n;
        read_integer(n, type);
        j = n;
        break;
    }

    case static_cast<int8_t>('d'): {
        union { real32_t f; int32_t d; } u;
        _is >> u.d;
        u.d = _order.convert(u.d);
        j = u.f;
        break;
    }

    case static_cast<int8_t>('D'): {
        union { real64_t f; int64_t d; } u;
        _is >> u.d;
        u.d = _order.convert(u.d);
        j = u.f;
        break;
    }

    case static_cast<int8_t>('C'):
    case static_cast<int8_t>('S'): {
        string_type s;
        read_string(s, type);
        j = s;
        break;
    }

    case static_cast<int8_t>('['):
        read_array(j);
        break;

    case static_cast<int8_t>('{'):
        read_object(j);
        break;

    case static_cast<int8_t>(']'):
    case static_cast<int8_t>('}'):
        break;

    default:
        throw json_exception(make_error_code(json_errc::bad_json));
    }
}

template <typename IStreamType, typename JsonType>
void ubjson_istream<IStreamType, JsonType>::read_integer (integer_type & r, int8_t type)
{
    if (type < 0)
        _is >> type;

    switch (type) {
    case static_cast<int8_t>('i'): {
        int8_t n;
        _is >> n;
        r = n;
        break;
    }

    case static_cast<int8_t>('U'): {
        uint8_t n;
        _is >> n;
        r = n;
        break;
    }

    case static_cast<int8_t>('I'): {
        int16_t n;
        _is >> n;
        r = _order.convert(n);
        break;
    }

    case static_cast<int8_t>('l'): {
        int32_t n;
        _is >> n;
        r = _order.convert(n);
        break;
    }

    case static_cast<int8_t>('L'): {
#if PFS_HAVE_INT64
        int64_t n;
        _is >> n;
        r = _order.convert(n);
#else
        throw json_exception(make_error_code(json_errc::range));
#endif
        break;
    }

    default:
        throw json_exception(make_error_code(json_errc::range));
    }
}

template <typename IStreamType, typename JsonType>
void ubjson_istream<IStreamType, JsonType>::read_string (string_type & s, int8_t type)
{
    switch (type) {
    case static_cast<int8_t>('C'): {
        char c;
        _is >> c;
        s = string_type(1, c);
        break;
    }

    case static_cast<int8_t>('S'): {
        integer_type n;
        read_integer(n);

        byte_string bs;

        while (n--) {
            byte_t c;
            _is >> c;
            bs.push_back(c);
        }

        s = string_type(bs.c_str());

        break;
    }

//    case static_cast<int8_t>('}'): {
//        if (_current_type != static_cast<int8_t>('{')) {
//            throw json_exception(make_error_code(json_errc::object_expected));
//        } else {
//            _current_type = type;
//        }
//        break;
    }}
}

template <typename IStreamType, typename JsonType>
void ubjson_istream<IStreamType, JsonType>::read_key (string_type & s)
{
    integer_type n;
    read_integer(n);

    byte_string bs;
    byte_t c = 0;

    while (n--) {
        _is >> c;
        bs.push_back(c);
    }

    s = string_type(bs.c_str());
}

template <typename IStreamType, typename JsonType>
void ubjson_istream<IStreamType, JsonType>::read_array (json_type & j)
{
    j = JsonType::make_array();

    while (true) {
        json_type jj;
        read(jj);

        if(_current_type == static_cast<int8_t>(']'))
            break;

        j.push_back(jj);
    }
}

template <typename IStreamType, typename JsonType>
void ubjson_istream<IStreamType, JsonType>::read_object (json_type & j)
{
    j = JsonType::make_object();

    while (true) {
        string_type key;

        read_key(key);

        // Here checks object is empty or object is finished
        if(_current_type == static_cast<int8_t>('}'))
            break;

        json_type jj;
        read(jj);

        if(_current_type == static_cast<int8_t>('}'))
            throw json_exception(make_error_code(json_errc::object_expected));

        j[key] = jj;
    }
}

template <typename JsonType>
JsonType from_ubjson (pfs::byte_string const & bs)
{
    typedef ubjson_istream<pfs::byte_istream, JsonType> ubjson_istream_t;

    JsonType j;

    pfs::byte_istream bis(bs.cbegin(), bs.cend(), endian::native_order());
    ubjson_istream_t(bis).read(j);

    return j;
}

}} // pfs::json

#endif /* __PFS_JSON_UBJSON_ISTREAM_HPP__ */

