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
    typedef JsonType                        json_type;
    typedef typename json_type::string_type string_type;
    typedef typename json_type::array_type  array_type;
    typedef typename json_type::object_type object_type;

    ubjson_istream (IStreamType & is, endian order = endian::big_endian)
        : _is(is)
    {}

    ubjson_istream & operator >> (json_type & j);

private:
//    ubjson_istream & input_json (json_type & j);

//    template <typename IntegerType>
//    ubjson_ostream & output_integer (IntegerType n, bool with_prefix);
//
//    template <typename RealType>
//    ubjson_ostream & output_real (RealType n, bool with_prefix);
//
//    ubjson_ostream & output_string (string_type const & s, bool with_prefix = true);
//    ubjson_ostream & output_array (json_type const & a);
//    ubjson_ostream & output_object (json_type const & o);
//
//    static int8_t prefix (json_type const & j);
//
//    struct compare_prefix
//    {
//        int8_t sample_prefix;
//
//        compare_prefix (int8_t p) : sample_prefix(p) {}
//
//        bool operator () (json_type const & j)
//        {
//            return prefix(j) == sample_prefix;
//        }
//    };

private:
    IStreamType & _is;
    endian        _order;
};

template <typename IStreamType, typename JsonType>
ubjson_istream<IStreamType, JsonType> & 
ubjson_istream<IStreamType, JsonType>::operator >> (json_type & j)
{
    int8_t type = -1;

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

    case static_cast<int8_t>('i'):// {
//        typename json<Traits>::uinteger_type d;
//        is >> d;
//        v = json<Traits>(d);
//        break;
//    }
        break;

    case static_cast<int8_t>('U'):
        break;

    case static_cast<int8_t>('l'):
        break;
        
    case static_cast<int8_t>('L'):
        break;
        
    case static_cast<int8_t>('d'):
        break;
        
    case static_cast<int8_t>('D'):
        break;
        
    case static_cast<int8_t>('C'):
        break;
        
    case static_cast<int8_t>('S'):
        break;
        
    case static_cast<int8_t>('['):
        break;
        
    case static_cast<int8_t>(']'):
        break;
        
//    case static_cast<int>(data_type::real): {
//        typename json<Traits>::real_type f;
//        is >> f;
//        v = json<Traits>(f);
//        break;
//    }
//
//    case static_cast<int>(data_type::string): {
//        byte_string u8;
//        is >> byte_string_ref_n<4>(& u8);
//        v = json<Traits>(typename json<Traits>::string_type(u8.c_str()));
//        break;
//    }
//
//    case static_cast<int>(data_type::array):
//    {
//        uint32_t n = 0;
//
//        is >> n;
//
//        if (n == 0) {
//            v = json<Traits>::make_array();
//        } else {
//            for (size_t i = 0; i < n; ++i) {
//                json<Traits> j;
//                is >> j;
//                v[i] = j;
//            }
//        }
//
//        break;
//    }
//
//    case static_cast<int>(data_type::object):
//    {
//        uint32_t n = 0;
//
//        is >> n;
//
//        if (n == 0) {
//            v = json<Traits>::make_object();
//        } else {
//            for (size_t i = 0; i < n; ++i) {
//                byte_string u8;
//                json<Traits> j;
//                is >> byte_string_ref_n<4>(& u8) >> j;
//
//                v[typename json<Traits>::string_type(u8.c_str())] = j;
//            }
//        }
//
//        break;
//    }
//
    default:
        throw json_exception(make_error_code(json_errc::bad_json));
    }

    return *this;
}


template <typename JsonType>
JsonType from_ubjson (pfs::byte_string const & bs)
{
    typedef ubjson_istream<pfs::byte_istream, JsonType> ubjson_istream_t;

    JsonType j;
    
    pfs::byte_istream bis(bs.cbegin(), bs.cend());
    ubjson_istream_t uis(bis);
    uis >> j;

    return j;
}

}} // pfs::json

#endif /* __PFS_JSON_UBJSON_ISTREAM_HPP__ */

