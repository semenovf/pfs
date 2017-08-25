/* 
 * File:   char.hpp
 * Author: wladt
 *
 * Created on March 30, 2017, 3:23 PM
 */

#ifndef __PFS_UNICODE_CHAR_HPP__
#define __PFS_UNICODE_CHAR_HPP__

#include <pfs/types.hpp>
#include <pfs/ctype.hpp>

namespace pfs {
namespace unicode {

enum
{
      max_code_point = 0x0010ffff
    , null_code_point = 0x00000000
    , default_replacement_char = 0x0000FFFD
    , max_bmp = 0x0000FFFF
    , bom_charstatic = 0x0000FEFF
    , hi_surrogate_start = 0x0000D800
    , hi_surrogate_end = 0x0000DBFF
    , low_surrogate_start = 0x0000DC00
    , low_surrogate_end = 0x0000DFFF
};


template <typename T1, typename T2>
inline T1 code_point_cast (T2 x)
{
    return static_cast<T1>(x);
}

struct char_t
{
    static uint32_t const replacement_char = default_replacement_char;
    typedef uint32_t value_type;

    value_type value;

    char_t ()
        : value (0)
    {}

    char_t (char_t const & other)
        : value (other.value)
    {}

    char_t (intmax_t v)
    {
        value = (v >= 0 && v <= max_code_point
                ? static_cast<value_type> (v)
                : replacement_char);
    }

    char_t & operator = (intmax_t v)
    {
        *this = char_t(v);
        return *this;
    }

    friend bool operator == (char_t const & lhs, char_t const & rhs)
    {
        return lhs.value == rhs.value;
    }

    friend bool operator != (char_t const & lhs, char_t const & rhs)
    {
        return lhs.value != rhs.value;
    }

    friend bool operator < (char_t const & lhs, char_t const & rhs)
    {
        return lhs.value < rhs.value;
    }

    friend bool operator <= (char_t const & lhs, char_t const & rhs)
    {
        return lhs.value <= rhs.value;
    }

    friend bool operator > (char_t const & lhs, char_t const & rhs)
    {
        return lhs.value > rhs.value;
    }

    friend bool operator>= (char_t const & lhs, char_t const & rhs)
    {
        return lhs.value >= rhs.value;
    }

    friend bool operator == (char_t const & lhs, char v)
    {
        return lhs.value == char_t(v).value;
    }

    friend bool operator != (char_t const & lhs, char v)
    {
        return lhs.value != char_t(v).value;
    }
};

/**
 * @brief Checks if unicode code point belongs
 *        to Low surrogate range [DC00-DFFF] (used for utf-16 character encoding).
 * @param c Unicode code point.
 * @return @c true if @c c belongs to Hi surrogate range.
 */
inline bool is_low_surrogate (char_t c)
{
	return (c.value & 0xfffffc00) == 0xdc00;
}

/**
 * @brief Checks if unicode code point belongs
 *        to Hi surrogate range [D800-DBFF] (used for utf-16 character encoding).
 * @param c Unicode code point.
 * @return @c true if @c c belongs to Hi surrogate range.
 */
inline bool is_hi_surrogate (char_t c)
{
	return (c.value & 0xfffffc00) == 0xd800;
}

/**
 * @brief Checks if unicode code point belongs
 *        to Hi or Low surrogate ranges (used for utf-16 character encoding).
 * @param c Unicode code point.
 * @return @c true if @c c belongs to Hi or Low surrogate ranges.
 */
inline bool is_surrogate (char_t c)
{
	return (c.value - hi_surrogate_start < 2048u);
}

#if __OBSOLETE__
/**
 * @return @c true if @a c is decimal digit character.
 * @note Decimal digits are any of: 0 1 2 3 4 5 6 7 8 9.
 */
inline bool is_decimal_digit (char_t c)
{
	return c.value >= char_t('0').value && c.value <= char_t('9').value;
}

inline bool is_space (char_t c)
{
	return c.value <= 127 && isspace(static_cast<char>(c.value));
}
#endif

}} // pfs::unicode

namespace pfs {

// TODO Need to support locale

template <>
inline bool is_alnum<unicode::char_t> (unicode::char_t c)
{
    return c.value <= 127 && is_alnum(static_cast<char>(c.value));
}

template <>
inline bool is_alpha<unicode::char_t> (unicode::char_t c)
{
    return c.value <= 127 && is_alpha(static_cast<char>(c.value));
}

template <>
inline bool is_cntrl<unicode::char_t> (unicode::char_t c)
{
    return c.value <= 127 && is_cntrl(static_cast<char>(c.value));
}

template <>
inline bool is_digit<unicode::char_t>(unicode::char_t c)
{
    return c.value <= 127 && is_digit(static_cast<char>(c.value));
}

template <>
inline bool is_graph<unicode::char_t> (unicode::char_t c)
{
    return c.value <= 127 && is_graph(static_cast<char>(c.value));
}

template <>
inline bool is_lower<unicode::char_t> (unicode::char_t c)
{
    return c.value <= 127 && is_lower(static_cast<char>(c.value));
}

template <>
inline bool is_print<unicode::char_t> (unicode::char_t c)
{
    return c.value <= 127 && is_print(static_cast<char>(c.value));
}

template <>
inline bool is_punct<unicode::char_t> (unicode::char_t c)
{
    return c.value <= 127 && is_punct(static_cast<char>(c.value));
}

template <>
inline bool is_space<unicode::char_t> (unicode::char_t c)
{
    return c.value <= 127 && is_space(static_cast<char>(c.value));
}

template <>
inline bool is_upper<unicode::char_t> (unicode::char_t c)
{
    return c.value <= 127 && is_upper(static_cast<char>(c.value));
}

template <>
inline bool is_xdigit<unicode::char_t> (unicode::char_t c)
{
    return c.value <= 127 && is_xdigit(static_cast<char>(c.value));
}

template <>
inline bool is_ascii<unicode::char_t> (unicode::char_t c)
{
    return c.value <= 127;
}

template <>
inline char to_ascii<unicode::char_t> (unicode::char_t c)
{
    return c.value <= 127 
            ? static_cast<char>(c.value)
            : -1;
}

} // pfs

#endif /* __PFS_UNICODE_CHAR_HPP__ */

