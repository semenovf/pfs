#pragma once
#include <pfs/lexical_cast.hpp>

namespace pfs {
namespace json {
namespace details {

template <typename T1, typename T2, typename JsonType>
struct cast_traits_basic
{
    typedef cast_traits_basic<T2, T2, JsonType> nearby_class;

    static T1 cast ()                                        { return static_cast<T1>(nearby_class::cast());  }
    static T1 cast (typename JsonType::boolean_type v)       { return static_cast<T1>(nearby_class::cast(v)); }
    static T1 cast (typename JsonType::integer_type v)       { return static_cast<T1>(nearby_class::cast(v)); }
    static T1 cast (typename JsonType::real_type v)          { return static_cast<T1>(nearby_class::cast(v)); }
    static T1 cast (typename JsonType::string_type const & v){ return static_cast<T1>(nearby_class::cast(v)); }
    static T1 cast (typename JsonType::array_type const & v) { return static_cast<T1>(nearby_class::cast(v)); }
    static T1 cast (typename JsonType::object_type const & v){ return static_cast<T1>(nearby_class::cast(v)); }
};

//template <typename T, typename JsonType>
//struct cast_traits_basic<T, T, JsonType>
//{
//    static T cast ();
//    static T cast (typename JsonType::boolean_type);
//    static T cast (typename JsonType::integer_type);
//    static T cast (typename JsonType::real_type);
//    static T cast (typename JsonType::string_type const &);
//    static T cast (typename JsonType::array_type const &);
//    static T cast (typename JsonType::object_type const &);
//};

template <typename JsonType>
struct cast_traits_basic<bool, bool, JsonType>
{
    static bool cast ()
    {
        return false;
    }

    static bool cast (typename JsonType::boolean_type v)
    {
        return bool(v);
    }

    static bool cast (typename JsonType::integer_type v)
    {
        return v ? true : false;
    }

    static bool cast (typename JsonType::real_type v)
    {
        return v != typename JsonType::real_type(0.0f) ? true : false;
    }

    static bool cast (typename JsonType::string_type const & v)
    {
        return v == "true" ? true : false;
    }

    static bool cast (typename JsonType::array_type const & v)
    {
        return v.size() > 0 ? true : false;
    }

    static bool cast (typename JsonType::object_type const & v)
    {
        return v.size() > 0 ? true : false;
    }
};

template <typename JsonType>
struct cast_traits_basic<intmax_t, intmax_t, JsonType>
{
    static intmax_t cast ()
    {
        return 0;
    }

    static intmax_t cast (typename JsonType::boolean_type v)
    {
        return bool(v) ? 1 : 0;
    }

    static intmax_t cast (typename JsonType::integer_type v)
    {
        return intmax_t(v);
    }

    static intmax_t cast (typename JsonType::real_type v)
    {
        return intmax_t(v);
    }

    static intmax_t cast (typename JsonType::string_type const & v)
    {
        return pfs::lexical_cast<intmax_t>(v);
    }

    static intmax_t cast (typename JsonType::array_type const & v)
    {
        return intmax_t(v.size());
    }

    static intmax_t cast (typename JsonType::object_type const & v)
    {
        return intmax_t(v.size());
    }
};

template <typename JsonType>
struct cast_traits_basic<double, double, JsonType>
{
    static double cast ()
    {
        return .0f;
    }

    static double cast (typename JsonType::boolean_type v)
    {
        return bool(v) ? 1.0f : 0;
    }

    static double cast (typename JsonType::integer_type v)
    {
        return double(v);
    }

    static double cast (typename JsonType::real_type v)
    {
        return double(v);
    }

    static double cast (typename JsonType::string_type const & v)
    {
        return pfs::lexical_cast<double>(v);
    }

    static double cast (typename JsonType::array_type const & v)
    {
        return double(v.size());
    }

    static double cast (typename JsonType::object_type const & v)
    {
        return double(v.size());
    }
};

template <typename JsonType>
struct cast_traits_basic<typename JsonType::string_type
        , typename JsonType::string_type
        , JsonType>
{
    typedef typename JsonType::string_type string_type;

    static string_type cast ()
    {
        return string_type();
    }

    static string_type cast (typename JsonType::boolean_type v)
    {
        return to_string<string_type>(v);
    }

    static string_type cast (typename JsonType::integer_type v)
    {
        return to_string<string_type>(v);
    }

    static string_type cast (typename JsonType::real_type v)
    {
        return to_string<string_type>(v);
    }

    static string_type cast (typename JsonType::string_type const & v)
    {
        return v;
    }

    static string_type cast (typename JsonType::array_type const & /*v*/)
    {
        return string_type();
    }

    static string_type cast (typename JsonType::object_type const & /*v*/)
    {
        return string_type();
    }
};

template <typename T, typename JsonType>
struct cast_traits;

template <typename JsonType>
struct cast_traits<bool, JsonType>
    : public cast_traits_basic<bool, bool, JsonType> {};

template <typename JsonType>
struct cast_traits<char, JsonType>
    : public cast_traits_basic<char, intmax_t, JsonType> {};

template <typename JsonType>
struct cast_traits<signed char, JsonType>
    : public cast_traits_basic<signed char, intmax_t, JsonType> {};

template <typename JsonType>
struct cast_traits<unsigned char, JsonType>
    : public cast_traits_basic<unsigned char, intmax_t, JsonType> {};

template <typename JsonType>
struct cast_traits<short, JsonType>
    : public cast_traits_basic<short, intmax_t, JsonType> {};

template <typename JsonType>
struct cast_traits<unsigned short, JsonType>
    : public cast_traits_basic<unsigned short, intmax_t, JsonType> {};

template <typename JsonType>
struct cast_traits<int, JsonType>
    : public cast_traits_basic<int, intmax_t, JsonType> {};

template <typename JsonType>
struct cast_traits<unsigned int, JsonType>
    : public cast_traits_basic<unsigned int, intmax_t, JsonType> {};

template <typename JsonType>
struct cast_traits<long, JsonType>
    : public cast_traits_basic<long, intmax_t, JsonType> {};

template <typename JsonType>
struct cast_traits<unsigned long, JsonType>
    : public cast_traits_basic<unsigned long, intmax_t, JsonType> {};

#if PFS_HAVE_LONG_LONG

template <typename JsonType>
struct cast_traits<long long, JsonType>
    : public cast_traits_basic<long long, intmax_t, JsonType> {};

template <typename JsonType>
struct cast_traits<unsigned long long, JsonType>
    : public cast_traits_basic<unsigned long long, intmax_t, JsonType> {};

#endif

template <typename JsonType>
struct cast_traits<float, JsonType>
    : public cast_traits_basic<float, double, JsonType> {};

template <typename JsonType>
struct cast_traits<double, JsonType>
    : public cast_traits_basic<double, double, JsonType> {};

template <typename JsonType>
struct cast_traits<typename JsonType::string_type, JsonType>
    : public cast_traits_basic<typename JsonType::string_type
        , typename JsonType::string_type, JsonType> {};

}}} // pfs::json::details
