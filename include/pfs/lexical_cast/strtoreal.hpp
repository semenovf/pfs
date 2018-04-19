#pragma once
#include <pfs/types.hpp>
#include <pfs/assert.hpp>
#include <pfs/math.hpp>
#include <pfs/lexical_cast/exception.hpp>

/* Grammar:
 *
 * double =   [sign] [integral-part] POINT fract-part [exp-part]
 *          / [sign] integral-part POINT [exp-part]
 *          / [sign] integral-part exp-part
 *          / [sign] (N | n ) (A | a ) (N | n )
 *          / [sign] (I | i ) (N | n ) (F | f ) [(I | i ) (N | n ) (I | i ) (T | t ) (Y | y )]
 *
 * integral-part = digit-seq
 *
 * fract-part    = digit-seq
 *
 * exp-part = exp-char [exp-sign] digit-seq
 *
 * exp-char = 'e' / 'E'
 *
 * sign = '+' / '-'
 *
 * exp-sign = '+' / '-'
 *
 * digit-seq = digit
 *              / digit-seq digit
 *
 */

//
// Hack to get locale dependent decimal point char (spied at stackoverflow.com)
//
// setlocale(LC_NUMERIC, "C");
// char fchars[10];
// sprintf(fchars, "%f", 0.0f);
// char decimalPoint = fchars[1];
//

namespace pfs {

enum parse_float_state_enum {
      parse_sign_state
    //, skip_zeros_state
    , parse_nan_state
    , parse_inf_state
    , parse_mantissa_state
    , parse_exp_state
    , finish_state
    //, nan_state
    //, infinity_state
};

//
// Table giving binary powers of 10. Entry is 10^2^i.Used to convert decimal
// exponents into floating-point numbers.
//
template <typename FloatT>
struct powersOf10;

template <>
struct powersOf10<float>
{
    static float * values ()
    {
        static float a[] = {
            10.
          , 100.
          , 1.0e4
          , 1.0e8
          , 1.0e16
          , 1.0e32
        };

        return a;
    }
};

template <>
struct powersOf10<double>
{
    static double * values ()
    {
        static double a[] = {
            10.
          , 100.
          , 1.0e4
          , 1.0e8
          , 1.0e16
          , 1.0e32
          , 1.0e64
          , 1.0e128
          , 1.0e256
        };

        return a;
    }
};

#ifdef PFS_HAVE_LONG_DOUBLE
template <>
struct powersOf10<long double>
{
    static long double * values ()
    {
        static long double a[] = {
            10.L
          , 100.L
          , 1.0e4L
          , 1.0e8L
          , 1.0e16L
          , 1.0e32L
          , 1.0e64L
          , 1.0e128L
          , 1.0e256L
          , 1.0e512L
          , 1.0e1024L
          , 1.0e2048L
          , 1.0e4096L
        };

        return a;
    }
};
#endif

namespace details {

//
// Emulates c-string iterator (pointer to char).
// Increments wrapped iterator if current position is not at the end
// and returns '\x0' at the end.
//
template <typename Iter>
struct char_iterator_wrapper
{
    typedef typename Iter::value_type value_type;

    Iter pos;
    Iter last;

    char_iterator_wrapper () {}

    char_iterator_wrapper (char_iterator_wrapper const & rhs)
        : pos(rhs.pos)
        , last(rhs.last)
    {}

    char_iterator_wrapper & operator = (char_iterator_wrapper const & rhs)
    {
        pos = rhs.pos;
        last = rhs.last;
        return *this;
    }

    char_iterator_wrapper (Iter const & p, Iter const & l)
        : pos(p)
        , last(l)
    {}

    value_type operator * () const
    {
        return pos == last ? value_type(0) : *pos;
    }

    void operator ++ () // prefix increment
    {
        if (pos != last)
            ++pos;
    }
};

} // details

template <typename RealType, typename CharIt>
RealType string_to_real (CharIt first
        , CharIt last
        , typename iterator_traits<CharIt>::value_type decimal_point_char
        , CharIt * badpos
        , error_code & ec) // badpos
{
    typedef typename iterator_traits<CharIt>::value_type value_type;
    static double frac1_powers[] = {1.0e0, 1.0e1, 1.0e2, 1.0e3, 1.0e4, 1.0e5
            , 1.0e6, 1.0e7, 1.0e8, 1.0e9};

    CharIt p(first);
    double r = 0.0f;
    int sign = 1;
    RealType dbl_exp = 1.0f;
    int exp_sign = 1;
    int exp = 0;
    int frac_exp = 0;
    int frac1 = 0, frac2 = 0;
    int frac1_power = 0;
    int mantissa_size = 0;
    int decimal_point_size = -1;
    details::char_iterator_wrapper<CharIt> pos(p, last);
    details::char_iterator_wrapper<CharIt> last_zero_pos;
    details::char_iterator_wrapper<CharIt> nan;
    details::char_iterator_wrapper<CharIt> inf;
    details::char_iterator_wrapper<CharIt> exp_pos;

    if (first == last) {
        if (badpos)
            *badpos = first;
        return RealType(0.0f);
    }

    // Skip over any leading whitespace
    //==========================================================================
    while (pfs::is_space(*pos))
        ++pos;

    // Parse sign
    //==========================================================================
    if (*pos == '-') {
        sign = -1;
        ++pos;
    } else if (*pos == '+') {
        ++pos;
    }

    // Parse NaN
    //==========================================================================
    nan = pos;

    if (*nan == 'N' || *nan == 'n') {
        ++nan;
        if (*nan == 'A' || *nan == 'a') {
            ++nan;
            if (*nan == 'N' || *nan == 'n') {
                ++nan;
                r = pfs::numeric_limits<RealType>::quiet_NaN();
                goto done;
            }
        }
    }

    // Parse +-Infinity
    //==========================================================================
    inf = pos;

    if (*inf == 'I' || *inf == 'i') {
        ++inf;
        if (*inf == 'N' || *inf == 'n') {
            ++inf;
            if (*inf == 'F' || *inf == 'f') {
                ++inf;
                if (*inf == 'I' || *inf == 'i') {
                    ++inf;
                    if (*inf == 'N' || *inf == 'n') {
                        ++inf;
                        if (*inf == 'I' || *inf == 'i') {
                            ++inf;
                            if (*inf == 'T' || *inf == 't') {
                                ++inf;
                                if (*inf == 'Y' || *inf == 'y') {
                                    ++inf;
                                    r = pfs::numeric_limits<RealType>::infinity();
                                    goto done;
                                }
                            }
                        }
                    }
                } else { // 'INF' string
                    r = pfs::numeric_limits<RealType>::infinity();
                    goto done;
                }
            }
        }
    }

    // Skip zeros
    //==========================================================================
    last_zero_pos = pos;

    while (*pos == '0') {
        last_zero_pos = pos;
        ++pos;
    }

    pos = last_zero_pos;

    // Parse mantissa
    //==========================================================================
    // For double type mantissa has 18 significant digits,
    // so extra digits could be considered zeros (they can't affect the
    // result value). And represent this 18 digits as two integer fractions

    for (value_type c = *pos
            ; c != value_type(0) && mantissa_size < 9
            ; ++pos, ++mantissa_size, c = *pos) {

        if (!is_digit(c)) {
            if (c == decimal_point_char) {
                if (decimal_point_size < 0) {
                    decimal_point_size = mantissa_size;
                    --mantissa_size;
                    continue;
                } else { // second decimal point
                    break;
                }
            } else {
                break;
            }
        }

        frac1 = 10 * frac1 + (to_ascii(c) - '0');
    }

    frac1_power = 0;

    for (value_type c = *pos
            ; c != value_type(0) && mantissa_size < 18
            ; ++pos, ++mantissa_size, c = *pos) {

        if (!is_digit(c)) {
            if (c == decimal_point_char) {
                if (decimal_point_size < 0) {
                    decimal_point_size = mantissa_size;
                    --mantissa_size;
                    continue;
                } else { // second decimal point
                    break;
                }
            } else {
                break;
            }
        }

        frac2 = 10 * frac2 + (to_ascii(c) - '0');
        ++frac1_power;
    }

    r = (frac1_powers[frac1_power] * frac1) + frac2;

    while (is_digit(*pos)) {
        ++mantissa_size;
        ++pos;
    }

    if (decimal_point_size < 0) {
        decimal_point_size = mantissa_size;
    }
//    } else {
//        --mantissa_size; // One of the digits was the point
//    }

    if (mantissa_size > 18) {
        frac_exp = decimal_point_size - 18;
        mantissa_size = 18;
    } else {
        frac_exp = decimal_point_size - mantissa_size;
    }

    if (mantissa_size == 0) {
        r = 0.0f;
        pos.pos = first;
        goto done;
    }

    // Parse exponent
    //==========================================================================

    if ((*pos == 'E') || (*pos == 'e')) {

        exp_pos = pos;

        ++pos;

        if (*pos == '-') {
            exp_sign = -1;
            ++pos;
        } else {
            if (*pos == '+') {
                ++pos;
            }
        }

        if (is_digit(*pos)) {
            while (is_digit(*pos)) {
                exp = exp * 10 + (to_ascii(*pos) - '0');
                ++pos;
            }
        } else {
            pos = exp_pos;
        }
    }

    if (exp_sign < 0) {
        exp = frac_exp - exp;
    } else {
        exp = frac_exp + exp;
    }

    // Generate a floating-point number that represents the exponent.
    // Do this by processing the exponent one bit at a time to combine
    // many powers of 2 of 10. Then combine the exponent with the
    // fraction.

    if (exp < 0) {
        exp_sign = -1;
        exp = -exp;
    } else {
        exp_sign = 1;
    }

    if (exp < pfs::numeric_limits<RealType>::min_exponent10) { // underflow
        ec = pfs::make_error_code(lexical_cast_errc::underflow);
        r = 0.0f;
        goto done;
    } else if (exp > pfs::numeric_limits<RealType>::max_exponent10) { // overflow
        if (sign < 0) {
            r = -pfs::numeric_limits<RealType>::infinity();
        } else {
            r = pfs::numeric_limits<RealType>::infinity();
        }

        ec = pfs::make_error_code(lexical_cast_errc::overflow);
        goto done;
    }

    for (RealType * d = powersOf10<RealType>::values(); exp != 0; exp >>= 1, d += 1) {
        if (exp & 01) {
            dbl_exp *= *d;
        }
    }

    if (exp_sign < 0) {
        r /= dbl_exp;
    } else {
        r *= dbl_exp;
    }

done:
    if (badpos)
        *badpos = pos.pos;

    return sign < 0 ? -r : r;
}

template <typename RealType, typename CharIt>
RealType string_to_real (CharIt first
        , CharIt last
        , typename iterator_traits<CharIt>::value_type decimal_point
        , CharIt * badpos)
{
    error_code ec;
    RealType result = string_to_real<RealType, CharIt>(first, last, decimal_point, badpos, ec);

    if (ec.value() != static_cast<int>(lexical_cast_errc::success))
        throw bad_lexical_cast(ec);

    return result;
}

} // pfs
