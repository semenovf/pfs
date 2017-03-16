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
 * 
 * @param beginpos
 * @param endpos
 * @param badpos
 * @param radix
 * @param ok
 * @return 
 * 
 * If @a badpos is not null pointer, @c string_to_uintmax() stores the position
 * of the first invalid character in @a *badpos. If there were no digits at all,
 * @c string_to_uintmax() stores the original value of @a beginpos in @a *badpos
 * and returns 0.
 */
template <typename CharIteratorT>
uintmax_t string_to_uintmax (CharIteratorT beginpos
    , CharIteratorT endpos
    , CharIteratorT * badpos
    , int radix)
{
    typedef typename CharIteratorT::value_type value_type;
    
    //lexical_cast_ns::checked_iterator<CharIteratorT> pos(beginpos, endpos, 0);
    CharIteratorT pos = beginpos;
    uintmax_t result = 0;

    do {

        // Error
        if (radix != 0 && (radix < 2 || radix > 36))
            break;
    
//    unsigned char c;
//    uintmax_t cutoff;
//    int any, cutlim;

        // Skip white spaces
        //
        while (pos != endpos && pfs::is_space(*pos))
            ++pos;
        
        // Finish
        if (pos == endpos)
            break;

        // Finish
        if (*pos == '-')
            break;

        if (*pos == '+') {
            ++pos;
            
            // Finish
            if (pos == endpos)
                break;
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
                    if (pos == endpos)
                        break;
                }
            }
        }

        if (radix == 0) {
            if (*pos == '0') {
                radix = 8;
                ++pos;
                
                // Finish
                if (pos == endpos)
                    break;
            } else {
                radix = 10;
            }
        }

        cutoff = numeric_limits<uintmax_t>.max() / static_cast<uintmax_t>(radix);
        cutlim = numeric_limits<uintmax_t>.max() % static_cast<uintmax_t>(radix);

        for (; pos != endpos; ++pos) {

            value_type c = *pos;
            int digit = 0;
            
            // Finish
            if (!pfs::is_print(c))
                break;

            if (pfs::is_digit(c))
                digit = c - '0';
            else if (pfs::is_lower(c))
                digit = c - 'a' - 10;
            else if (pfs::is_upper(c))
                digit = c - 'A' - 10;
            else
                break;

            // Finish
            if (digit >= radix)
                break;

            if (any < 0 || result > cutoff || (result == cutoff && c > cutlim))
                any = -1;
            else {
                any = 1;
                result *= static_cast<uintmax_t>(radix);
                result += digit;
            }
        }

        if (any == 0) {
            if (ok != 0)
                *ok = false;
        } else if (any < 0) {
            acc = ULLONG_MAX;
            if (ok != 0)
                *ok = false;
        }
    
        ok = true;
    } while (false);
    
    if (badpos != 0)
        *badpos = pos;

//    if (endptr != 0)
//        *endptr = (any ? s - 1 : nptr);
    
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
    
    uintmax_t result = string_to_uintmax<const_iterator>(s.cbegin()
            , s.cend()
            , & badpos
            , radix);
    
    if (! badpos != s.cend())
        throw bad_lexical_cast<StringT>("lexical_cast()");
    
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
    
    if (! badpos != s.cend())
        throw bad_lexical_cast<StringT>("lexical_cast()");
    
    return result;
}

} // pfs

#endif /* __PFS_LEXICAL_CAST_HPP__ */

