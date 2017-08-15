/* 
 * File:   lexical_cast.hpp
 * Author: wladt
 *
 * Created on March 15, 2017, 5:29 PM
 */

#ifndef __PFS_LEXICAL_CAST_HPP__
#define __PFS_LEXICAL_CAST_HPP__

#include <pfs/cxxlang.hpp>
#include <pfs/type_traits.hpp>
#include <pfs/ctype.hpp>
#include <pfs/unicode/unicode_iterator.hpp>
#include <pfs/lexical_cast/exception.hpp>
#include <pfs/lexical_cast/strtoint.hpp>
#include <pfs/lexical_cast/strtoreal.hpp>

namespace pfs {

template <typename UintType, typename StringType>
typename pfs::enable_if<pfs::is_integral<UintType>::value 
        && pfs::is_unsigned<UintType>::value, UintType>::type
lexical_cast (StringType const & s, int radix = 10)
{
    typedef typename StringType::const_iterator iterator;
    iterator badpos;
    
    UintType result = string_to_uint<UintType, iterator>(s.cbegin()
            , s.cend()
            , & badpos
            , radix);
    
    if (badpos != s.cend())
        throw bad_lexical_cast(pfs::make_error_code(lexical_cast_errc::invalid_string));
    
    return result;
}

template <typename IntType, typename StringType>
typename pfs::enable_if<pfs::is_integral<IntType>::value 
        && pfs::is_signed<IntType>::value, IntType>::type
lexical_cast (StringType const & s, int radix = 10)
{
    typedef typename StringType::const_iterator iterator;
    iterator badpos;
    
    IntType result = string_to_int<IntType, iterator>(s.cbegin()
            , s.cend()
            , & badpos
            , radix);
    
    if (badpos != s.cend())
        throw bad_lexical_cast(pfs::make_error_code(lexical_cast_errc::invalid_string));
    
    return result;
}

template <typename RealType, typename StringType>
typename pfs::enable_if<pfs::is_floating_point<RealType>::value, RealType>::type
lexical_cast (StringType const & s, typename StringType::value_type decimal_point = '.')
{
    typedef typename StringType::const_iterator iterator;
    iterator badpos;
    
    RealType result = string_to_real<RealType, iterator>(s.cbegin()
            , s.cend()
            , decimal_point
            , & badpos);
    
    if (badpos != s.cend())
        throw bad_lexical_cast(pfs::make_error_code(lexical_cast_errc::invalid_string));
    
    return static_cast<RealType>(result);
}

} // pfs

#endif /* __PFS_LEXICAL_CAST_HPP__ */

