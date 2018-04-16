#pragma once
#include <cmath>
#include <pfs/types.hpp>

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

inline double fabs (double x)
{
#if __cplusplus >= 201103L
    return std::fabs(x);
#else
    return ::fabs(x);
#endif
}
       
inline float fabs (float x)
{
#if __cplusplus >= 201103L
    return std::fabs(x);
#else
    return ::fabsf(x);
#endif
}

#if PFS_HAVE_LONG_DOUBLE
inline long double fabs (long double x)
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

} // pfs

namespace pfs {
namespace math {

// Integer Division (Floor function)
// http://mathworld.wolfram.com/IntegerDivision.html
// Floor function symbols: |_a/b_| or a\b
//
template <typename Integral1, typename Integral2>
inline Integral1 floor_div (Integral1 a, Integral2 b)
{
    return (a - (a < 0 ? b - Integral2(1) : 0)) / b;
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
