/*
 * strtoreal.cpp
 *
 *  Created on: Jan 2, 2016
 *      Author: wladt
 */

#include "pfs/string.hpp"
#include "pfs/unicode.hpp"

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

namespace pfs {

enum state_enum {
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


//inline int __to_decimal_digit (unicode::char_type c)
//{
//	int r = -1;
//
//	if (c.value >= '0' && c.value <= '9')
//		r = c.value - '0';
//
//	return r;
//}


static string::const_iterator check_str_nan (string::const_iterator begin
		, string::const_iterator end)
{
	typedef string::value_type char_type;
	string::const_iterator pos(begin);

	if (pos < end && (*pos == 'N' || *pos == 'n')) {
		if (++pos < end && (*pos == 'A' || *pos == 'a')) {
			if (++pos < end && (*pos == 'N' || *pos == 'n')) {
				++pos;
				return pos;
			}
		}
	}

	return begin;
}

static string::const_iterator check_str_inf (string::const_iterator begin
		, string::const_iterator end)
{
	typedef string::value_type char_type;

	string::const_iterator pos(begin);

	if (pos < end && (*pos == 'I' || *pos == 'i')) {
		if (++pos < end && (*pos == 'N' || *pos == 'n')) {
			if (++pos < end && (*pos == 'F' || *pos == 'f')) {
				if (++pos < end && (*pos == 'I' || *pos == 'i')) {
					if (++pos < end && (*pos == 'N' || *pos == 'n')) {
						if (++pos < end && (*pos == 'I' || *pos == 'i')) {
							if (++pos < end && (*pos == 'T' || *pos == 't')) {
								if (++pos < end && (*pos == 'Y' || *pos == 'y')) {
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

	return begin;
}

static string::const_iterator parse_exp (string::const_iterator begin
		, string::const_iterator end
		, int & expSign
		, int & exp)
{
	typedef string::value_type char_type;

	string::const_iterator pos(begin);
	exp = 0;
	expSign = 1;

	if (pos < end) {
		if (!(*pos == 'e' || *pos == 'E'))
			return begin;

		++pos;

		if (pos == end)
			return begin;

		if (*pos == '-') {
			expSign = -1;
			++pos;
		} else if (*pos == '+') {
			++pos;
		}

		if (pos == end)
			return begin;

		if (!unicode::is_decimal_digit(*pos))
			return begin;

		while (pos < end) {
			if (unicode::is_decimal_digit(*pos)) {
				int digit = __to_decimal_digit(*pos);
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
 * @param begin
 * @param end
 * @param endref
 * @return
 *          If no conversion is performed, zero is returned and the value of @c begin
 *          is stored in the location referenced by @c endref.
 * 			If error occured @c errno set to @c ERANGE (overflow or underflow) or @c EINVAL
 * 			(if infinity or NaN value)
 */
real_t string_to_real (string::const_iterator begin
		, string::const_iterator end
		, string::value_type decimalPoint
		, string::const_iterator * endref)
{
	typedef string::value_type char_type;

	string::const_iterator pos(begin);
	real_t r = real_t(0.0f);
	int sign = 1;
	int expSign = 1;
	int integralSize = 0;
	int fractSize = 0;
	int exp = 0;
	bool hasDecPoint = false;

	/* Table giving binary powers of 10.  Entry
	   is 10^2^i.  Used to convert decimal
	   exponents into floating-point numbers.
	*/
	static real_t powersOf10[] = {
		  PFS_REAL_LITERAL(10.)
	    , PFS_REAL_LITERAL(100.)
		, PFS_REAL_LITERAL(1.0e4)
		, PFS_REAL_LITERAL(1.0e8)
		, PFS_REAL_LITERAL(1.0e16)
		, PFS_REAL_LITERAL(1.0e32)
		, PFS_REAL_LITERAL(1.0e64)
		, PFS_REAL_LITERAL(1.0e128)
		, PFS_REAL_LITERAL(1.0e256)
#ifdef PFS_HAVE_REAL128
		, PFS_REAL_LITERAL(1.0e512)
		, PFS_REAL_LITERAL(1.0e1024)
		, PFS_REAL_LITERAL(1.0e2048)
		, PFS_REAL_LITERAL(1.0e4096)
#endif
	};

	errno = 0;

    if (begin == end) {
    	if (endref)
    		*endref = begin;
		return real_t(0.0f);
    }

    //
    // Skip over any leading whitespace.
    //
    while (pos < end && unicode::is_space(*pos)) {
    	++pos;
    }

    state_enum state = parse_sign_state;

    while (state < finish_state && pos < end) {
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
    			while (pos < end && *pos == '0')
    				++pos;
    		} else {
    			state = parse_nan_state;
    		}
    		break;

    	case parse_nan_state: {
    		string::const_iterator pos1 = check_str_nan(pos, end);
    		if (pos1 != pos) {
    			r = sign < 0 ? - nan_value<real_t>() : nan_value<real_t>();
    			pos = pos1;
    			state = nan_state;
    		} else {
    			state = parse_inf_state;
    		}
    		break;
    	}

    	case parse_inf_state: {
    		string::const_iterator pos1 = check_str_inf(pos, end);

    		if (pos1 != pos) {
    			r = sign < 0 ? - inf_value<real_t>() : inf_value<real_t>();
    			pos = pos1;
    			state = infinity_state;
    		} else {
    			state = parse_mantissa_state;
    		}
    		break;
    	}

    	case parse_mantissa_state:
    		while (pos < end) {
				if (unicode::is_decimal_digit(*pos)) {
					int digit = __to_decimal_digit(*pos);

					r = r * PFS_REAL_LITERAL(1.e1) + digit;

					if (hasDecPoint) {
						++fractSize;
					} else {
						++integralSize;
					}
				} else if (*pos == decimalPoint) {
					if (!hasDecPoint) {
						hasDecPoint = true;
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
    		pos = parse_exp(pos, end, expSign, exp);
    		state = finish_state;
    		break;

    	case finish_state:
    	default:
    		break;
    	}
    }

	int mant_size = integralSize + fractSize;

    if (state > finish_state) {
    	;
    } else if (mant_size == 0) {
    	if (state != skip_zeros_state) // to accept zero series as 0
    		pos = begin;
    } else if (state <= finish_state) {
		if (expSign < 0)
			exp = - exp;

		if (fractSize > 0)
			exp -= fractSize;

		if (exp < min_exponent10<real_t>()/*PFS_REAL_MIN_10_EXP*/) { // underflow
			errno = ERANGE;
			r = 0.0f;
		} else if (exp > max_exponent10<real_t>()/*PFS_REAL_MAX_10_EXP*/) { // overflow
			errno = ERANGE;
			if (sign < 0) {
				r = - inf_value<real_t>()/*PFS_HUGE_VAL*/;
			} else {
				r = inf_value<real_t>()/*PFS_HUGE_VAL*/;
			}
		} else {
			real_t dblExp, *d;

			dblExp = PFS_REAL_LITERAL(1.0);

			if (exp < 0) {
				exp = - exp;
				expSign = -1;
			} else {
				expSign = 1;
			}

			for (d = powersOf10; exp != 0; exp >>= 1, d += 1) {
				if (exp & 01) {
					dblExp *= *d;
				}
			}

			if (expSign < 0) {
				r /= dblExp;
			} else {
				r *= dblExp;
			}

			if (sign < 0)
				r = - r;
		}
    }

    if (endref) {
    	*endref = pos;
    }

    if (!errno && (is_nan(r) || is_inf(r)))
    	errno = EINVAL;

    return r;
}

} // pfs
