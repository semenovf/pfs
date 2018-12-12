#pragma once
#include <pfs/cxxlang.hpp>
#include <pfs/type_traits.hpp>
#include <pfs/system_error.hpp>
#include <pfs/string.hpp>

/*
 * Grammars:
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
 * @return Base-@a radix Digit converted from character, or -1 if conversion
 *      is impossible.
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
 *        digits with @a radix in range from 2 to 36 inclusively.
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

template <typename CharIt>
bool parse_integral_part_helper (CharIt & pos
        , CharIt last
        , error_code & ec
        , int & radix
        , int & sign
        , bool & digits_found)
{
    typedef typename iterator_traits<CharIt>::value_type value_type;

    // Bad radix
    if (radix != 0 && (radix < 2 || radix > 36)) {
        ec = make_error_code(errc::invalid_argument);
        return false;
    }

    // Empty sequence
    if (pos == last) {
        ec = make_error_code(errc::invalid_argument);
        return false;
    }

    // Skip white spaces
    while (pos != last && pfs::is_space(*pos))
        ++pos;

    // No digits seen
    if (pos == last) {
        ec = make_error_code(errc::invalid_argument);
        return false;
    }

    value_type c = *pos;

    // Sign
    if (c == value_type('+') || c == value_type('-')) {
        ++pos;

        // No digits seen
        if (pos == last) {
            ec = make_error_code(errc::invalid_argument);
            return false;
        }

        if (c == value_type('-'))
            sign = -1;
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
                    ec = make_error_code(errc::invalid_argument);
                    return false;
                }
            }
        }
    }

    // Determine radix (continue)
    if (radix == 0 || radix == 8) {
        if (*pos == value_type('0')) {
            radix = 8;
            ++pos;
            digits_found = true;
        }
    }

    if (radix == 0)
        radix = 10;

    return true;
}

/**
 * @brief Parse integral part from the character sequence.
 * @param first Character sequence begin position.
 * @param last Character sequence end position.
 * @param endpos Stores the position of the first invalid character in *endpos.
 * @param radix Radix for the expected digits in the sequence. Must be zero
 *        (auto recognition) or in range [2, 36].
 * @param ec After parsing stores one of the code below:
 *      @arg zero if parsing of integral part is successfull;
 *      @arg @c invalid_argument if the sequence is empty or radix is out-of-range;
 *      @arg @c result_out_of_range if the sequence represents too big integer to
 *          fit in the specified integer type (underflow or overflow).
 *
 * @return The result of the conversion, unless one oth below errors occured
 *         while parsing:
 *      @arg if an underflow occurs, function returns
 *          @c pfs::numeric_limits<IntT>::min() and error code @a ec set to
 *          @c result_out_of_range;
 *      @arg if an overflow occurs, function returns
 *          @c pfs::numeric_limits<IntT>::max() and error code @a ec set to
 *          @c result_out_of_range;
 *      @arg if the given @a radix contains an unsupported value, function
 *          returns 0 and error code @a ec set to @c invalid_argument;
 *      @arg if the sequence is empty or no digits seen in the sequence,
 *          function returns 0 and error code @a ec set to @c invalid_argument.
 *      @arg if expected unsigned integer but negative sign ('-') found,
 *          function returns 0 and error code @a ec set to @c invalid_argument.
 */
template <typename IntT, typename CharIt>
IntT parse_integral_part (CharIt first
        , CharIt last
        , error_code & ec
        , CharIt * endpos
        , int radix)
{
    typedef typename iterator_traits<CharIt>::value_type value_type;

    IntT result = 0;
    int sign = 1;
    CharIt pos = first;
    ec.clear();
    bool digits_found = false;

    if (parse_integral_part_helper<CharIt>(pos
                , last
                , ec
                , radix
                , sign
                , digits_found)) {

        IntT cutoff_value = 0;
        IntT cutoff_limit = 0;

        //if (sign > 0) {
        if (is_unsigned<IntT>::value) {
            cutoff_value = numeric_limits<IntT>::max() / radix;
            cutoff_limit = numeric_limits<IntT>::max() % radix;
        } else {
            cutoff_value = numeric_limits<IntT>::min() / radix;
            cutoff_limit = numeric_limits<IntT>::min() % radix;

            cutoff_value *= -1;
            cutoff_limit *= -1;
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

            digits_found = true;
        }

        if (parse_digit<CharIt>(pos, radix) >= 0) {
            ec = make_error_code(errc::result_out_of_range);

            if (is_unsigned<IntT>::value) {
                result = numeric_limits<IntT>::max();
            } else {
                if (sign > 0)
                    result = numeric_limits<IntT>::max();
                else
                    result = numeric_limits<IntT>::min();
            }

            while (parse_digit<CharIt>(pos, radix) >= 0)
                ++pos;
        } else {
           result *= sign;
        }
    }

    if (endpos) {
        if (digits_found)
            *endpos = pos;
        else
            *endpos = first;
    }

    return result;
}

template <typename IntT, typename CharIt>
inline IntT to_integral (CharIt first
        , CharIt last
        , error_code & ec
        , CharIt * endpos
        , int radix)
{
    return parse_integral_part<IntT, CharIt>(first, last, ec, endpos, radix);
}

/**
 * @brief Interprets a signed integer value in the string str.
 *
 * @details Discards any whitespace characters (as identified by calling
 *      is_space()) until the first non-whitespace character is found,
 *      then takes as many characters as possible to form a valid @c base-n
 *      (where n=@a radix) integer number representation and converts them to an
 *      integer value. The valid integer value consists of the following parts:
 *      @arg (optional) plus or minus sign;
 *      @arg (optional) prefix (@b 0) indicating octal base (applies only when
 *          the @a radix is 8 or ​0​);
 *      @arg (optional) prefix (@b 0x or @b 0X) indicating hexadecimal base
 *          (applies only when the @a radix is 16 or ​0​);
 *      @arg a sequence of digits.
 *
 *      The set of valid values for @c radix is {0,2,3,...,36}. For bases
 *      larger than 10, valid digits include alphabetic characters, starting
 *      from Aa for base-11 integer, to Zz for base-36 integer. The case of the
 *      characters is ignored.
 *      If the value of radix is ​@c 0​, the numeric radix is auto-detected:
 *      if the prefix is @c 0, the base is octal, if the prefix is @b 0x
 *      or @b 0X, the base is hexadecimal, otherwise the base is decimal.
 *
 * @param first Start position of the sequence.
 * @param last End position of the sequence.
 * @param ec Reference to store error code.
 * @param str_end Address of iterator to store position past the last character
 *      interpreted. If @a str_end is NULL, it is ignored.
 * @param radix Base of the interpreted integer value.
 * @return Specified integer type value converted from string.
 */
template <typename IntT>
IntT to_integral (string::const_iterator first
        , string::const_iterator last
        , error_code & ec
        , string::const_iterator * str_end = 0, int radix = 10)
{
    string::const_iterator endpos;

    IntT result = parse_integral_part<IntT>(first, last, ec, & endpos, radix);

    if (str_end)
        *str_end = endpos;

    return result;
}

/**
 * @brief Interprets a signed integer value in the string str.
 *
 * @param str The string to convert
 * @param ec Reference to store error code.
 * @param str_end Address of iterator to store position past the last character
 *      interpreted. If @a str_end is NULL, it is ignored.
 * @param radix Base of the interpreted integer value.
 * @return Specified integer type value converted from string.
 */
template <typename IntT>
inline IntT to_integral (string const & str, error_code & ec
        , string::const_iterator * str_end = 0, int radix = 10)
{
    return to_integral<IntT>(str.cbegin(), str.cend(), ec, str_end, radix);
}

/**
 * @brief Interprets a signed integer value in the string str.
 *
 * @param str The string to convert
 * @param str_end Address of iterator to store position past the last character
 *      interpreted. If @a str_end is NULL, it is ignored.
 * @param radix Base of the interpreted integer value.
 * @return Specified integer type value converted from string.
 *
 * @throw pfs::invalid_argument if no conversion could be performed.
 * @throw pfs::out_of_range if the converted value would fall out of the
 *      range of the result type.
 */
template <typename IntT>
IntT to_integral (string const & str, string::const_iterator * str_end = 0, int radix = 10)
{
    string::const_iterator endpos;
    error_code ec;

    IntT result = parse_integral_part<IntT>(
            str.cbegin(), str.cend(), ec, & endpos, radix);

    if (str_end)
        *str_end = endpos;

    if (ec) {
        if (ec == make_error_code(errc::invalid_argument))
            throw invalid_argument();

        if (ec == make_error_code(errc::invalid_argument))
            throw out_of_range();
    }

    return result;
}

} // pfs

