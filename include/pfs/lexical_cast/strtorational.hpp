#pragma once
#include <pfs/types.hpp>
#include <pfs/assert.hpp>
#include <pfs/math.hpp>
#include "exception.hpp"
#include "strtoint.hpp"

/* Grammar:
 *
 * rational =  [integral-part] POINT fract-part
 *          /  integral-part [ POINT ]
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
 * POINT = '.'
 *
 * NON_ZERO_DIGIT = "1" / "2" / ... / "9"
 *
 * DIGIT          = "0" / 1" / "2" / ... / "9"
 *
 * ws             = <whitespace>
 *
 */

namespace pfs {

template <typename CharIt>
rational<uintmax_t> parse_rational (
      CharIt beginpos     // char sequence begin position
    , CharIt endpos       // char sequence end position
    , typename iterator_traits<CharIt>::value_type decimal_point_char
    , CharIt * badpos     // pointer to stores end-of-parse position
    , int * psign)
{
    CharIt pos = beginpos;
    int sign = 0;

    if (badpos)
        *badpos = pos;

    uintmax_t integral_part = parse_integer<CharIt>(beginpos
            , endpos
            , & pos
            , 10
            , pfs::numeric_limits<uintmax_t>::max()
            , & sign);

    return rational<uintmax_t>();
//
//     bool ok = false;
//
//     if (tmppos == beginpos) {
//         // Error
//     } else if (tmppos != endpos && is_space(*tmppos)) {
//         //
//         // Atempt to parse denominator
//         //
//         do {
//             context->is_num_denom = true;
//             context->numinator = integral_part;
//             context->denominator = 1;
//             context->sign = sign;
//
//             beginpos = tmppos;
//
//             while (tmppos != endpos && is_space(*tmppos))
//                 ++tmppos;
//
//             // Ok
//             if (tmppos != endpos && *tmppos != '/') {
//                 ok = true;
//                 break;
//             }
//
//             ++tmppos;
//
//             while (tmppos != endpos && is_space(*tmppos))
//                 ++tmppos;
//
//             // Error
//             if (tmppos == endpos)
//                 break;
//
//             int denom_sign = 0;
//             beginpos = tmppos;
//             uintmax_t denominator = parse_integer<CharIt>(beginpos
//                     , endpos
//                     , & tmppos
//                     , 10
//                     , pfs::numeric_limits<uintmax_t>::max()
//                     , & denom_sign);
//
//             // Error: denominator must be positive
//             if (denom_sign < 0)
//                 break;
//
//             // Error: denominator must be non-empty
//             if (beginpos == tmppos)
//                 break;
//
//             context->is_num_denom = true;
//             context->numinator = integral_part;
//             context->denominator = denominator;
//
//             ok = true;
//         } while (false);
//     } else {
//         context->is_num_denom = false;
//         context->integral_part = integral_part;
//         context->sign = sign;
//
//         //
//         // Attempt to parse fractional part
//         //
//         if (tmppos != endpos && *tmppos == decimal_point_char) {
//             ++tmppos;
//
//             uintmax_t numinator = 0;
//             uintmax_t denominator = 1;
//
//             // Leading zeros for fraction part
//             while (tmppos != endpos) {
//                 int digit = parse_digit<CharIt>(tmppos, 10);
//
//
//             }
//         }
//     }
// /*
//     if (tmppos != endpos && *tmppos == decimal_point_char) {
//
//         ++tmppos;
//
//         if (tmppos != endpos) {
//             int fract_sign = 0;
//             beginpos = tmppos;
//             uintmax_t fract_part = parse_integer<CharIt>(beginpos
//                     , endpos
//                     , & tmppos
//                     , 10
//                     , pfs::numeric_limits<uintmax_t>::max()
//                     , & fract_sign);
//
//             if (fract_sign > 0) {
//                 context->fract_part = fract_part;
//                 fail = false;
//             }
//         }
//     }
//
//     if (fail) {
//         if (tmppos != endpos) {
//             while (is_space(*tmppos))
//                 ++tmppos;
//         }
//
//         if (tmppos != endpos && *tmppos == '/') {
//             while (is_space(*tmppos))
//                 ++tmppos;
//             if (tmppos != endpos) {
//             }
//         }
//     }*/
//
//     if (badpos)
//         *badpos = tmppos;
//
//     return ok;
}


// template <typename RationalT, typename CharIt>
// RationalT string_to_unsigned_rational (CharIt beginpos
//         , CharIt endpos
//         , typename iterator_traits<CharIt>::value_type decimal_point_char
//         , CharIt * badpos
//         , error_code &)
// {
//     typedef typename RationalT::int_type uint_type;
//     int sign = 0;
//
//     CharIt tmppos = beginpos;
//
//     if (badpos)
//         *badpos = tmppos;
//
//     uintmax_t integral_part = parse_integer<CharIt>(beginpos
//             , endpos
//             , & tmppos
//             , 10
//             , pfs::numeric_limits<uint_type>::max()
//             , & sign);
//
//     // Error: expected unsigned number
//     if (sign < 0) {
//         tmppos = beginpos;
//         integral_part = 0;
//     } else {
//         if (tmppos != endpos) {
//             if (*tmppos == decimal_point_char) {
//                 ++tmppos;
//             }
//         }
//     }
//
//     if (badpos)
//         *badpos = tmppos;
//
//     return static_cast<UIntT>(result);
// }
//
//
// template <typename RationalT, typename CharIt>
// RationalT string_to_rational (CharIt beginpos
//         , CharIt endpos
//         , CharIt * badpos
//         , error_code & ec)
// {
//     typedef typename RationalT::int_type int_type;
//     typedef typename iterator_traits<CharIt>::value_type value_type;
//     int sign = 0;
//
//     CharIt tmppos = beginpos;
//
//     if (badpos)
//         *badpos = tmppos;
//
//     uintmax_t uresult = parse_integer<CharIt>(beginpos
//             , endpos
//             , & tmppos
//             , 10
//             , pfs::numeric_limits<int_type>::max()
//             , & sign);
//
//     IntT result = static_cast<IntT>(uresult) * sign;
//
//     //
//     // Workaround in case when sequence contains numeric_limits<IntT>::min() value
//     //
//     if (sign < 0) {
//         IntT cutoff_value = numeric_limits<IntT>::min() / radix;
//
//         if (tmppos != endpos && result >= cutoff_value) {
//             int digit = parse_digit<CharIt>(tmppos, radix);
//
//             if (digit > 0) {
//                 if (cutoff_value * radix >= numeric_limits<IntT>::min() + digit) {
//                     result = result * radix - digit;
//                     ++tmppos;
//                 }
//             }
//         }
//     }
//
//     if (badpos)
//         *badpos = tmppos;
//
//     return result;
// }

} // pfs

