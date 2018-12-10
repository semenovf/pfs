#pragma once
#include <pfs/assert.hpp>
#include <pfs/types.hpp>
#include <pfs/limits.hpp>
#include <pfs/ctype.hpp>
#include <pfs/system_error.hpp>
#include <pfs/iterator.hpp>
#include <pfs/vector.hpp>

// #include <pfs/math.hpp>
// #include "exception.hpp"
// #include "strtoint.hpp"

/* Grammars:
 *
 * [1] number = integer / real / rational
 *
 * [2] integer = integral-part
 *
 * [3] real = [integral-part] POINT fract-part [exp-part]
 *          / integral-part POINT [exp-part]
 *          / integral-part exp-part
 *          / [sign] (N | n ) (A | a ) (N | n )
 *          / [sign] (I | i ) (N | n ) (F | f ) [(I | i ) (N | n ) (I | i ) (T | t ) (Y | y )]
 *
 * [4] rational = integral-part
 *          /  integral-part POINT [ fract-part ]
 *          /  numinator *ws '/' *ws denominator
 *
 * integral-part = *ws [ sign ] +DIGIT
 *
 * fract-part    = +DIGIT
 *
 * numinator     = integral-part
 *
 * denominator   = +DIGIT
 *
 * sign = '+' / '-'
 *
 * POINT = '.' ; or user-defined
 *
 * NON_ZERO_DIGIT = "1" / "2" / ... / "9" / ...  ; depends on radix
 *
 * DIGIT          = "0" / 1" / "2" / ... / "9" / ... ; depends on radix
 *
 * ws             = <whitespace>
 *
 */

namespace pfs {

/**
 * @return
 */
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

/**
 * @brief Parses character sequence for identification characters representing
 *        digits with radix in range from 2 to 36 inclusively.
 *
 * @throw range_error if radix is out of range.
 */
template <typename CharIt, typename DigitType, template <typename> class Container>
CharIt parse_digits (CharIt first, CharIt last, int radix, Container<DigitType> & digits)
{
    if (radix < 2 || radix > 36)
        throw pfs::range_error("radix must be in the range [2, 36]");

    CharIt pos = first;

    for (; pos != last; ++pos) {
        int digit = parse_digit<CharIt>(pos, radix);

        if (digit < 0)
            return pos;

        digits.push_back(static_cast<DigitType>(digit));
    }

    return pos;
}

// integral-part = *ws [ sign ] +DIGIT

/**
 * @brief Parse integral part from the character sequence.
 * @param first Character sequence begin position.
 * @param last Character sequence end position.
 * @param endpos Stores the position of the first invalid character in *endpos.
 * @param radix Radix for the expected digits in the sequence. Must be zero
 *        (auto recognition) or in range [2, 36].
 * @param ec After parsing stores one of the code below:
 *      <ul>
 *      <li>zero if parsing of integral part is successfull;
 *      <li>@c invalid_argument if the sequence is empty or radix is out-of-range;
 *      <li>@c result_out_of_range if the sequence represents too big integer to
 *          fit in the specified integer type (underflow or overflow).
 *      </ul>
 *
 * @return The result of the conversion, unless one oth below errors occured
 *         while parsing:
 *         <ul>
 *         <li> if an underflow occurs, function returns
 *              @c pfs::numeric_limits<IntT>::min() and error code @a ec set to
 *              @c result_out_of_range;
 *         <li> if an overflow occurs, function returns
 *              @c pfs::numeric_limits<IntT>::max() and error code @a ec set to
 *              @c result_out_of_range;
 *         <li> if the given @a radix contains an unsupported value, function
 *              returns 0 and error code @a ec set to @c invalid_argument;
 *         <li> if the sequence is empty or no digits seen in the sequence,
 *              function returns 0 and error code @a ec set to @c invalid_argument.
 *         <li> if expected unsigned integer but negative sign ('-') found,
 *              function returns 0 and error code @a ec set to @c invalid_argument.
 *         </ul>
 */
template <typename IntT, typename CharIt>
IntT parse_integral_part (CharIt first
        , CharIt last
        , CharIt * endpos
        , int radix
        , error_code & ec)
{
    typedef typename iterator_traits<CharIt>::value_type value_type;

    IntT result = 0;
    int sign = 1;
    CharIt pos = first;
    ec.clear();

    do {
        // Bad radix
        if (radix != 0 && (radix < 2 || radix > 36)) {
            ec = make_error_code(errc::invalid_argument);
            break;
        }

        // Empty sequence
        if (first == last) {
            ec = make_error_code(errc::invalid_argument);
            break;
        }

        // Skip white spaces
        while (pos != last && pfs::is_space(*pos))
            ++pos;

        // No digits seen
        if (pos == last) {
            pos = first;
            ec = make_error_code(errc::invalid_argument);
            break;
        }

        value_type c = *pos;

        // Sign
        if (c == value_type('+') || c == value_type('-')) {
            ++pos;

            // No digits seen
            if (pos == last) {
                pos = first;
                ec = make_error_code(errc::invalid_argument);
                break;
            }

            if (c == value_type('-'))
                sign = -1;
        }

        // Expected unsigned integer
        if (sign < 0 && numeric_limits<IntT>::min() >= 0) {
            pos = first;
            ec = make_error_code(errc::invalid_argument);
            break;
        }

        // Determine radix
        if (radix == 0 || radix == 16) {
            CharIt nextpos = pos;
            ++nextpos;

            if (nextpos != last) {
                if (*pos == value_type('0')
                        && (*nextpos == value_type('x')
                            || *nextpos == value_type('X'))) {
                    radix = 16;
                    ++pos;
                    ++pos;

                    // No digits seen
                    if (pos == last) {
                        pos = first;
                        ec = make_error_code(errc::invalid_argument);
                        break;
                    }
                }
            }
        }

        // Determine radix (continue)
        if (radix == 0) {
            if (*pos == value_type('0')) {
                radix = 8;
                ++pos;

                // No digits seen
                if (pos == last) {
                    pos = first;
                    ec = make_error_code(errc::invalid_argument);
                    break;
                }
            } else {
                radix = 10;
            }
        }

        IntT cutoff_value = 0;
        IntT cutoff_limit = 0;

        if (sign > 0) {
            cutoff_value = numeric_limits<IntT>::max() / radix;
            cutoff_limit = numeric_limits<IntT>::max() % radix;
        } else {
            cutoff_value = -1 * numeric_limits<IntT>::min() / radix;
            cutoff_limit = -1 * numeric_limits<IntT>::min() % radix;
        }

        for (; pos != last; ++pos) {
            int digit = parse_digit<CharIt>(pos, radix);

            // Finish
            if (digit < 0)
                break;

            if (result < cutoff_value
                    || (result == cutoff_value && static_cast<uintmax_t>(digit) <= cutoff_limit)) {
                result *= radix;
                result += digit;
            } else {
                break;
            }
        }

        if (parse_digit<CharIt>(pos, radix) >= 0) {
            ec = make_error_code(errc::result_out_of_range);

            if (sign > 0)
                result = numeric_limits<IntT>::max();
            else
                result = numeric_limits<IntT>::min();

            while (parse_digit<CharIt>(pos, radix) >= 0)
                ++pos;
        }
    } while (false);

    if (endpos)
        *endpos = pos;

    return result * sign;
}

} // pfs


