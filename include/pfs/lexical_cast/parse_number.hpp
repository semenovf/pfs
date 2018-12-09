#pragma once
#include <pfs/assert.hpp>
#include <pfs/types.hpp>
#include <pfs/limits.hpp>
#include <pfs/ctype.hpp>
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
 * [3] real = [sign] [integral-part] POINT fract-part [exp-part]
 *          / [sign] integral-part POINT [exp-part]
 *          / [sign] integral-part exp-part
 *          / [sign] (N | n ) (A | a ) (N | n )
 *          / [sign] (I | i ) (N | n ) (F | f ) [(I | i ) (N | n ) (I | i ) (T | t ) (Y | y )]
 *
 * [4] rational = integral-part
 *          /  integral-part POINT [ fract-part ]
 *          /  numinator *ws '/' *ws denominator
 *
 * integral-part = *ws [ sign ] NON_ZERO_DIGIT *DIGIT
 *
 * fract-part    = DIGIT+
 *
 * numinator     = *ws [ sign ] NON_ZERO_DIGIT *DIGIT
 *
 * denominator   = NON_ZERO_DIGIT *DIGIT
 *
 * sign = '+' / '-'
 *
 * POINT = '.' ; or user-defined
 *
 * NON_ZERO_DIGIT = "1" / "2" / ... / "9"
 *
 * DIGIT          = "0" / 1" / "2" / ... / "9"
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

template <typename CharIt, typename IntT, template <typename> class Container>
CharIt parse_digits (CharIt first, CharIt last, int radix, Container<IntT> & digits)
{
    CharIt pos = first;

    for (; pos != last; ++pos) {
        int digit = parse_digit<CharIt>(pos, radix);

        if (digit < 0)
            return pos;

        digits.push_back(static_cast<IntT>(digit));
    }
}

template <typename CharIt>
class digits_parser
{
    typedef typename iterator_traits<CharIt>::value_type value_type;

    CharIt _badpos;        // pointer to stores end-of-parse position
    int    _sign;          // stores sign value (-1 or +1)
    int    _leading_zeros; // number of leading zeros
    pfs::vector<uint8_t> _digits;

private:
    void clear ()
    {
        _sign = 1;
        _leading_zeros = 0;
        _digits.clear();
    }

public:
    digits_parser ()
        : _sign(1)
        , _leading_zeros(0)
    {}

    pfs::vector<uint8_t> const & digits () const
    {
        return _digits;
    }

    bool parse (CharIt first, CharIt last, int radix = 10);
};

/**
 * @param first Char sequence begin position
 * @param last  Char sequence end position
 * @param radix Radix (must be between 2 and 36 inclusive)
 */
template <typename CharIt>
bool digits_parser<CharIt>::parse (
          CharIt first
        , CharIt last
        , int radix)
{
    PFS_ASSERT_X(radix >= 2 && radix <= 36, "radix must be in range [2, 36]");

    clear();

    _badpos = first;

    if (first == last)
        return false;

    CharIt pos = first;

    // Finish

    // Skip white spaces
    while (pos != last && pfs::is_space(*pos))
        ++pos;

    if (pos == last)
        return false;

    value_type c = *pos;

    if (c == value_type('+') || c == value_type('-')) {
        ++pos;

        // Finish
        if (pos == last)
            return false;

        if (c == value_type('-'))
            _sign = -1;
    }

    while (*pos == value_type('0')) {
        ++_leading_zeros;
        ++pos;
    }

    for (; pos != last; ++pos) {
        int digit = parse_digit<CharIt>(pos, radix);

        if (digit < 0) {
            _badpos = pos;
            return false;
        }

        _digits.push_back(static_cast<uint8_t>(digit));
    }

    if (_digits.empty() && _leading_zeros < 1)
        return false;

    _badpos = pos;

    return true;
}

} // pfs


