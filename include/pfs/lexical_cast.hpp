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

namespace lexical_cast_ns {
        
template <typename CharIteratorT>
class checked_iterator
{
public:
    typedef typename CharIteratorT::value_type value_type;
    
private:
    CharIteratorT & _pos;
    CharIteratorT & _endpos;
    value_type      _badval;
    
public:
    checked_iterator (CharIteratorT & beginpos
            , CharIteratorT & endpos
            , value_type badval = 0)
        : _pos(beginpos)
        , _endpos(endpos)
        , _badval(badval)
    {}
    
    checked_iterator & operator ++ () // prefix increment
	{
        if (_pos != _endpos)
            ++_pos;
        return this;
	}

    value_type operator * () const
    {
        return _pos == _endpos ? _badval : *_pos;
    }

    CharIteratorT & pos ()
    {
        return _pos;
    }
    
    value_type value () const
    {
        return _pos == _endpos ? _badval : *_pos;
    }

    value_type next_value ()
    {
        if (_pos != _endpos) {
            CharIteratorT p(_pos);
            ++p;
            return p == _endpos ? _badval : *p;
        }
        
        return _badval;
    }
    
    bool at_end () const
    {
        return _pos == _endpos;
    }
};

} // lexical_cast_ns

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
    , int radix
    , bool * ok)
{
    typedef typename CharIteratorT::value_type value_type;
    
    lexical_cast_ns::checked_iterator<CharIteratorT> p(beginpos, endpos, 0);
    uintmax_t result = 0;

    if (ok != 0)
        *ok = false;
    
    if (radix != 0 && radix < 2) {
        if (badpos != 0)
            *badpos = p.pos();

        return 0;
    }
    
//    unsigned char c;
    uintmax_t cutoff;
    int any, cutlim;

    // Skip white spaces
    //
    while (pfs::isspace(*p)) {
        ++p;
    }
    
    if (*p == '-') {
        if (badpos != 0)
            *badpos = p;
        
        return 0;
    } 
    
    if (*p == '+')
        ++p;
    
    // Determine radix
    //
    if (radix == 0 || radix == 16) {
        value_type next_value = p.next_value();
        
        if (*p == '0' && (next_value == 'x' || next_value == 'X')) {
            radix = 16;
            ++p;
            ++p;
        }
    }
    
    if (radix == 0) {
        if (*p == '0') {
            radix = 8;
            ++p;
        } else {
            radix = 10;
        }
    }
    
    cutoff = numeric_limits<uintmax_t>.max() / static_cast<uintmax_t>(radix);
    cutlim = numeric_limits<uintmax_t>.max() % static_cast<uintmax_t>(radix);
    
    for (result = 0, any = 0; pos != endpos; c = *s++) {
        if (!pfs::isascii(c))
            break;
        
        if (pfs::isdigit(c))
            c -= '0';
        else if (pfs::isalpha(c))
            c -= pfs::isupper(c) ? 'A' - 10 : 'a' - 10;
        else
            break;
        
        if (c >= radix)
            break;
        
        if (any < 0 || result > cutoff || (result == cutoff && c > cutlim))
            any = -1;
        else {
            any = 1;
            result *= static_cast<uintmax_t>(radix);
            result += c;
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
    
    if (endptr != 0)
        *endptr = (any ? s - 1 : nptr);
    
    return result;
}

template <typename CharIteratorT>
intmax_t string_to_intmax (CharIteratorT beginpos
    , CharIteratorT endpos
    , CharIteratorT * badpos
    , int radix
    , bool * ok)
{
    return 0;
}

template <typename StringT, typename Integer>
typename pfs::enable_if<pfs::is_integral<Integer>::value 
        && pfs::is_unsigned<Integer>::value, Integer>::type
lexical_cast (StringT const & s, int radix = 10)
{
    bool ok = true;
    uintmax_t result = string_to_uintmax<StringT>(s, radix, & ok);
    if (! ok)
        throw bad_lexical_cast<StringT>("lexical_cast()");
    return 0;//result;
}

template <typename StringT, typename Integer>
typename pfs::enable_if<pfs::is_integral<Integer>::value 
        && pfs::is_signed<Integer>::value, Integer>::type
lexical_cast (StringT const & s, int radix = 10)
{
    bool ok = true;
    intmax_t result = lexical_cast_intmax<StringT>(s, radix, & ok);
    if (! ok)
        throw bad_lexical_cast<StringT>("lexical_cast()");
    return 0;//result;
}

} // pfs

#endif /* __PFS_LEXICAL_CAST_HPP__ */

