#pragma once
#include <cmath>
#include <pfs/types.hpp>
#include <pfs/cxxlang.hpp>

namespace pfs {

inline bool isnan (float x)
{
    return std::isnan(x);
}

inline bool isnan (double x)
{
    return std::isnan(x);
}

inline bool isinf (float x)
{
    return std::isinf(x);
}

inline bool isinf (double x)
{
    return std::isinf(x);
}

#if PFS_HAVE_LONG_DOUBLE

inline bool isnan (long double x)
{
    return std::isnan(x);
}

inline bool isinf (long double x)
{
    return std::isinf(x);
}

#endif

inline float pow (float x, float y)
{
    return ::powf(x, y);
}

inline double pow (double x, double y)
{
    return ::pow(x, y);
}

#if PFS_HAVE_LONG_DOUBLE

inline long double pow (long double x, long double y)
{
    return ::powl(x, y);
}

#endif

inline int abs (int x)
{
    return std::abs(x);
}

inline long int abs (long int x)
{
    return std::abs(x);
}

#if PFS_HAVE_LONG_LONG
inline long long int abs (long long int x)
{
    return std::abs(x);
}
#endif

inline double abs (double x)
{
#if __cplusplus >= 201103L
    return std::fabs(x);
#else
    return ::fabs(x);
#endif
}

inline float abs (float x)
{
#if __cplusplus >= 201103L
    return std::fabs(x);
#else
    return ::fabsf(x);
#endif
}

#if PFS_HAVE_LONG_DOUBLE
inline long double abs (long double x)
{
#if __cplusplus >= 201103L
    return std::fabs(x);
#else
    return ::fabsl(x);
#endif
}
#endif

inline float round (float x)
{
#if __cplusplus >= 201103L
    return std::round(x);
#else
    return ::round(x);
#endif
}

inline double round (double x)
{
#if __cplusplus >= 201103L
    return std::round(x);
#else
    return ::round(x);
#endif
}

#if PFS_HAVE_LONG_DOUBLE
inline long double round (long double x)
{
#if __cplusplus >= 201103L
    return std::round(x);
#else
    return ::round(x);
#endif
}
#endif // PFS_HAVE_LONG_DOUBLE


inline float floor (float x)
{
    return std::floor(x);
}

inline double floor (double x)
{
    return std::floor(x);
}

#if PFS_HAVE_LONG_DOUBLE
inline long double floor (long double x)
{
    return std::floor(x);
}
#endif

inline float ceil (float x)
{
    return std::ceil(x);
}

inline double ceil (double x)
{
    return std::ceil(x);
}

#if PFS_HAVE_LONG_DOUBLE
inline long double ceil (long double x)
{
    return std::ceil(x);
}
#endif

//
// The idea and implementation is from:
// https://stackoverflow.com/questions/1903954/is-there-a-standard-sign-function-signum-sgn-in-c-c
//
template <typename T>
int sign_of (T value)
{
    // Avoid repeated construction
    T zero(0);
    return (zero < value) - (value < zero);
}

/**
 * @brief Calculates Greatest Common Divisor. Based on Knuth 4.5.2. algorithm B.
 */
template <typename IntT>
IntT gcd (IntT a, IntT b) noexcept
{
    // Avoid repeated construction
    IntT zero(0);

    if (b == zero) return a;

    if (a == zero) return b;

    if (a < zero) a = -a;
    if (b < zero) b = -b;

    int k = 0;

    while (!((a & 1) || (b & 1))) { //B1
        a >>= 1;
        b >>= 1;
        ++k;
    }

    IntT t = (a & 1) ? -b : a; //B2

    while (a != b) {
        while (t && ((t & 1) ^ 1)) t >>= 1;  //B3 & B4

        if (t < zero) b = -t; //B5
        else a = t;

        t = a - b;  //B6
    }

    return a << k;
}

/**
 * @brief Calculates Lowest (Least) Common Multiple.
 */

template <typename IntT>
inline IntT lcm (IntT a, IntT b) noexcept
{
    // Avoid repeated construction
    IntT zero(0);
    IntT g = gcd(a, b);
    return ( g != zero) ? (a / g * b) : zero;
}

} // pfs

namespace pfs {
namespace math {

// Integer Division (Floor function)
// http://mathworld.wolfram.com/IntegerDivision.html
// Floor function symbols: |_a/b_| or a\b
//
template <typename IntT1, typename IntT2>
inline IntT1 floor_div (IntT1 a, IntT2 b)
{
    // Avoid repeated construction
    IntT1 zero(0);
    return (a - (a < zero ? b - IntT2(1) : zero)) / b;
}

//template <intmax_t Pn>
//struct integral_sign
//    : integral_constant<intmax_t, (Pn < 0) ? -1 : 1>
//{ };

namespace details {

inline int integral_sign (intmax_t Pn)
{
    return (Pn < 0) ? -1 : 1;
}

inline intmax_t integral_abs (intmax_t Pn)
{
    return Pn * integral_sign(Pn);
}

inline intmax_t integral_gcd (intmax_t IntegralConstant1, intmax_t IntegralConstant2)
{
    if (IntegralConstant2 == 0)
        return integral_abs(IntegralConstant1);
    else if (IntegralConstant1 == 0)
        return integral_abs(IntegralConstant2);

    return integral_gcd(IntegralConstant2, IntegralConstant1 % IntegralConstant2);
}

} // details

template <intmax_t IntegralConstant>
struct integral_sign
{
    static int const value;
};

template <intmax_t IntegralConstant>
int const integral_sign<IntegralConstant>::value = details::integral_sign(IntegralConstant);

template <intmax_t IntegralConstant>
struct integral_abs
{
    static intmax_t const value;
};

template <intmax_t IntegralConstant>
intmax_t const integral_abs<IntegralConstant>::value =  details::integral_abs(IntegralConstant);

template <intmax_t IntegralConstant1, intmax_t IntegralConstant2>
struct integral_gcd
{
    static intmax_t const value;
};

template <intmax_t IntegralConstant1, intmax_t IntegralConstant2>
intmax_t const integral_gcd<IntegralConstant1, IntegralConstant2>::value
        = details::integral_gcd(IntegralConstant1, IntegralConstant2);

}} // pfs::math
