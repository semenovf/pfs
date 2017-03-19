/* 
 * File:   lexical_cast.hpp
 * Author: wladt
 *
 * Created on March 15, 2017, 5:29 PM
 */

#ifndef __PFS_LEXICAL_CAST_HPP__
#define __PFS_LEXICAL_CAST_HPP__

#include <string>
#include <pfs/traits/string.hpp>
#include <pfs/limits.hpp>
#include <pfs/types.hpp>

#include <pfs/compiler.hpp>
#include PFS_CC_HEADER(ctype)

namespace pfs {

template <typename StringT>
class bad_lexical_cast
{
    StringT _msg;
    
public:
    explicit bad_lexical_cast (StringT const & m)
        : _msg(m)
    {}

    explicit bad_lexical_cast (char const * s)
        : _msg(s)
    {}
};

//namespace lexical_cast_ns {
//        
//template <typename CharIteratorT>
//class checked_iterator
//{
//public:
//    typedef typename CharIteratorT::value_type value_type;
//    
//private:
//    CharIteratorT & _pos;
//    CharIteratorT & _endpos;
//    value_type      _badval;
//    
//public:
//    checked_iterator (CharIteratorT & beginpos
//            , CharIteratorT & endpos
//            , value_type badval = 0)
//        : _pos(beginpos)
//        , _endpos(endpos)
//        , _badval(badval)
//    {}
//    
//    checked_iterator & operator ++ () // prefix increment
//	{
//        if (_pos != _endpos)
//            ++_pos;
//        return this;
//	}
//
//    value_type operator * () const
//    {
//        return _pos == _endpos ? _badval : *_pos;
//    }
//
//    bool operator == (CharIteratorT)
//    
//    CharIteratorT & pos ()
//    {
//        return _pos;
//    }
//    
//    value_type value () const
//    {
//        return _pos == _endpos ? _badval : *_pos;
//    }
//
//    value_type next_value ()
//    {
//        if (_pos != _endpos) {
//            CharIteratorT p(_pos);
//            ++p;
//            return p == _endpos ? _badval : *p;
//        }
//        
//        return _badval;
//    }
//    
//    bool at_end () const
//    {
//        return _pos == _endpos;
//    }
//};
//
//} // lexical_cast_ns

//
// Grammar (case radix = 0)
// 
// result =   *ws ["+"] NON_ZERO_DIGIT *DIGIT
//          / *ws ["+"] "0" "x" 1*HEXDIGIT
//          / *ws ["+"] "0" 1*OCTALDIGIT
// NON_ZERO_DIGIT = "1" / "2" / ... / "9"
// DIGIT          = "0" / 1" / "2" / ... / "9"
// HEXDIGIT       = "0" / 1" / ... / "9" / "A" / ... / "F" / "a" / ... / "f"
// OCTALDIGIT     = "0" / 1" / ... / "7"
// ws             = <whitespace>
//
// Grammar (case radix > 1)
//
// result =   *ws ["+"] NON_ZERO_DIGIT *DIGIT
//

/**
 * @brief Convert a string to an unsigned integer.
 * 
 * @param beginpos Begin position of the string to parse.
 * @param endpos End position of the string to parse.
 * @param badpos If @a badpos is not null pointer, @c string_to_uintmax()
 *               stores the address of the first invalid character 
 *               in @c *badpos.
 * @param radix Radix (must be between 2 and 36 inclusive, or be the special
                value 0)
 * @param overflow If @a overflow is not null pointer, @c string_to_uintmax()
 *               stores the @c -1 if string started with @c '-' sign,
 *               @c 0 if no errors occurred and @c 1 if result is overflow.
 * @return @arg unsigned integer number in range [0, numeric_limits<uintmax_t>::max()]
 *         and @a *overflow set to 0 on success;
 *         @arg unsigned integer number in range [0, numeric_limits<uintmax_t>::max()]
 *         and @a *overflow set to -1 if character sequence begin with @c '-' sign;
 *         @arg @c numeric_limits<uintmax_t>::max() and @a *overflow set to 1 if 
 *         character sequence represented number greater than 
 *         @c numeric_limits<uintmax_t>::max();
 * 
 * @throws pfs::invalid_argument if @a radix is not equal to 0 or 
 *         is out of bounds of range [2,36].
 * 
 * @note The string may begin with an arbitrary amount of white space
 * (as determined by pfs::is_space()) followed by a single 
 * optional @c '+' or @c '-' sign.
 * 
 * @note If base is zero or 16, the string may then include a "0x" prefix, 
 * and the number will be read in base 16; otherwise, a zero base is 
 * taken as 10 (decimal) unless the next character is '0', in which case 
 * it is taken as 8 (octal).
 */
template <typename CharIteratorT>
uintmax_t string_to_uintmax (CharIteratorT beginpos
    , CharIteratorT endpos
    , CharIteratorT * badpos
    , int radix
    , int * overflow)
{
    typedef typename CharIteratorT::value_type value_type;
    
    CharIteratorT pos = beginpos;
    uintmax_t result = 0;
    bool ok   = true;
    bool over = false;
    int sign  = 1;
    
    if (overflow)
        *overflow = 0;
    
    if (radix != 0 && (radix < 2 || radix > 36))
        throw pfs::invalid_argument("string_to_uintmax(): bad radix");

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

        if (c == '+' || c == '-') {
            ++pos;
            
            // Finish
            if (pos == endpos) {
                ok = false;
                break;
            }
            
            if (c == '-')
                sign = -1;
        }

        // Determine radix
        //
        if (radix == 0 || radix == 16) {
            CharIteratorT nextpos = pos;
            ++nextpos;

            if (nextpos != endpos) {
                if (*pos == '0' && (*nextpos == 'x' || *nextpos == 'X')) {
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
            if (*pos == '0') {
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

        uintmax_t cutoff_value = numeric_limits<uintmax_t>::max() / static_cast<uintmax_t>(radix);
        uintmax_t cutoff_limit = numeric_limits<uintmax_t>::max() % static_cast<uintmax_t>(radix);

        for (; ok && pos != endpos; ++pos) {

            value_type c = *pos;
            int digit = 0;
            
            // Finish
            if (!pfs::is_print(c)) {
                ok = false;
                break;
            }

            if (pfs::is_digit(c))
                digit = c - '0';
            else if (pfs::is_lower(c))
                digit = c - 'a' - 10;
            else if (pfs::is_upper(c))
                digit = c - 'A' - 10;
            else
                break;

            // Finish
            if (digit >= radix) {
                ok = false;
                break;
            }

            if (result < cutoff_value
                    || (result == cutoff_value && digit <= cutoff_limit)) {
                result *= static_cast<uintmax_t>(radix);
                result += digit;
            } else {
                ok = false;
                over = true;
                break;
            }
        }
    } while (ok);
    
    if (badpos != 0)
        *badpos = pos;

    if (sign < 0)
        result *= sign;

    if (over && overflow) {
        if (sign < 0) {
            *overflow = -1;
        } else {
            *overflow = 1;
        }
    }
    
    return result;
}

template <typename CharIteratorT>
intmax_t string_to_intmax (CharIteratorT beginpos
    , CharIteratorT endpos
    , CharIteratorT * badpos
    , int radix)
{
    return 0;
}

template <typename StringT, typename Integer>
typename pfs::enable_if<pfs::is_integral<Integer>::value 
        && pfs::is_unsigned<Integer>::value, Integer>::type
lexical_cast (StringT const & s, int radix = 10)
{
    typedef typename StringT::const_iterator const_iterator;
    const_iterator badpos;
    int overflow = 0;
    
    uintmax_t result = string_to_uintmax<const_iterator>(s.cbegin()
            , s.cend()
            , & badpos
            , radix
            , & overflow);
    
    if (badpos != s.cend())
        throw bad_lexical_cast<StringT>("lexical_cast(): bad cast from string to numeric");
    
    return result;
}

template <typename StringT, typename Integer>
typename pfs::enable_if<pfs::is_integral<Integer>::value 
        && pfs::is_signed<Integer>::value, Integer>::type
lexical_cast (StringT const & s, int radix = 10)
{
    typedef typename StringT::const_iterator const_iterator;
    const_iterator badpos;
    
    intmax_t result = string_to_intmax<const_iterator>(s.cbegin()
            , s.cend()
            , & badpos
            , radix);
    
    if (badpos != s.cend())
        throw bad_lexical_cast<StringT>("lexical_cast(): bad cast from string to numeric");
    
    return result;
}

} // pfs

#endif /* __PFS_LEXICAL_CAST_HPP__ */

