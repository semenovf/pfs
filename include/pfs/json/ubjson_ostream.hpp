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
    enum falgs_enum {
          no_flags        = 0x00
        , type_optimized  = 0x01
        , count_optimized = 0x02
        , full_optimized  = type_optimized | count_optimized
    };

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
        return output_json(j, true);
    }

private:
    ubjson_ostream & output_json (json_type const & v, bool with_prefix);

    template <typename IntegerType>
    ubjson_ostream & output_integer (IntegerType n, bool with_prefix);

    template <typename RealType>
    ubjson_ostream & output_real (RealType n, bool with_prefix);

    ubjson_ostream & output_string (string_type const & s, bool with_prefix = true);
    ubjson_ostream & output_array (json_type const & a);
    ubjson_ostream & output_object (json_type const & o);

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

template <typename OStreamType, typename JsonTraits>
template <typename IntegerType>
ubjson_ostream<OStreamType, JsonTraits> &
ubjson_ostream<OStreamType, JsonTraits>::output_integer (IntegerType n, bool with_prefix)
{
    if (pfs::is_unsigned<IntegerType>()) {
        if (n <= pfs::numeric_limits<uint8_t>::max()) {
            if (n <= numeric_limits<int8_t>::max())
                _os << static_cast<int8_t>('i');
            else
                _os << static_cast<int8_t>('U');
            _os << static_cast<uint8_t>(n);
        } else if (n <= pfs::numeric_limits<uint16_t>::max()) {
            _os << static_cast<int8_t>('I');
            _os << _order.convert(static_cast<uint16_t>(n));
        } else if (n <= pfs::numeric_limits<uint32_t>::max()) {
            _os << static_cast<int8_t>('l');
            _os << _order.convert(static_cast<uint32_t>(n));
        }
#if PFS_HAVE_INT64
        else if (n <= pfs::numeric_limits<uint64_t>::max()) {
            _os << static_cast<int8_t>('L');
            _os << _order.convert(static_cast<uint64_t>(n));
        }
#endif
        else {
            throw json_exception(make_error_code(json_errc::range)
                , "unexpected unsigned integer width");
        }
    } else {
        if (pfs::numeric_limits<int8_t>::min() <= n && n <= pfs::numeric_limits<int8_t>::max()) {
            _os << static_cast<int8_t>('i');
            _os << static_cast<int8_t>(n);
        } else if (pfs::numeric_limits<int16_t>::min() <= n && n <= pfs::numeric_limits<int16_t>::max()) {
            _os << static_cast<int8_t>('I');
            _os << _order.convert(static_cast<int16_t>(n));
        } else if (pfs::numeric_limits<int32_t>::min() <= n && n <= pfs::numeric_limits<int32_t>::max()) {
            _os << static_cast<int8_t>('l');
            _os << _order.convert(static_cast<int32_t>(n));
        }
#if PFS_HAVE_INT64
        else if (pfs::numeric_limits<int64_t>::min() <= n && n <= pfs::numeric_limits<int64_t>::max()) {
            _os << static_cast<int8_t>('L');
            _os << _order.convert(static_cast<int64_t>(n));
        }
#endif
        else {
            throw json_exception(make_error_code(json_errc::range)
                , "unexpected signed integer width");
        }
    }

    return *this;
}

template <typename OStreamType, typename JsonTraits>
template <typename RealType>
ubjson_ostream<OStreamType, JsonTraits> &
ubjson_ostream<OStreamType, JsonTraits>::output_real (RealType n, bool with_prefix)
{
    if (pfs::is_same<RealType, real32_t>()) {
        _os << static_cast<int8_t>('d');
        _os << _order.convert(static_cast<int32_t>(n));
    } else if (pfs::is_same<RealType, real64_t>()) {
        _os << static_cast<int8_t>('D');
        _os << _order.convert(static_cast<int64_t>(n));
    } else {
        throw json_exception(make_error_code(json_errc::range)
            , "unexpected real type");
    }

    return *this;
}

template <typename OStreamType, typename JsonTraits>
ubjson_ostream<OStreamType, JsonTraits> &
ubjson_ostream<OStreamType, JsonTraits>::output_string (string_type const & s, bool with_prefix)
{
    if (with_prefix) {
        // Using size is safe here (no matter the string encoding)
        if (s.size() == 1 && *s.cbegin() <= numeric_limits<int8_t>::max()) {
            _os << static_cast<int8_t>('C');
            _os << static_cast<int8_t>((*s.cbegin()).value);
        } else {
            _os << static_cast<int8_t>('S');
            output_integer(s.size(), true);
            _os << s;
        }
    } else {
        // Used as key value in objects usually
        output_integer(s.size(), true);
        _os << s;
    }

    return *this;
}

template <typename OStreamType, typename JsonTraits>
ubjson_ostream<OStreamType, JsonTraits> &
ubjson_ostream<OStreamType, JsonTraits>::output_array (json_type const & j)
{
    _os << static_cast<int8_t>('[');

    bool use_count_optimization = ((_flags & count_optimized) && !j.empty());

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
            _os << static_cast<int8_t>('$');
            _os << first_prefix;
        }
    }

    if (use_count_optimization) {
        _os << static_cast<int8_t>('#');
        output_integer(j.size(), true);
    }

    typename json_type::const_iterator first = j.cbegin();
    typename json_type::const_iterator last  = j.cend();

    for (; first != last; ++first)
        output_json(*first, !use_type_optimization);

    // If a count is specified the container must not specify an end-marker.
    if (!use_count_optimization)
        _os << static_cast<int8_t>(']');

    return *this;
}

template <typename OStreamType, typename JsonTraits>
ubjson_ostream<OStreamType, JsonTraits> &
ubjson_ostream<OStreamType, JsonTraits>::output_object (json_type const & j)
{
    _os << static_cast<int8_t>('{');

    bool use_count_optimization = ((_flags & count_optimized) && !j.empty());

    // If a type is specified, a count must also be specified.
    // A type cannot be specified by itself.
    bool use_type_optimization  = ((_flags & type_optimized) && use_count_optimization);

    // Additional checks for type optimization
    if (use_type_optimization) {

        // This call is safe. Array is not empty as checked before
        int8_t first_prefix = prefix(*j.cbegin());
        use_type_optimization = pfs::all_of(j.cbegin(), j.cend(), compare_prefix(first_prefix));

        if (use_type_optimization) {
            _os << static_cast<int8_t>('$');
            _os << first_prefix;
        }
    }

    if (use_count_optimization) {
        _os << static_cast<int8_t>('#');
        output_integer(j.size(), true);
    }

    typename json_type::const_iterator first = j.cbegin();
    typename json_type::const_iterator last = j.cend();

    for (; first != last; ++first) {
        // The [S] (string) marker is omitted from each of the names in the
        // name/value pairings inside the object. The JSON specification does
        // not allow non-string name values, therefore the [S] marker
        // is redundant and must not be used.
        output_string(first.key(), false);
        output_json(*first, !use_type_optimization);
    }

    // If a count is specified the container must not specify an end-marker.
    if (!use_count_optimization)
        _os << static_cast<int8_t>('}');

    return *this;
}

template <typename OStreamType, typename JsonTraits>
int8_t ubjson_ostream<OStreamType, JsonTraits>::prefix (json_type const & j)
{
    switch (j.type()) {
    case data_type::null:
        return static_cast<int8_t>('Z');

    case data_type::boolean:
        return static_cast<int8_t>(j.boolean_data() ? 'T' : 'F');

    case data_type::integer: {
        typename json_type::integer_type const & n = j.integer_data();

        if (pfs::numeric_limits<int8_t>::min() <= n && n <= pfs::numeric_limits<int8_t>::max())
            return static_cast<int8_t>('i');
        else if (pfs::numeric_limits<int16_t>::min() <= n && n <= pfs::numeric_limits<int16_t>::max())
            return static_cast<int8_t>('I');
        else if (pfs::numeric_limits<int32_t>::min() <= n && n <= pfs::numeric_limits<int32_t>::max())
            return static_cast<int8_t>('l');
        else //if (pfs::numeric_limits<int64_t>::min() <= n && n <= pfs::numeric_limits<int64_t>::max())
            return static_cast<int8_t>('L');
    }

    case data_type::uinteger: {
        typename json_type::uinteger_type const & n = static_cast<typename json_type::uinteger_type>(j.integer_data());

        if (n <= pfs::numeric_limits<uint8_t>::max())
            if (n <= numeric_limits<int8_t>::max())
                return static_cast<int8_t>('i');
            else
                return static_cast<int8_t>('U');
        else if (n <= pfs::numeric_limits<uint16_t>::max())
            return static_cast<int8_t>('I');
        else if (n <= pfs::numeric_limits<uint32_t>::max())
            return static_cast<int8_t>('l');
        else //if (n <= pfs::numeric_limits<uint64_t>::max()) {
            return static_cast<int8_t>('L');
    }

    case data_type::real:
        return static_cast<int8_t>('D');

    case data_type::string: {
        typename json_type::string_type const & s = j.string_data();

        if (s.size() == 1 && *s.cbegin() <= numeric_limits<int8_t>::max())
            return static_cast<int8_t>('C');
        else
            return static_cast<int8_t>('S');
    }

    case data_type::array:
        return static_cast<int8_t>('[');

    case data_type::object:
        return static_cast<int8_t>('{');
    }
}

template <typename OStreamType, typename JsonTraits>
ubjson_ostream<OStreamType, JsonTraits> &
ubjson_ostream<OStreamType, JsonTraits>::output_json (json_type const & j, bool with_prefix)
{
    switch (j.type()) {
    case data_type::null:
        _os << static_cast<int8_t>('Z');
        break;

    case data_type::boolean:
        _os << (j.boolean_data() ? static_cast<int8_t>('T')
                : static_cast<int8_t>('F'));
        break;

    case data_type::integer:
        output_integer(j.integer_data(), with_prefix);
        break;

    case data_type::uinteger:
        output_integer(static_cast<typename json_type::uinteger_type const &>(j.integer_data())
                , with_prefix);
        break;

    case data_type::real:
        output_real(j.real_data(), with_prefix);
        break;

    case data_type::string:
        output_string(j.string_data(), with_prefix);
        break;

    case data_type::array:
        output_array(j);
        break;

    case data_type::object:
        output_object(j);
        break;
    }

    return *this;
}

template <typename JsonType>
pfs::byte_string to_ubjson (JsonType const & j)
{
    typedef pfs::json::ubjson_ostream<pfs::byte_ostream, JsonType> ubjson_ostream_t;

    pfs::byte_ostream os;
    ubjson_ostream_t uos(os);
    uos << j;
    return os.data();
}

}} // pfs::json

#endif /* __PFS_JSON_UBJSON_OSTREAM_HPP__ */

