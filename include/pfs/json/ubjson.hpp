#ifndef __PFS_JSON_UBJSON_HPP__
#define __PFS_JSON_UBJSON_HPP__

#include <pfs/limits.hpp>
#include <pfs/endian.hpp>


//
// [Universal Binary JSON Specification](http://ubjson.org)
// 

namespace pfs {
namespace json {

// The Universal Binary JSON specification requires that all numeric values 
// be written in Big-Endian order.

template <typename ArchiveType, typename Traits>
struct ubjson_archive
{
    typedef json<Traits> json_type;
    
    ubjson_archive (ArchiveType & ar, endian order = endian::network_order())
        : _ar(ar)
        , _order(order)
    {}
    
    ubjson_archive & operator << (json_type const & v);
    ubjson_archive & operator >> (json_type & v);
    
private:
    int integer_width (intmax_t n);
    int uinteger_width (uintmax_t n);
    
private:
    ArchiveType & _ar;
    endian        _order;
};

template <typename ArchiveType, typename Traits>
int ubjson_archive<ArchiveType, Traits>::integer_width (intmax_t n)
{
    if (pfs::numeric_limits<int8_t>::min() <= n && n <= pfs::numeric_limits<int8_t>::max())
        return 1;

    if (pfs::numeric_limits<int16_t>::min() <= n && n <= pfs::numeric_limits<int16_t>::max())
        return 2;

    if (pfs::numeric_limits<int32_t>::min() <= n && n <= pfs::numeric_limits<int32_t>::max())
        return 4;

    if (pfs::numeric_limits<int64_t>::min() <= n && n <= pfs::numeric_limits<int64_t>::max())
        return 8;
    
    return sizeof(intmax_t);
}

template <typename ArchiveType, typename Traits>
int ubjson_archive<ArchiveType, Traits>::uinteger_width (uintmax_t n)
{
    if (n <= pfs::numeric_limits<uint8_t>::max())
        return 1;

    if (n <= pfs::numeric_limits<uint16_t>::max())
        return 2;

    if (n <= pfs::numeric_limits<uint32_t>::max())
        return 4;

    if (n <= pfs::numeric_limits<uint64_t>::max())
        return 8;
    
    return sizeof(uintmax_t);
}

template <typename ArchiveType, typename Traits>
ubjson_archive & ubjson_archive<ArchiveType, Traits>::operator << (json_type const & v)
{
    switch (v.type()) {
    case data_type::null:
        _ar << 'Z';
        break;

    case data_type::boolean:
        _ar << (v.template get<typename json_type::boolean_type>() ? 'T' : 'F');
        break;

    case data_type::integer: {
        intmax_t n = v.template get<typename json_type::integer_type>();
        int width = integer_width(n);
        
        switch (width) {
        case 1:
            _ar << 'i';
            _ar << static_cast<int8_t>(n);
            break;
        case 2:
            _ar << 'I';
            _ar << _order.convert(static_cast<int16_t>(n));
            break;
        case 4:
            _ar << 'l';
            _ar << _order.convert(static_cast<int32_t>(n));
            break;
        case 8:
            _ar << 'L';
            _ar << _order.convert(static_cast<int64_t>(n));
            break;
        }
        break;
    }

    case data_type::uinteger: {
        uintmax_t n = v.template get<typename json_type::uinteger_type>();
        int width = uinteger_width(n);
        
        switch (width) {
        case 1:
            if (n < numeric_limits<int8_t>::max())
                _ar << 'i';
            else
                _ar << 'U';
            _ar << static_cast<uint8_t>(n);
            break;
        case 2:
            _ar << 'I';
            _ar << _order.convert(static_cast<uint16_t>(n));
            break;
        case 4:
            _ar << 'l';
            _ar << _order.convert(static_cast<uint32_t>(n));
            break;
        case 8:
            _ar << 'L';
            _ar << _order.convert(static_cast<uint64_t>(n));
            break;
        }
        break;
    }

    case data_type::real:
        //os << v.template get<typename json<Traits>::real_type>();
        break;

    case data_type::string: {
        //byte_string u8 = u8string<byte_string>(v.template get<typename json<Traits>::string_type>());
        //os << byte_string_ref_n<4>(& u8);
        break;
    }

    case data_type::array: {
        //typename json<Traits>::const_iterator it = v.cbegin();
        //typename json<Traits>::const_iterator it_end = v.cend();
        //
        //os << static_cast<uint32_t>(v.size());
        //
        //for (; it != it_end; ++it)
        //    os << *it;

        break;
    }

    case data_type::object: {
        typename json<Traits>::const_iterator it = v.cbegin();
        typename json<Traits>::const_iterator it_end = v.cend();

        os << static_cast<uint32_t>(v.size());

        for (; it != it_end; ++it) {
            byte_string key = u8string<byte_string>(it.key());
            os << byte_string_ref_n<4>(& key) << *it;
        }

        break;
    }}

    return *this;
}

template <typename ArchiveType, typename Traits>
ubjson_archive & ubjson_archive<ArchiveType, Traits>::operator >> (json_type & v)
{
//    byte_t type = 0;
//
//    is >> type;

    switch (type) {
    case static_cast<int>(data_type::null):
//        v = json<Traits>();
        break;

    case static_cast<int>(data_type::boolean): {
//        bool b;
//        is >> b;
//        v = json<Traits>(b);
        break;
    }
//
    case static_cast<int>(data_type::integer): {
//        typename json<Traits>::integer_type d;
//        is >> d;
//        v = json<Traits>(d);
        break;
    }

    case static_cast<int>(data_type::uinteger): {
//        typename json<Traits>::uinteger_type d;
//        is >> d;
//        v = json<Traits>(d);
        break;
    }

    case static_cast<int>(data_type::real): {
//        typename json<Traits>::real_type f;
//        is >> f;
//        v = json<Traits>(f);
        break;
    }

    case static_cast<int>(data_type::string): {
//        byte_string u8;
//        is >> byte_string_ref_n<4>(& u8);
//        v = json<Traits>(typename json<Traits>::string_type(u8.c_str()));
        break;
    }

    case static_cast<int>(data_type::array): {
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
        break;
    }

    case static_cast<int>(data_type::object): {
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

        break;
    }

    default:
        throw json_exception(make_error_code(json_errc::bad_json));
    }
    
    return *this;
}

}} // pfs::json

#endif /* __PFS_JSON_UBJSON_HPP__ */

