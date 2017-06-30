/*
 * strtoreal.cpp
 *
 *  Created on: Jan 2, 2016
 *      Author: wladt
 */

//#include "pfs/string.hpp"
//#include "pfs/unicode.hpp"

#include <pfs/types.hpp>
#include <pfs/assert.hpp>
#include <pfs/math.hpp>
#include <pfs/lexical_cast/exception.hpp>

/* Grammar:
 *
 * double =   [sign] [integral-part] POINT fract-part [exp-part]
 * 			/ [sign] integral-part POINT [exp-part]
 *			/ [sign] integral-part exp-part
 *			/ [sign] (N | n ) (A | a ) (N | n )
 *			/ [sign] (I | i ) (N | n ) (F | f ) [(I | i ) (N | n ) (I | i ) (T | t ) (Y | y )]
 *
 * integral-part = digit-seq
 *
 * fract-part    = digit-seq
 *
 * exp-part = exp-char [exp-sign] digit-seq
 *
 * exp-char = 'e' / 'E'
 *
 * sign = '+' / '-'
 *
 * exp-sign = '+' / '-'
 *
 * digit-seq = digit
 * 			/ digit-seq digit
 *
 */

//
//  hack to get locale dependent decimal point char (spied at stackoverflow.com)
//
//	setlocale(LC_NUMERIC, "C");
//	char fchars[10];
//	sprintf(fchars, "%f", 0.0f);
//	char decimalPoint = fchars[1];
//

namespace pfs {

enum parse_float_state_enum {
	  parse_sign_state
	, skip_zeros_state
	, parse_nan_state
	, parse_inf_state
	, parse_mantissa_state
	, parse_exp_state
	, finish_state
	, nan_state
	, infinity_state
};

/* Table giving binary powers of 10.  Entry
   is 10^2^i.  Used to convert decimal
   exponents into floating-point numbers.
*/
template <typename FloatT>
struct powersOf10;

template <>
struct powersOf10<float>
{
    static float * values ()
    {
        static float a[] = {
            10.
          , 100.
          , 1.0e4
          , 1.0e8
          , 1.0e16
          , 1.0e32
        };
        
        return a;
    }
};

template <>
struct powersOf10<double>
{
    static double * values ()
    {
        static double a[] = {
            10.
          , 100.
          , 1.0e4
          , 1.0e8
          , 1.0e16
          , 1.0e32
          , 1.0e64
          , 1.0e128
          , 1.0e256
        };
        
        return a;
    }
};

#ifdef PFS_HAVE_LONG_DOUBLE
template <>
struct powersOf10<long double>
{
    static long double * values ()
    {
        static long double a[] = {
            10.L
          , 100.L
          , 1.0e4L
          , 1.0e8L
          , 1.0e16L
          , 1.0e32L
          , 1.0e64L
          , 1.0e128L
          , 1.0e256L
          , 1.0e512L
          , 1.0e1024L
          , 1.0e2048L
          , 1.0e4096L
        };
        
        return a;
    }
};
#endif

template <typename CharIteratorT>
static CharIteratorT __parse_nan_string (CharIteratorT beginpos
		, CharIteratorT endpos)
{
    //typedef typename iterator_traits<CharIteratorT>::value_type value_type;
	CharIteratorT pos(beginpos);

	if (pos < endpos && (*pos == 'N' || *pos == 'n')) {
		if (++pos < endpos && (*pos == 'A' || *pos == 'a')) {
			if (++pos < endpos && (*pos == 'N' || *pos == 'n')) {
				++pos;
				return pos;
			}
		}
	}

	return beginpos;
}

template <typename CharIteratorT>
CharIteratorT __parse_inf_string (CharIteratorT beginpos
		, CharIteratorT endpos)
{
	//typedef typename iterator_traits<CharIteratorT>::value_type value_type;

	CharIteratorT pos(beginpos);

	if (pos < endpos && (*pos == 'I' || *pos == 'i')) {
		if (++pos < endpos && (*pos == 'N' || *pos == 'n')) {
			if (++pos < endpos && (*pos == 'F' || *pos == 'f')) {
				if (++pos < endpos && (*pos == 'I' || *pos == 'i')) {
					if (++pos < endpos && (*pos == 'N' || *pos == 'n')) {
						if (++pos < endpos && (*pos == 'I' || *pos == 'i')) {
							if (++pos < endpos && (*pos == 'T' || *pos == 't')) {
								if (++pos < endpos && (*pos == 'Y' || *pos == 'y')) {
									++pos;
									return pos;
								}
							}
						}
					}
				} else { // 'INF' string
					return pos;
				}
			}
		}
	}

	return beginpos;
}

template <typename CharIteratorT>
CharIteratorT __parse_exp (CharIteratorT beginpos
		, CharIteratorT endpos
		, int & expSign
		, int & exp)
{
    typedef typename iterator_traits<CharIteratorT>::value_type value_type;

	CharIteratorT pos(beginpos);
	exp = 0;
	expSign = 1;

	if (pos < endpos) {
		if (!(*pos == 'e' || *pos == 'E'))
			return beginpos;

		++pos;

		if (pos == endpos)
			return beginpos;

		if (*pos == '-') {
			expSign = -1;
			++pos;
		} else if (*pos == '+') {
			++pos;
		}

		if (pos == endpos)
			return beginpos;

		if (!pfs::is_digit(*pos))
			return beginpos;

		while (pos < endpos) {
			if (pfs::is_digit(*pos)) {
				int digit = *pos - '0';
				exp = exp * 10 + digit;
				PFS_ASSERT(exp >= 0);
			} else {
				break;
			}
			++pos;
		}
	}

	return pos;
}

/**
 *
 * @param beginpos
 * @param endpos
 * @param badpos
 * @return
 *          If no conversion is performed, zero is returned and the value of @c beginpos
 *          is stored in the location referenced by @c badpos.
 * 			If error occured @c errno set to @c ERANGE (overflow or underflow) or @c EINVAL
 * 			(if infinity or NaN value)
 */
template <typename FloatT, typename CharIteratorT>
FloatT string_to_float (CharIteratorT beginpos
		, CharIteratorT endpos
		, typename iterator_traits<CharIteratorT>::value_type decimalPoint
		, CharIteratorT * badpos
        , error_code & ec) // badpos
{
	typedef typename iterator_traits<CharIteratorT>::value_type value_type;

	CharIteratorT pos(beginpos);
	FloatT r = FloatT(0.0f);
	int sign = 1;
	int exp_sign = 1;
	int integral_size = 0;
	int fract_size = 0;
	int exp = 0;
	bool has_dec_point = false;
    bool has_payload = false; // one or more digits found

//#ifdef PFS_HAVE_LONG_DOUBLE
//	static long double powersOf10[] = {
//#else
//    static double powersOf10[] = {
//#endif        
//		  10.
//	    , 100.
//		, 1.0e4
//		, 1.0e8
//		, 1.0e16
//        , 1.0e32
//		, 1.0e64
//		, 1.0e128
//		, 1.0e256
//#ifdef PFS_HAVE_LONG_DOUBLE
//		, 1.0e512L
//		, 1.0e1024L
//		, 1.0e2048L
//		, 1.0e4096L
//#endif
//	};

	errno = 0;

    if (beginpos == endpos) {
    	if (badpos)
    		*badpos = beginpos;
		return FloatT(0.0f);
    }

    //
    // Skip over any leading whitespace.
    //
    while (pos < endpos && pfs::is_space(*pos)) {
    	++pos;
    }

    parse_float_state_enum state = parse_sign_state;

    while (state < finish_state && pos < endpos) {
    	switch (state) {
    	case parse_sign_state:
    		if (*pos == '-') {
    			sign = -1;
    			++pos;
    		} else if (*pos == '+') {
    			++pos;
    		}

    		state = skip_zeros_state;
    		break;

    	case skip_zeros_state:
    		if (*pos == '0') {
    			while (pos < endpos && *pos == '0')
    				++pos;
                has_payload = true;
    		} else {
    			state = parse_nan_state;
    		}
    		break;

    	case parse_nan_state: {
    		CharIteratorT pos1 = __parse_nan_string(pos, endpos);
            
    		if (pos1 != pos) {
                r = pfs::numeric_limits<FloatT>::quiet_NaN();
    			if (sign < 0)
                    r = -r;
    			pos = pos1;
    			state = nan_state;
                has_payload = true;
    		} else {
    			state = parse_inf_state;
    		}
    		break;
    	}

    	case parse_inf_state: {
    		CharIteratorT pos1 = __parse_inf_string(pos, endpos);

    		if (pos1 != pos) {
                r = pfs::numeric_limits<FloatT>::infinity();
    			if (sign < 0)
                    r = -r;
    			pos = pos1;
    			state = infinity_state;
                has_payload = true;
    		} else {
    			state = parse_mantissa_state;
    		}
    		break;
    	}

    	case parse_mantissa_state:
    		while (pos < endpos) {
				if (pfs::is_digit(*pos)) {
					int digit = *pos - '0';

					r = r * 1.e1 + digit;

					if (has_dec_point) {
						++fract_size;
					} else {
						++integral_size;
					}
				} else if (*pos == decimalPoint) {
					if (!has_dec_point) {
						has_dec_point = true;
					} else {
						state = finish_state;
						break;
					}
				} else  {
					state = parse_exp_state;
					break;
				}
				++pos;
    		}
    		break;

    	case parse_exp_state:
    		pos = __parse_exp(pos, endpos, exp_sign, exp);
    		state = finish_state;
    		break;

    	case finish_state:
    	default:
    		break;
    	}
    }

	int mant_size = integral_size + fract_size;

    if (mant_size > 0)
        has_payload = true;

    if (! has_payload) {
        pos = beginpos;
    } else if (state <= finish_state) {
		if (exp_sign < 0)
			exp = - exp;

		if (fract_size > 0)
			exp -= fract_size;
        
		if (exp < pfs::numeric_limits<FloatT>::min_exponent10) { // underflow
			ec = pfs::make_error_code(lexical_cast_errc::underflow);
			r = 0.0f;
		} else if (exp > pfs::numeric_limits<FloatT>::max_exponent10) { // overflow
			if (sign < 0) {
				r = - pfs::numeric_limits<FloatT>::infinity();
			} else {
				r = pfs::numeric_limits<FloatT>::infinity();
			}
            ec = pfs::make_error_code(lexical_cast_errc::overflow);
		} else {
			FloatT dblExp = FloatT(1.0);

			if (exp < 0) {
				exp = - exp;
				exp_sign = -1;
			} else {
				exp_sign = 1;
			}

			for (FloatT * d = powersOf10<FloatT>::values(); exp != 0; exp >>= 1, d += 1) {
				if (exp & 01) {
					dblExp *= *d;
				}
			}
            
//            dblExp = pfs::pow(10.0f, exp);

			if (exp_sign < 0) {
				r /= dblExp;
			} else {
				r *= dblExp;
			}

			if (sign < 0)
				r = - r;
		}
    }

    if (badpos) {
    	*badpos = pos;
    }

    return r;
}

template <typename FloatT, typename CharIteratorT>
FloatT string_to_float (CharIteratorT beginpos
		, CharIteratorT endpos
		, typename iterator_traits<CharIteratorT>::value_type decimalPoint
		, CharIteratorT * badpos)
{
    error_code ec;
    FloatT result = string_to_float<FloatT, CharIteratorT>(beginpos, endpos, decimalPoint, badpos, ec);
            
    if (ec.value() != static_cast<int>(lexical_cast_errc::success))
        throw bad_lexical_cast(lexical_cast_category().message(ec.value()));
    
    return result;
}
    
} // pfs
