/* 
 * File:   lexical_cast.hpp
 * Author: wladt
 *
 * Created on March 15, 2017, 5:29 PM
 */

#ifndef __PFS_LEXICAL_CAST_STRTOINT_HPP__
#define __PFS_LEXICAL_CAST_STRTOINT_HPP__

//#include <string>
//#include <pfs/exception.hpp>
//#include <pfs/type_traits.hpp>
//#include <pfs/traits/string.hpp>
#include <pfs/limits.hpp>
#include <pfs/types.hpp>

#include <pfs/compiler.hpp>
#include PFS_CC_HEADER(ctype)

namespace pfs {

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
uintmax_t __string_to_uintmax (CharIteratorT beginpos
    , CharIteratorT endpos
    , CharIteratorT * badpos
    , int radix
    , int * psign
    , int * poverflow)
{
    typedef typename iterator_traits<CharIteratorT>::value_type value_type;
    
    CharIteratorT pos = beginpos;
    uintmax_t result = 0;
    bool ok   = true;
    bool over = false;
    int sign  = 1;
    
    if (poverflow)
        *poverflow = 0;
    
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
            CharIteratorT nextpos = pos;
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
                digit = c - value_type('0');
            else if (pfs::is_lower(c))
                digit = c - value_type('a') - 10;
            else if (pfs::is_upper(c))
                digit = c - value_type('A') - 10;
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

    if (over && poverflow) {
        if (sign < 0) {
            *poverflow = -1;
        } else {
            *poverflow = 1;
        }
    }
    
    if (psign)
        *psign = sign;
    
    return result;
}

template <typename CharIteratorT>
uintmax_t string_to_uintmax (CharIteratorT beginpos
    , CharIteratorT endpos
    , CharIteratorT * badpos
    , int radix
    , int * poverflow)
{
    int sign = 0;
    int overflow = 0;
    
    uintmax_t result = __string_to_uintmax(beginpos
            , endpos
            , badpos
            , radix
            , & sign
            , & overflow);
    
    if (poverflow)
        *poverflow = overflow;

    if (sign < 0)
        result *= sign;
    
    return result;    
}

template <typename CharIteratorT>
intmax_t string_to_intmax (CharIteratorT beginpos
    , CharIteratorT endpos
    , CharIteratorT * badpos
    , int radix
    , int * poverflow)
{
    int sign = 0;
    int overflow = 0;
    
    intmax_t result = 0;
    
    uintmax_t unsigned_result = __string_to_uintmax(beginpos
            , endpos
            , badpos
            , radix
            , & sign
            , & overflow);

    uintmax_t abs_min = static_cast<uintmax_t>(-1 * (numeric_limits<intmax_t>::min() + 1) + 1);
    uintmax_t abs_max = static_cast<uintmax_t>(numeric_limits<intmax_t>::max());

    if (sign < 0) {
        if (unsigned_result >= abs_min) {
            result = static_cast<intmax_t>(unsigned_result);
        } else {
            result = static_cast<intmax_t>(unsigned_result);
        }
    } else {
        
    }
    
    if (sign > 0 && unsigned_result >= abs_min) {
        result = static_cast<intmax_t>(unsigned_result);
    } else if (sign < 0 && unsigned_result <= abs_max) {
        result = -1 * static_cast<intmax_t>(unsigned_result);
    }
    
    if (! overflow) {
        
        if (sign > 0 && unsigned_result >= abs_min) {
            result
        }
        
        
        if (radix != 10 && sign > 0 
                && unsigned_result >= max_abs 
                && unsigned_result <= numeric_limits<uintmax_t>::max()) {
            result = -1 * unsigned_result;
        } else if (sign > 0 && unsigned_result > numeric_limits<intmax_t>::max()) {
            overflow = 1;
        } else if (sign < 0 && unsigned_result > max_abs) {
            overflow = -1;
        }
    }
    
    if (overflow != 0) {
        unsigned_result /= radix;
        
        if (badpos && *badpos != beginpos)
            --*badpos;
    }

    if (sign < 0)
        result = -1 * unsigned_result;
    else
        result = static_cast<intmax_t>(unsigned_result);

    if (poverflow)
        *poverflow = overflow;

    return result;    
    
//    if (! overflow) {
//        uintmax_t max_abs = static_cast<uintmax_t>(numeric_limits<intmax_t>::max()) + 1;
//        
//        if (radix != 10 && sign > 0 
//                && unsigned_result >= max_abs 
//                && unsigned_result <= numeric_limits<uintmax_t>::max()) {
//            result = -1 * unsigned_result;
//        } else if (sign > 0 && unsigned_result > numeric_limits<intmax_t>::max()) {
//            overflow = 1;
//        } else if (sign < 0 && unsigned_result > max_abs) {
//            overflow = -1;
//        }
//    }
//    
//    if (overflow != 0) {
//        unsigned_result /= radix;
//        
//        if (badpos && *badpos != beginpos)
//            --*badpos;
//    }
//
//    if (sign < 0)
//        result = -1 * unsigned_result;
//    else
//        result = static_cast<intmax_t>(unsigned_result);
//
//    if (poverflow)
//        *poverflow = overflow;
//
//    return result;
}

} // pfs

#endif /* __PFS_LEXICAL_CAST_STRTOINT_HPP__ */

