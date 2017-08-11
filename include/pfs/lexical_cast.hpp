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

template <typename UintT, typename StringT>
typename pfs::enable_if<pfs::is_integral<UintT>::value 
        && pfs::is_unsigned<UintT>::value, UintT>::type
lexical_cast (StringT const & s, int radix = 10)
{
    typedef typename StringT::const_iterator iterator;
    iterator badpos;
    
    UintT result = string_to_uint<UintT, iterator>(s.cbegin()
            , s.cend()
            , & badpos
            , radix);
    
    if (badpos != s.cend())
        throw bad_lexical_cast(pfs::make_error_code(lexical_cast_errc::invalid_string));
    
    return result;
}

template <typename IntT, typename StringT>
typename pfs::enable_if<pfs::is_integral<IntT>::value 
        && pfs::is_signed<IntT>::value, IntT>::type
lexical_cast (StringT const & s, int radix = 10)
{
    typedef typename StringT::const_iterator iterator;
    iterator badpos;
    
    IntT result = string_to_int<IntT, iterator>(s.cbegin()
            , s.cend()
            , & badpos
            , radix);
    
    if (badpos != s.cend())
        throw bad_lexical_cast(pfs::make_error_code(lexical_cast_errc::invalid_string));
    
    return result;
}

template <typename RealType, typename StringT>
typename pfs::enable_if<pfs::is_floating_point<RealType>::value, RealType>::type
lexical_cast (StringT const & s, typename StringT::value_type decimal_point = '.')
{
    typedef typename StringT::const_iterator iterator;
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

