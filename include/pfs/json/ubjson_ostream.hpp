#ifndef __PFS_JSON_UBJSON_OSTREAM_HPP__
#define __PFS_JSON_UBJSON_OSTREAM_HPP__

#include <pfs/algorithm.hpp>
#include <pfs/limits.hpp>
#include <pfs/endian.hpp>
#include <pfs/json/json.hpp>

//
// [Universal Binary JSON Specification](http://ubjson.org)
//

namespace pfs {
namespace json {

// The Universal Binary JSON specification requires that all numeric values
// be written in Big-Endian order.

template <typename OStreamType, typename JsonType>
struct ubjson_ostream
{
private:
    enum falgs_enum {
          no_flags        = 0x00
        , type_optimized  = 0x01
        , count_optimized = 0x02
        , full_optimized  = type_optimized | count_optimized
    };

public:
    typedef JsonType                        json_type;
    typedef typename json_type::string_type string_type;
    typedef typename json_type::array_type  array_type;
    typedef typename json_type::object_type object_type;

    ubjson_ostream (OStreamType & ar, int flags = no_flags)
        : _os(ar)
        , _flags(flags)
        , _order(endian::big_endian)
    {}

    ubjson_ostream (OStreamType & ar, endian const & order, int flags = no_flags)
        : _os(ar)
        , _flags(flags)
        , _order(order)
    {}

    ubjson_ostream & operator << (json_type const & j)
    {
        pfs::error_code ex = write_json(j, true);
        if (ex)
            throw json_exception(ex);
        return *this;
    }

    pfs::error_code write (json_type const & j)
    {
        return write_json(j, true);
    }

private:
    pfs::error_code write_json    (json_type const & v, bool with_prefix);
    pfs::error_code write_integer (typename json_type::integer_type n, bool with_prefix);
    pfs::error_code write_real    (typename json_type::real_type f, bool with_prefix);
    pfs::error_code write_string  (string_type const & s, bool with_prefix = true);
    pfs::error_code write_array   (json_type const & a);
    pfs::error_code write_object  (json_type const & o);

    static int8_t prefix (json_type const & j);

    struct compare_prefix
    {
        int8_t sample_prefix;

        compare_prefix (int8_t p) : sample_prefix(p) {}

        bool operator () (json_type const & j)
        {
            return prefix(j) == sample_prefix;
        }
    };

private:
    OStreamType & _os;
    int8_t        _flags;
    endian        _order;
};

template <typename OStreamType, typename JsonType>
int8_t ubjson_ostream<OStreamType, JsonType>::prefix (json_type const & j)
{
    switch (j.type()) {
    case data_type::null:
        return UBJSON_CHAR_NULL;

    case data_type::boolean:
        return static_cast<int8_t>(j.boolean_data()
                ? UBJSON_CHAR_TRUE
                : UBJSON_CHAR_FALSE);

    case data_type::integer: {
        typename json_type::integer_type const & n = j.integer_data();

        if (pfs::numeric_limits<int8_t>::max() < n && n <= pfs::numeric_limits<uint8_t>::max())
            return UBJSON_CHAR_UINT8;
        else if (pfs::numeric_limits<int8_t>::min() <= n && n <= pfs::numeric_limits<int8_t>::max())
            return UBJSON_CHAR_INT8;
        else if (pfs::numeric_limits<int16_t>::min() <= n && n <= pfs::numeric_limits<int16_t>::max())
            return UBJSON_CHAR_INT16;
        else if (pfs::numeric_limits<int32_t>::min() <= n && n <= pfs::numeric_limits<int32_t>::max())
            return UBJSON_CHAR_INT32;
        else //if (pfs::numeric_limits<int64_t>::min() <= n && n <= pfs::numeric_limits<int64_t>::max())
            return UBJSON_CHAR_INT64;
    }

    case data_type::real:
        return UBJSON_CHAR_REAL64;

    case data_type::string: {
        typename json_type::string_type const & s = j.string_data();

        if (s.size() == 1 && *s.cbegin() <= numeric_limits<int8_t>::max())
            return UBJSON_CHAR_CHAR;
        else
            return UBJSON_CHAR_STRING;
    }

    case data_type::array:
        return UBJSON_CHAR_ARRAY_BEGIN;

    case data_type::object:
        return UBJSON_CHAR_OBJECT_BEGIN;
    }
}

template <typename OStreamType, typename JsonType>
pfs::error_code ubjson_ostream<OStreamType, JsonType>::write_integer (typename json_type::integer_type n, bool with_prefix)
{
    if (pfs::numeric_limits<int8_t>::max() < n && n <= pfs::numeric_limits<uint8_t>::max()) {
        if (with_prefix)
            _os << UBJSON_CHAR_UINT8;
        _os << static_cast<uint8_t>(n);
    } else if (pfs::numeric_limits<int8_t>::min() <= n && n <= pfs::numeric_limits<int8_t>::max()) {
        if (with_prefix)
            _os << UBJSON_CHAR_INT8;
        _os << static_cast<int8_t>(n);
    } else if (pfs::numeric_limits<int16_t>::min() <= n && n <= pfs::numeric_limits<int16_t>::max()) {
        if (with_prefix)
            _os << UBJSON_CHAR_INT16;
        _os << _order.convert(static_cast<int16_t>(n));
    } else if (pfs::numeric_limits<int32_t>::min() <= n && n <= pfs::numeric_limits<int32_t>::max()) {
        if (with_prefix)
            _os << UBJSON_CHAR_INT32;
        _os << _order.convert(static_cast<int32_t>(n));
    }
#if PFS_HAVE_INT64
    else if (pfs::numeric_limits<int64_t>::min() <= n && n <= pfs::numeric_limits<int64_t>::max()) {
        if (with_prefix)
            _os << UBJSON_CHAR_INT64;
        _os << _order.convert(static_cast<int64_t>(n));
    }
#endif
    else {
        return make_error_code(json_errc::range);
    }

    return pfs::error_code();
}

template <typename OStreamType, typename JsonType>
pfs::error_code ubjson_ostream<OStreamType, JsonType>::write_real (typename json_type::real_type f, bool with_prefix)
{
    if (pfs::is_same<typename json_type::real_type, real32_t>()) {
        if (with_prefix)
            _os << static_cast<int8_t>('d');
        _os << _order.convert(static_cast<int32_t>(f));
    } else if (pfs::is_same<typename json_type::real_type, real64_t>()) {
        union { real64_t f; int64_t d; } u;
        u.f = f;

        if (with_prefix)
            _os << static_cast<int8_t>('D');
        _os << _order.convert(u.d);
    } else {
        return make_error_code(json_errc::range);
    }

    return pfs::error_code();
}

template <typename OStreamType, typename JsonType>
pfs::error_code ubjson_ostream<OStreamType, JsonType>::write_string (string_type const & s, bool with_prefix)
{
    if (with_prefix) {
        // Using size is safe here (no matter the string encoding)
        if (s.size() == 1 && *s.cbegin() <= numeric_limits<int8_t>::max()) {
            _os << static_cast<int8_t>('C');
            _os << static_cast<int8_t>((*s.cbegin()).value);
        } else {
            _os << static_cast<int8_t>('S');
            write_integer(static_cast<typename json_type::integer_type>(s.size()), true);
            _os << s;
        }
    } else {
        write_integer(static_cast<typename json_type::integer_type>(s.size()), true);
        _os << s;
    }

    return pfs::error_code();
}

template <typename OStreamType, typename JsonType>
pfs::error_code ubjson_ostream<OStreamType, JsonType>::write_array (json_type const & j)
{
    _os << UBJSON_CHAR_ARRAY_BEGIN;

    bool use_count_optimization = ((_flags & count_optimized) && !j.empty());
    bool is_special_case = false;

    // If a type is specified, a count must also be specified.
    // A type cannot be specified by itself.
    bool use_type_optimization  = ((_flags & type_optimized) && use_count_optimization);

    // Additional checks for type optimization
    if (use_type_optimization) {

        // This call is safe. Array is not empty as checked before
        int8_t first_prefix = prefix(*j.cbegin());
        use_type_optimization = pfs::all_of(j.cbegin()
                , j.cend()
                , compare_prefix(first_prefix));

        if (use_type_optimization) {
            _os << UBJSON_CHAR_TYPE;
            _os << first_prefix;

            // Check special cases
            if (first_prefix == UBJSON_CHAR_NOOP
                    || first_prefix == UBJSON_CHAR_NULL
                    || first_prefix == UBJSON_CHAR_TRUE
                    || first_prefix == UBJSON_CHAR_FALSE) {
                is_special_case = true;
            }
        }
    }

    if (use_count_optimization) {
        _os << UBJSON_CHAR_SIZE;
        write_integer(static_cast<typename json_type::integer_type>(j.size()), true);
    }

    // Ignore special cases: strongly-typed arrays of null, no-op and boolean values
    if (! is_special_case) {
        typename json_type::const_iterator first = j.cbegin();
        typename json_type::const_iterator last  = j.cend();

        for (; first != last; ++first)
            write_json(*first, !use_type_optimization);
    }

    // If a count is specified the container must not specify an end-marker.
    if (!use_count_optimization)
        _os << UBJSON_CHAR_ARRAY_END;

    return pfs::error_code();
}

template <typename OStreamType, typename JsonType>
pfs::error_code ubjson_ostream<OStreamType, JsonType>::write_object (json_type const & j)
{
    _os << UBJSON_CHAR_OBJECT_BEGIN;

    bool use_count_optimization = ((_flags & count_optimized) && !j.empty());
    bool is_special_case = false;

    // If a type is specified, a count must also be specified.
    // A type cannot be specified by itself.
    bool use_type_optimization  = ((_flags & type_optimized) && use_count_optimization);

    // Additional checks for type optimization
    if (use_type_optimization) {

        // This call is safe. Array is not empty as checked before
        int8_t first_prefix = prefix(*j.cbegin());
        use_type_optimization = pfs::all_of(j.cbegin(), j.cend(), compare_prefix(first_prefix));

        if (use_type_optimization) {
            _os << UBJSON_CHAR_TYPE;
            _os << first_prefix;

            // Check special cases
            if (first_prefix == UBJSON_CHAR_NOOP
                    || first_prefix == UBJSON_CHAR_NULL
                    || first_prefix == UBJSON_CHAR_TRUE
                    || first_prefix == UBJSON_CHAR_FALSE) {
                is_special_case = true;
            }
        }
    }

    if (use_count_optimization) {
        _os << UBJSON_CHAR_SIZE;
        write_integer(static_cast<typename json_type::integer_type>(j.size()), true);
    }

    typename json_type::const_iterator first = j.cbegin();
    typename json_type::const_iterator last = j.cend();

    if (is_special_case) {
        for (; first != last; ++first) {
            // The [S] (string) marker is omitted from each of the names in the
            // name/value pairings inside the object. The JSON specification does
            // not allow non-string name values, therefore the [S] marker
            // is redundant and must not be used.
           write_string(first.key(), false);
        }
    } else {
        for (; first != last; ++first) {
            write_string(first.key(), false);
            write_json(*first, !use_type_optimization);
        }
    }

    // If a count is specified the container must not specify an end-marker.
    if (!use_count_optimization)
        _os << UBJSON_CHAR_OBJECT_END;

    return pfs::error_code();
}

template <typename OStreamType, typename JsonType>
pfs::error_code ubjson_ostream<OStreamType, JsonType>::write_json (json_type const & j, bool with_prefix)
{
    switch (j.type()) {
    case data_type::null:
        _os << UBJSON_CHAR_NULL;
        break;

    case data_type::boolean:
        _os << (j.boolean_data()
                ? UBJSON_CHAR_TRUE
                : UBJSON_CHAR_FALSE);
        break;

    case data_type::integer:
        return write_integer(j.integer_data(), with_prefix);

    case data_type::real:
        return write_real(j.real_data(), with_prefix);

    case data_type::string:
        return write_string(j.string_data(), with_prefix);

    case data_type::array:
        return write_array(j);

    case data_type::object:
        return write_object(j);
    }

    return pfs::error_code();
}

template <typename JsonType>
pfs::byte_string to_ubjson (JsonType const & j, int flags, pfs::error_code & ex)
{
    typedef pfs::json::ubjson_ostream<pfs::byte_ostream, JsonType> ubjson_ostream_t;

    pfs::byte_string buffer;
    // ubjson_ostream_t has own order------
    //                                    |
    //                                    v
    pfs::byte_ostream os(buffer, endian::native_order());
    ex = ubjson_ostream_t(os, flags).write(j);
    return os.data();
}

template <typename JsonType>
inline pfs::byte_string to_ubjson (JsonType const & j, pfs::error_code & ex)
{
    return to_ubjson<JsonType>(j, 0, ex);
}

template <typename JsonType>
pfs::byte_string to_ubjson (JsonType const & j, int flags)
{
    pfs::error_code ex;
    pfs::byte_string r = to_ubjson<JsonType>(j, flags, ex);

    if (ex)
        throw json_exception(ex);

    return r;
}

template <typename JsonType>
inline pfs::byte_string to_ubjson (JsonType const & j)
{
    return to_ubjson<JsonType>(j, 0);
}


}} // pfs::json

#endif /* __PFS_JSON_UBJSON_OSTREAM_HPP__ */

