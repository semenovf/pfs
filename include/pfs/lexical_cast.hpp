/* 
 * File:   lexical_cast.hpp
 * Author: wladt
 *
 * Created on March 15, 2017, 5:29 PM
 */

#ifndef __PFS_LEXICAL_CAST_HPP__
#define __PFS_LEXICAL_CAST_HPP__

#include <string>
#include <pfs/type_traits.hpp>
#include <pfs/unicode/unicode_iterator.hpp>
#include <pfs/lexical_cast/bad_lexical_cast.hpp>
#include <pfs/lexical_cast/strtoint.hpp>
//#include <pfs/lexical_cast/strtoreal.hpp>


//#include <pfs/traits/string.hpp>
//#include <pfs/limits.hpp>
//#include <pfs/types.hpp>

#include <pfs/compiler.hpp>
#include PFS_CC_HEADER(ctype)

namespace pfs {

template <typename StringT, typename Integer>
typename pfs::enable_if<pfs::is_integral<Integer>::value 
        && pfs::is_unsigned<Integer>::value, Integer>::type
lexical_cast (StringT const & s, int radix = 10)
{
    typedef typename unicode::unicode_iterator_traits<
        typename StringT::const_iterator>::iterator iterator;
    iterator badpos;
    int overflow = 0;
    int sign = 0;
    
    uintmax_t result = string_to_uintmax<iterator>(iterator(s.cbegin())
            , iterator(s.cend())
            , & badpos
            , radix
            , & sign
            , & overflow);
    
    if (badpos != iterator(s.cend()))
        throw bad_lexical_cast("lexical_cast(): bad cast from string to numeric");
    
    return result;
}

#if __FIXME__

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
        throw bad_lexical_cast("lexical_cast(): bad cast from string to numeric");
    
    return result;
}

template <typename StringT, typename Float>
typename pfs::enable_if<pfs::is_floating_point<Float>::value, Float>::type
lexical_cast (StringT const & s, typename StringT::value_type decimal_point = '.')
{
    typedef typename StringT::const_iterator const_iterator;
    const_iterator badpos;
    
    intmax_t result = string_to_real<const_iterator>(s.cbegin()
            , s.cend()
            , decimal_point
            , & badpos);
    
    if (badpos != s.cend())
        throw bad_lexical_cast("lexical_cast(): bad cast from string to numeric");
    
#if PFS_HAVE_LONG_DOUBLE
	if (! (fabsl(result) <= fabsl(numeric_limits<Float>::max())))
        throw bad_lexical_cast("lexical_cast(): bad cast from string to numeric");
#else
	if (! (fabs(r) <= fabs(max_value<Float>())))
		throw bad_lexical_cast("lexical_cast(): bad cast from string to numeric");
#endif

    return static_cast<Float>(result);
}

#endif

} // pfs

#endif /* __PFS_LEXICAL_CAST_HPP__ */

