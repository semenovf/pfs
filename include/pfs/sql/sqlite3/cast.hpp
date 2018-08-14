#pragma once
#include <pfs/lexical_cast.hpp>

namespace pfs {
namespace sql {
namespace sqlite3 {
namespace details {

template <typename T1, typename T2, typename StringT>
struct cast_traits_basic
{
    typedef cast_traits_basic<T2, T2, StringT> nearby_class;

    static T1 cast ()                  { return static_cast<T1>(nearby_class::cast());  }
    static T1 cast (bool v)            { return static_cast<T1>(nearby_class::cast(v)); }
    static T1 cast (intmax_t v)        { return static_cast<T1>(nearby_class::cast(v)); }
    static T1 cast (double  v)         { return static_cast<T1>(nearby_class::cast(v)); }
    static T1 cast (StringT const & v) { return static_cast<T1>(nearby_class::cast(v)); }
};

template <typename StringT>
struct cast_traits_basic<bool, bool, StringT>
{
    static bool cast ()
    {
        return false;
    }

    static bool cast (bool v)
    {
        return bool(v);
    }

    static bool cast (intmax_t v)
    {
        return v ? true : false;
    }

    static bool cast (double v)
    {
        return v != double(0.0f) ? true : false;
    }

    static bool cast (StringT const & v)
    {
        return (v == "true" || v == "yes") ? true : false;
    }
};

template <typename StringT>
struct cast_traits_basic<intmax_t, intmax_t, StringT>
{
    static intmax_t cast ()
    {
        return 0;
    }

    static intmax_t cast (bool v)
    {
        return bool(v) ? 1 : 0;
    }

    static intmax_t cast (intmax_t v)
    {
        return intmax_t(v);
    }

    static intmax_t cast (double v)
    {
        return intmax_t(v);
    }

    static intmax_t cast (StringT const & v)
    {
        return pfs::lexical_cast<intmax_t>(v);
    }
};

template <typename StringT>
struct cast_traits_basic<double, double, StringT>
{
    static double cast ()
    {
        return .0f;
    }

    static double cast (bool v)
    {
        return bool(v) ? 1.0f : 0;
    }

    static double cast (intmax_t v)
    {
        return double(v);
    }

    static double cast (double v)
    {
        return double(v);
    }

    static double cast (StringT const & v)
    {
        return pfs::lexical_cast<double>(v);
    }
};

template <typename StringT>
struct cast_traits_basic<StringT, StringT, StringT>
{
    static StringT cast ()
    {
        return StringT();
    }

    static StringT cast (bool v)
    {
        return to_string(v);
    }

    static StringT cast (intmax_t v)
    {
        return to_string(v);
    }

    static StringT cast (double v)
    {
        return to_string(v);
    }

    static StringT cast (StringT const & v)
    {
        return v;
    }
};

template <typename T, typename StringT>
struct cast_traits;

template <typename StringT>
struct cast_traits<bool, StringT>
    : public cast_traits_basic<bool, intmax_t, StringT> {};

template <typename StringT>
struct cast_traits<char, StringT>
    : public cast_traits_basic<char, intmax_t, StringT> {};

template <typename StringT>
struct cast_traits<signed char, StringT>
    : public cast_traits_basic<signed char, intmax_t, StringT> {};

template <typename StringT>
struct cast_traits<unsigned char, StringT>
    : public cast_traits_basic<unsigned char, intmax_t, StringT> {};

template <typename StringT>
struct cast_traits<short, StringT>
    : public cast_traits_basic<short, intmax_t, StringT> {};

template <typename StringT>
struct cast_traits<unsigned short, StringT>
    : public cast_traits_basic<unsigned short, intmax_t, StringT> {};

template <typename StringT>
struct cast_traits<int, StringT>
    : public cast_traits_basic<int, intmax_t, StringT> {};

template <typename StringT>
struct cast_traits<unsigned int, StringT>
    : public cast_traits_basic<unsigned int, intmax_t, StringT> {};

template <typename StringT>
struct cast_traits<long, StringT>
    : public cast_traits_basic<long, intmax_t, StringT> {};

template <typename StringT>
struct cast_traits<unsigned long, StringT>
    : public cast_traits_basic<unsigned long, intmax_t, StringT> {};

#if PFS_HAVE_LONG_LONG

template <typename StringT>
struct cast_traits<long long, StringT>
    : public cast_traits_basic<long long, intmax_t, StringT> {};

template <typename StringT>
struct cast_traits<unsigned long long, StringT>
    : public cast_traits_basic<unsigned long long, intmax_t, StringT> {};

#endif

template <typename StringT>
struct cast_traits<float, StringT>
    : public cast_traits_basic<float, double, StringT> {};

template <typename StringT>
struct cast_traits<double, StringT>
    : public cast_traits_basic<double, double, StringT> {};

template <typename StringT>
struct cast_traits<StringT, StringT>
    : public cast_traits_basic<StringT, StringT, StringT> {};

}}}} // pfs::sql::sqlite3::details

