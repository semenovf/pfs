#pragma once
#include <pfs/limits.hpp>
#include <pfs/types.hpp>
#include <pfs/ctype.hpp>
#include <pfs/iterator.hpp>
#include "exception.hpp"

namespace pfs {

//
// Grammar (case radix = 0)
//
// result =   *ws ["+" / "-"] NON_ZERO_DIGIT *DIGIT
//          / *ws ["+" / "-"] "0" "x" 1*HEXDIGIT
//          / *ws ["+" / "-"] "0" 1*OCTALDIGIT
// NON_ZERO_DIGIT = "1" / "2" / ... / "9"
// DIGIT          = "0" / 1" / "2" / ... / "9"
// HEXDIGIT       = "0" / 1" / ... / "9" / "A" / ... / "F" / "a" / ... / "f"
// OCTALDIGIT     = "0" / 1" / ... / "7"
// ws             = <whitespace>
//
// Grammar (case radix > 1)
//
// result =   *ws ["+" / "-"] NON_ZERO_DIGIT *DIGIT
//

template <typename CharIt>
int parse_digit (CharIt pos, int radix)
{
    typedef typename iterator_traits<CharIt>::value_type value_type;
    value_type c = *pos;
    int digit = 0;

    if (!pfs::is_print(c))
        return -1;

    if (pfs::is_digit(c))
        digit = pfs::to_ascii(c) - '0';
    else if (pfs::is_lower(c))
        digit = pfs::to_ascii(c) - 'a' + 10;
    else if (pfs::is_upper(c))
        digit = pfs::to_ascii(c) - 'A' + 10;
    else
        return -1;

    if (digit >= radix)
        return -1;

    return digit;
}

template <typename CharIt>
uintmax_t parse_integer (
      CharIt beginpos     // char sequence begin position
    , CharIt endpos       // char sequence end position;
    , CharIt * badpos     // pointer to stores end-of-parse position
    , int radix           // radix (must be between 2 and 36 inclusive,
                          //    or be the special value 0)
    , uintmax_t max_value // maximum value for number parsed from char sequence
    , int * psign)        // pointer to store sign value
{
    typedef typename iterator_traits<CharIt>::value_type value_type;

    CharIt pos = beginpos;
    uintmax_t result = 0;
    bool ok   = true;
    int sign  = 1;

    // Skip white spaces
    while (pos != endpos && pfs::is_space(*pos))
        ++pos;

    do {
        value_type c;

        // Finish
        if (pos == endpos) {
            ok = false;
            break;
        }

        c = *pos;

        if (c == value_type('+') || c == value_type('-')) {
            ++pos;

            // Finish
            if (pos == endpos) {
                ok = false;
                break;
            }

            if (c == value_type('-'))
                sign = -1;
        }

        // Determine radix
        //
        if (radix == 0 || radix == 16) {
            CharIt nextpos = pos;
            ++nextpos;

            if (nextpos != endpos) {
                if (*pos == value_type('0')
                        && (*nextpos == value_type('x')
                            || *nextpos == value_type('X'))) {
                    radix = 16;
                    ++pos;
                    ++pos;

                    // Finish
                    if (pos == endpos) {
                        ok = false;
                        break;
                    }
                }
            }
        }

        if (radix == 0) {
            if (*pos == value_type('0')) {
                radix = 8;
                ++pos;

                // Finish
                if (pos == endpos) {
                    ok = false;
                    break;
                }
            } else {
                radix = 10;
            }
        }

        uintmax_t cutoff_value = max_value / radix;
        uintmax_t cutoff_limit = max_value % radix;

        for (; ok && pos != endpos; ++pos) {
            int digit = parse_digit<CharIt>(pos, radix);

            // Finish
            if (digit < 0) {
                ok = false;
                break;
            }

            if (result < cutoff_value
                    || (result == cutoff_value && static_cast<uintmax_t>(digit) <= cutoff_limit)) {
                result *= static_cast<uintmax_t>(radix);
                result += digit;
            } else {
                ok = false;
                break;
            }
        }
    } while (ok);

    if (badpos != 0)
        *badpos = pos;

    if (psign)
        *psign = sign;

    return result;
}

/**
 * @brief Convert a character sequence to an unsigned integer.
 *
 * @param beginpos Begin position of the character sequence to parse.
 * @param endpos End position of the character sequence to parse.
 * @param badpos If @a badpos is not null pointer, @c string_to_uintmax()
 *               stores position after last accepted character in @c *badpos.
 * @param radix  Radix (must be between 2 and 36 inclusive, or be the special
 *               value 0)
 * @param ec     If error occurred @a ec keeps the code that reflects the relevant reason:
 *               @li @c lexical_cast_errc::bad_radix if radix is out of bounds;
 * @return @arg unsigned integer number in range [0, numeric_limits<uintmax_t>::max()]
 *              if parse is success;
 *         @arg 0 if sequence has leading minus sign (@a *badpos stores @a beginpos
 *              value in this case)
 */

//  * @throws pfs::invalid_argument if @a radix is not equal to 0 or
//  *         it is out of bounds of range [2,36].
//  *
//  * @note The string may begin with an arbitrary amount of white space
//  *       (as determined by pfs::is_space()) followed by a single
//  *       optional @c '+' or @c '-' sign.
//  *
//  * @note If base is zero or 16, the string may then include a "0x" prefix,
//  *       and the number will be read in base 16; otherwise, a zero base is
//  *       taken as 10 (decimal) unless the next character is '0', in which case
//  *       it is taken as 8 (octal).
//  */

template <typename UIntT, typename CharIt>
UIntT string_to_uint (CharIt beginpos
        , CharIt endpos
        , CharIt * badpos
        , int radix
        , error_code & ec)
{
    int sign = 0;

    CharIt tmppos = beginpos;

    if (badpos)
        *badpos = tmppos;

    if (radix != 0 && (radix < 2 || radix > 36)) {
        ec = pfs::make_error_code(lexical_cast_errc::bad_radix); // bad radix
        return 0;
    }

    uintmax_t result = parse_integer<CharIt>(beginpos
            , endpos
            , & tmppos
            , radix
            , pfs::numeric_limits<UIntT>::max()
            , & sign);

    if (sign < 0) {
        tmppos = beginpos;
        result = 0;
    }

    if (badpos)
        *badpos = tmppos;

    return static_cast<UIntT>(result);
}

template <typename IntT, typename CharIt>
IntT string_to_int (CharIt beginpos
        , CharIt endpos
        , CharIt * badpos
        , int radix
        , error_code & ec)
{
    typedef typename iterator_traits<CharIt>::value_type value_type;
    int sign = 0;

    CharIt tmppos = beginpos;

    if (badpos)
        *badpos = tmppos;

    if (radix != 0 && (radix < 2 || radix > 36)) {
        ec = pfs::make_error_code(lexical_cast_errc::bad_radix); // bad radix
        return 0;
    }

    uintmax_t uresult = parse_integer<CharIt>(beginpos
            , endpos
            , & tmppos
            , radix
            , pfs::numeric_limits<IntT>::max()
            , & sign);

    IntT result = static_cast<IntT>(uresult) * sign;

    //
    // Workaround in case when sequence contains numeric_limits<IntT>::min() value
    //
    if (sign < 0) {
        IntT cutoff_value = numeric_limits<IntT>::min() / radix;

        if (tmppos != endpos && result >= cutoff_value) {
            int digit = parse_digit<CharIt>(tmppos, radix);

            if (digit > 0) {
                if (cutoff_value * radix >= numeric_limits<IntT>::min() + digit) {
                    result = result * radix - digit;
                    ++tmppos;
                }
            }
        }
    }

    if (badpos)
        *badpos = tmppos;

    return result;
}

template <typename UIntT, typename CharIt>
UIntT string_to_uint (CharIt beginpos
    , CharIt endpos
    , CharIt * badpos
    , int radix = 10)
{
    error_code ec;

    UIntT result = string_to_uint<UIntT, CharIt>(beginpos
            , endpos, badpos, radix, ec);

    if (ec.value() != static_cast<int>(lexical_cast_errc::success))
        throw bad_lexical_cast(ec);

    return result;
}

template <typename IntT, typename CharIt>
IntT string_to_int (CharIt beginpos
    , CharIt endpos
    , CharIt * badpos
    , int radix = 10)
{
    error_code ec;

    IntT result = string_to_int<IntT, CharIt>(beginpos
            , endpos, badpos, radix, ec);

    if (ec.value() != static_cast<int>(lexical_cast_errc::success))
        throw bad_lexical_cast(ec);

    return result;
}

} // pfs
