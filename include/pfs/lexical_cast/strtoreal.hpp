/*
 * strtoreal.cpp
 *
 *  Created on: Jan 2, 2016
 *      Author: wladt
 */

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
//	, skip_zeros_state
	, parse_nan_state
	, parse_inf_state
	, parse_mantissa_state
	, parse_exp_state
	, finish_state
//	, nan_state
//	, infinity_state
};

//
// Table giving binary powers of 10. Entry is 10^2^i.Used to convert decimal
// exponents into floating-point numbers.
//
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

#if __OBSOLETE__

template <typename CharIteratorT>
CharIteratorT __parse_exp (CharIteratorT beginpos
		, CharIteratorT endpos
		, int & expSign
		, int & exp)
{
    //typedef typename iterator_traits<CharIteratorT>::value_type value_type;

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
				int digit = to_ascii(*pos) - '0';
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

// TODO DEPRECATED see string_to_real()

template <typename FloatT, typename CharIteratorT>
FloatT string_to_float (CharIteratorT first
		, CharIteratorT last
		, typename iterator_traits<CharIteratorT>::value_type decimal_point
		, CharIteratorT * badpos
        , error_code & ec) // badpos
{
	CharIteratorT pos(first);
	FloatT r = FloatT(0.0f);
	int sign = 1;
	int exp_sign = 1;
	int integral_size = 0;
	int fract_size = 0;
	int exp = 0;
	bool has_dec_point = false;
    bool has_payload = false; // one or more digits found

	errno = 0;

    if (first == last) {
    	if (badpos)
    		*badpos = first;
		return FloatT(0.0f);
    }

    //
    // Skip over any leading whitespace.
    //
    while (pos < last && pfs::is_space(*pos)) {
    	++pos;
    }

    parse_float_state_enum state = parse_sign_state;

    while (state < finish_state && pos < last) {
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
    			while (pos < last && *pos == '0')
    				++pos;
                has_payload = true;
    		} else {
    			state = parse_nan_state;
    		}
    		break;

    	case parse_nan_state: {
    		CharIteratorT pos1 = __parse_nan_string(pos, last);
            
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
    		CharIteratorT pos1 = __parse_inf_string(pos, last);

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
    		while (pos < last) {
				if (pfs::is_digit(*pos)) {
					int digit = to_ascii(*pos) - '0';

					r = r * 1.e1 + digit;

					if (has_dec_point) {
						++fract_size;
					} else {
						++integral_size;
					}
				} else if (*pos == decimal_point) {
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
    		pos = __parse_exp(pos, last, exp_sign, exp);
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
        pos = first;
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

#endif // __OBSOLETE__

namespace details {

//
// Emulates c-string iterator (pointer to char).
// Increments wrapped iterator if current position is not at the end 
// and returns '\x0' at the end.
//
template <typename Iter>
struct char_iterator_wrapper
{
    typedef typename Iter::value_type value_type;

    Iter pos;
    Iter last;

    char_iterator_wrapper () {}
    
    char_iterator_wrapper (char_iterator_wrapper const & rhs)
        : pos(rhs.pos)
        , last(rhs.last)
    {}

    char_iterator_wrapper & operator = (char_iterator_wrapper const & rhs)
    {
        pos = rhs.pos;
        last = rhs.last;
        return *this;
    }
    
    char_iterator_wrapper (Iter const & p, Iter const & l)
        : pos(p)
        , last(l)
    {}
    
    value_type operator * () const
    {
    	return pos == last ? value_type(0) : *pos;
    }

    void operator ++ () // prefix increment
	{
        if (pos != last)
            ++pos;
	}
};

} // details

template <typename RealType, typename CharIterator>
RealType string_to_real (CharIterator first
		, CharIterator last
		, typename iterator_traits<CharIterator>::value_type decimal_point_char
		, CharIterator * badpos
        , error_code & ec) // badpos
{
    typedef typename iterator_traits<CharIterator>::value_type value_type;
    static RealType frac1_powers[] = {1.0e0, 1.0e1, 1.0e2, 1.0e3, 1.0e4, 1.0e5, 1.0e6, 1.0e7, 1.0e8, 1.0e9};
    
	CharIterator p(first);
	RealType r = RealType(0.0f);
	int sign = 1;
    RealType dbl_exp = 1.0f;
    int exp_sign = 1;
    int exp = 0;
    int frac_exp = 0;
    int frac1 = 0, frac2 = 0;
    int frac1_power = 0;
    int mantissa_size = 0;
    int decimal_point_size = -1;
    details::char_iterator_wrapper<CharIterator> pos(p, last);
    details::char_iterator_wrapper<CharIterator> last_zero_pos;
    details::char_iterator_wrapper<CharIterator> nan;
    details::char_iterator_wrapper<CharIterator> inf;
    details::char_iterator_wrapper<CharIterator> exp_pos;

    if (first == last) {
    	if (badpos)
    		*badpos = first;
		return RealType(0.0f);
    }

    // Skip over any leading whitespace
    //==========================================================================
    while (pfs::is_space(*pos))
        ++pos;

    // Parse sign
    //==========================================================================
    if (*pos == '-') {
        sign = -1;
        ++pos;
    } else if (*pos == '+') {
        ++pos;
    }

    // Parse NaN
    //==========================================================================
    nan = pos;

    if (*nan == 'N' || *nan == 'n') {
        ++nan;
        if (*nan == 'A' || *nan == 'a') {
            ++nan;
            if (*nan == 'N' || *nan == 'n') {
                ++nan;
                r = pfs::numeric_limits<RealType>::quiet_NaN();
                goto done;
            }
        }
    }

    // Parse +-Infinity
    //==========================================================================
    inf = pos;

    if (*inf == 'I' || *inf == 'i') {
        ++inf;
        if (*inf == 'N' || *inf == 'n') {
            ++inf;
            if (*inf == 'F' || *inf == 'f') {
                ++inf;
                if (*inf == 'I' || *inf == 'i') {
                    ++inf;
                    if (*inf == 'N' || *inf == 'n') {
                        ++inf;
                        if (*inf == 'I' || *inf == 'i') {
                            ++inf;
                            if (*inf == 'T' || *inf == 't') {
                                ++inf;
                                if (*inf == 'Y' || *inf == 'y') {
                                    ++inf;
                                    r = pfs::numeric_limits<RealType>::infinity();
                                    goto done;
                                }
                            }
                        }
                    }
                } else { // 'INF' string
                    r = pfs::numeric_limits<RealType>::infinity();
                    goto done;
                }
            }
        }
    }
    
    // Skip zeros
    //==========================================================================
    last_zero_pos = pos;
    
    while (*pos == '0') {
        last_zero_pos = pos;
        ++pos;
    }
    
    pos = last_zero_pos;
    
    // Parse mantissa
    //==========================================================================
    // For double type mantissa has 18 significant digits, 
    // so extra digits could be considered zeros (they can't affect the 
    // result value). And represent this 18 digits as two integer fractions

    for (value_type c = *pos
            ; c != value_type(0) && mantissa_size < 9
            ; ++pos, ++mantissa_size, c = *pos) {
        
        if (!is_digit(c)) {
            if (c == decimal_point_char) {
                if (decimal_point_size < 0) {
                    decimal_point_size = mantissa_size;
                    --mantissa_size;
                    continue;
                } else { // second decimal point
                    break;
                }
            } else {
                break;
            }
        }

        frac1 = 10 * frac1 + (to_ascii(c) - '0');
    }

    frac1_power = 0;
            
    for (value_type c = *pos
            ; c != value_type(0) && mantissa_size < 18
            ; ++pos, ++mantissa_size, c = *pos) {
        
        if (!is_digit(c)) {
            if (c == decimal_point_char) {
                if (decimal_point_size < 0) {
                    decimal_point_size = mantissa_size;
                    --mantissa_size;
                    continue;
                } else { // second decimal point
                    break;
                }
            } else {
                break;
            }
        }
        
        frac2 = 10 * frac2 + (to_ascii(c) - '0');
        ++frac1_power;
    }

    r = (frac1_powers[frac1_power] * frac1) + frac2;

    while (is_digit(*pos)) {
        ++mantissa_size;
        ++pos;
    }

    if (decimal_point_size < 0) {
        decimal_point_size = mantissa_size;
    }
//    } else {
//        --mantissa_size; // One of the digits was the point
//    }

    if (mantissa_size > 18) {
        frac_exp = decimal_point_size - 18;
        mantissa_size = 18;
    } else {
        frac_exp = decimal_point_size - mantissa_size;
    }

    if (mantissa_size == 0) {
        r = 0.0f;
        pos.pos = first;
        goto done;
    }

    // Parse exponent
    //==========================================================================

    if ((*pos == 'E') || (*pos == 'e')) {
        
        exp_pos = pos;
        
        ++pos;

        if (*pos == '-') {
            exp_sign = -1;
            ++pos;
        } else {
            if (*pos == '+') {
                ++pos;
            }
        }

        if (is_digit(*pos)) {
            while (is_digit(*pos)) {
                exp = exp * 10 + (to_ascii(*pos) - '0');
                ++pos;
            }
        } else {
            pos = exp_pos;
        }
    }

    if (exp_sign < 0) {
        exp = frac_exp - exp;
    } else {
        exp = frac_exp + exp;
    }

    // Generate a floating-point number that represents the exponent.
    // Do this by processing the exponent one bit at a time to combine
    // many powers of 2 of 10. Then combine the exponent with the
    // fraction.

    if (exp < 0) {
        exp_sign = -1;
        exp = -exp;
    } else {
        exp_sign = 1;
    }

    if (exp < pfs::numeric_limits<RealType>::min_exponent10) { // underflow
        ec = pfs::make_error_code(lexical_cast_errc::underflow);
        r = 0.0f;
        goto done;
    } else if (exp > pfs::numeric_limits<RealType>::max_exponent10) { // overflow
        if (sign < 0) {
            r = -pfs::numeric_limits<RealType>::infinity();
        } else {
            r = pfs::numeric_limits<RealType>::infinity();
        }

        ec = pfs::make_error_code(lexical_cast_errc::overflow);
        goto done;
    }

    for (RealType * d = powersOf10<RealType>::values(); exp != 0; exp >>= 1, d += 1) {
        if (exp & 01) {
            dbl_exp *= *d;
        }
    }

    if (exp_sign < 0) {
        r /= dbl_exp;
    } else {
        r *= dbl_exp;
    }
    
done:
    if (badpos)
        *badpos = pos.pos;

    return sign < 0 ? -r : r;
}

namespace experimental {

template <typename CharIteratorT>
double string_to_double (CharIteratorT first
		, CharIteratorT last
		, typename iterator_traits<CharIteratorT>::value_type decimalPoint
		, CharIteratorT * badpos
        , error_code & ec) // badpos
{
    typedef typename iterator_traits<CharIteratorT>::value_type value_type;
    static int const max_exponent = 511;
    
    int sign = 1;
    int expSign = 1;
	double fraction;
	int exp = 0;	   /* Exponent read from "EX" field. */
	int fracExp = 0;   /* Exponent that derives from the fractional
                        * part.  Under normal circumstatnces, it is
                        * the negative of the number of digits in F.
                        * However, if I is very long, the last digits
                        * of I get dropped (otherwise a long I with a
                        * large negative exponent could cause an
                        * unnecessary overflow on I alone).  In this
                        * case, fracExp is incremented one for each
                        * dropped digit. */
	int mantSize;      /* Number of digits in mantissa. */
	int decPt;         /* Number of mantissa digits BEFORE decimal point. */
    double dblExp = 1.0f;
  

	/*
	 * Strip off leading blanks and check for a sign.
	 */

	CharIteratorT p = first;
    
	while (pfs::is_space(*p)) {
		++p;
	}
    
	if (*p == '-') {
		sign = -1;
		++p;
	} else {
		if (*p == '+') {
			++p;
		}
	}

	/*
	 * Count the number of digits in the mantissa (including the decimal
	 * point), and also locate the decimal point.
	 */

	decPt = -1;
    
	for (mantSize = 0; ; ++mantSize)	{
		if (!pfs::is_digit(*p)) {
			if ((*p != decimalPoint) || (decPt >= 0)) {
				break;
			}
			decPt = mantSize;
		}
		++p;
	}

	/*
	 * Now suck up the digits in the mantissa.  Use two integers to
	 * collect 9 digits each (this is faster than using floating-point).
	 * If the mantissa has more than 18 digits, ignore the extras, since
	 * they can't affect the value anyway.
	 */

	CharIteratorT pExp = p;
    
	pfs::advance(p, -mantSize);
    
	if (decPt < 0) {
		decPt = mantSize;
	} else {
		mantSize -= 1;			/* One of the digits was the point. */
	}
    
	if (mantSize > 18) {
		fracExp = decPt - 18;
		mantSize = 18;
	} else {
		fracExp = decPt - mantSize;
	}
    
	if (mantSize == 0) {
		fraction = 0.0f;
		p = first;
		goto done;
	} else {
		int frac1 = 0;
        
		for ( ; mantSize > 9; --mantSize) {
			value_type c = *p;
			++p;
            
			if (c == '.') {
				c = *p;
				++p;
			}
            
			frac1 = 10 * frac1 + (pfs::to_ascii(c) - '0');
		}
        
		int frac2 = 0;
        
		for (; mantSize > 0; --mantSize)	{
			value_type c = *p;
			++p;
            
			if (c == '.') {
				c = *p;
				++p;
			}
            
			frac2 = 10 * frac2 + (pfs::to_ascii(c) - '0');
		}
        
		fraction = (1.0e9 * frac1) + frac2;
	}

	/*
	 * Skim off the exponent.
	 */

	p = pExp;
    
	if ((*p == 'E') || (*p == 'e')) {
		++p;
        
		if (*p == '-') {
			expSign = -1;
			++p;
		} else {
			if (*p == '+') {
				++p;
			}
		}
        
		while (pfs::is_digit(*p)) {
			exp = exp * 10 + (pfs::to_ascii(*p) - '0');
			++p;
		}
	}
    
	if (expSign < 0) {
		exp = fracExp - exp;
	} else {
		exp = fracExp + exp;
	}

	/*
	 * Generate a floating-point number that represents the exponent.
	 * Do this by processing the exponent one bit at a time to combine
	 * many powers of 2 of 10. Then combine the exponent with the
	 * fraction.
	 */

	if (exp < 0) {
		expSign = -1;
		exp = -exp;
	} else {
		expSign = 1;
	}
    
	if (exp > max_exponent) {
		exp = max_exponent;
	}
    
 	for (double * d = powersOf10<double>::values(); exp != 0; exp >>= 1, d += 1) {
		if (exp & 01) {
			dblExp *= *d;
		}
	}
   
	if (expSign < 0) {
		fraction /= dblExp;
	} else {
		fraction *= dblExp;
	}

done:
	if (badpos) {
		*badpos = p;
	}

	if (sign < 0) {
		return -fraction;
	}

    return fraction;    
}

template <typename CharIteratorT>
float string_to_float (CharIteratorT first
		, CharIteratorT last
		, typename iterator_traits<CharIteratorT>::value_type decimalPoint
		, CharIteratorT * badpos
        , error_code & ec) // badpos
{
    typedef typename iterator_traits<CharIteratorT>::value_type value_type;
    static int const max_exponent = 38;
    
    int sign = 1;
    int expSign = 1;
	float fraction;
	int exp = 0;	   /* Exponent read from "EX" field. */
	int fracExp = 0;   /* Exponent that derives from the fractional
                        * part.  Under normal circumstatnces, it is
                        * the negative of the number of digits in F.
                        * However, if I is very long, the last digits
                        * of I get dropped (otherwise a long I with a
                        * large negative exponent could cause an
                        * unnecessary overflow on I alone).  In this
                        * case, fracExp is incremented one for each
                        * dropped digit. */
	int mantSize;      /* Number of digits in mantissa. */
	int decPt;         /* Number of mantissa digits BEFORE decimal point. */
    float dblExp = 1.0f;
  

	/*
	 * Strip off leading blanks and check for a sign.
	 */

	CharIteratorT p = first;
    
	while (pfs::is_space(*p)) {
		++p;
	}
    
	if (*p == '-') {
		sign = -1;
		++p;
	} else {
		if (*p == '+') {
			++p;
		}
	}

	/*
	 * Count the number of digits in the mantissa (including the decimal
	 * point), and also locate the decimal point.
	 */

	decPt = -1;
    
	for (mantSize = 0; ; ++mantSize)	{
		if (!pfs::is_digit(*p)) {
			if ((*p != decimalPoint) || (decPt >= 0)) {
				break;
			}
			decPt = mantSize;
		}
		++p;
	}

	/*
	 * Now suck up the digits in the mantissa.  Use two integers to
	 * collect 9 digits each (this is faster than using floating-point).
	 * If the mantissa has more than 18 digits, ignore the extras, since
	 * they can't affect the value anyway.
	 */

	CharIteratorT pExp = p;
    
	pfs::advance(p, -mantSize);
    
	if (decPt < 0) {
		decPt = mantSize;
	} else {
		mantSize -= 1;			/* One of the digits was the point. */
	}
    
	if (mantSize > 18) {
		fracExp = decPt - 18;
		mantSize = 18;
	} else {
		fracExp = decPt - mantSize;
	}
    
	if (mantSize == 0) {
		fraction = 0.0f;
		p = first;
		goto done;
	} else {
		int frac1 = 0;
        
		for ( ; mantSize > 9; --mantSize) {
			value_type c = *p;
			++p;
            
			if (c == '.') {
				c = *p;
				++p;
			}
            
			frac1 = 10 * frac1 + (pfs::to_ascii(c) - '0');
		}
        
		int frac2 = 0;
        
		for (; mantSize > 0; --mantSize)	{
			value_type c = *p;
			++p;
            
			if (c == '.') {
				c = *p;
				++p;
			}
            
			frac2 = 10 * frac2 + (pfs::to_ascii(c) - '0');
		}
        
		fraction = (1.0e9 * frac1) + frac2;
	}

	/*
	 * Skim off the exponent.
	 */

	p = pExp;
    
	if ((*p == 'E') || (*p == 'e')) {
		++p;
        
		if (*p == '-') {
			expSign = -1;
			++p;
		} else {
			if (*p == '+') {
				++p;
			}
		}
        
		while (pfs::is_digit(*p)) {
			exp = exp * 10 + (pfs::to_ascii(*p) - '0');
			++p;
		}
	}
    
	if (expSign < 0) {
		exp = fracExp - exp;
	} else {
		exp = fracExp + exp;
	}

	/*
	 * Generate a floating-point number that represents the exponent.
	 * Do this by processing the exponent one bit at a time to combine
	 * many powers of 2 of 10. Then combine the exponent with the
	 * fraction.
	 */

	if (exp < 0) {
		expSign = -1;
		exp = -exp;
	} else {
		expSign = 1;
	}
    
	if (exp > max_exponent) {
		exp = max_exponent;
	}
    
 	for (float * d = powersOf10<float>::values(); exp != 0; exp >>= 1, d += 1) {
		if (exp & 01) {
			dblExp *= *d;
		}
	}
   
	if (expSign < 0) {
		fraction /= dblExp;
	} else {
		fraction *= dblExp;
	}

done:
	if (badpos) {
		*badpos = p;
	}

	if (sign < 0) {
		return -fraction;
	}

    return fraction;    
}

template <typename CharIteratorT>
double string_to_double (CharIteratorT first
		, CharIteratorT last
		, typename iterator_traits<CharIteratorT>::value_type dec_point
		, CharIteratorT * badpos)
{
    error_code ec;
    double result = string_to_double<CharIteratorT>(first, last, dec_point, badpos, ec);
            
    if (ec.value() != static_cast<int>(lexical_cast_errc::success))
        throw bad_lexical_cast(ec);
    
    return result;
}

template <typename CharIteratorT>
float string_to_float (CharIteratorT first
		, CharIteratorT last
		, typename iterator_traits<CharIteratorT>::value_type dec_point
		, CharIteratorT * badpos)
{
    error_code ec;
    float result = string_to_float<CharIteratorT>(first, last, dec_point, badpos, ec);
            
    if (ec.value() != static_cast<int>(lexical_cast_errc::success))
        throw bad_lexical_cast(ec);
    
    return result;
}

} // experimental

// TODO DEPRECATED
// use string_to_real() instead

//template <typename FloatT, typename CharIteratorT>
//FloatT string_to_float (CharIteratorT first
//		, CharIteratorT last
//		, typename iterator_traits<CharIteratorT>::value_type dec_point
//		, CharIteratorT * badpos)
//{
//    error_code ec;
//    FloatT result = string_to_float<FloatT, CharIteratorT>(first, last, dec_point, badpos, ec);
//            
//    if (ec.value() != static_cast<int>(lexical_cast_errc::success))
//        throw bad_lexical_cast(ec);
//    
//    return result;
//}

template <typename RealType, typename CharIterator>
RealType string_to_real (CharIterator first
		, CharIterator last
		, typename iterator_traits<CharIterator>::value_type decimal_point
		, CharIterator * badpos)
{
    error_code ec;
    RealType result = string_to_real<RealType, CharIterator>(first, last, decimal_point, badpos, ec);
            
    if (ec.value() != static_cast<int>(lexical_cast_errc::success))
        throw bad_lexical_cast(ec);
    
    return result;
}
    
} // pfs
