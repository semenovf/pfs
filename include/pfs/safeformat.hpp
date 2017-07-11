
/* 
 * File:   safeformat.hpp
 * Author: wladt
 *
 * Created on July 6, 2017, 4:09 PM
 */

#ifndef __PFS_SAFEFORMAT_HPP__
#define __PFS_SAFEFORMAT_HPP__

#include <pfs/ctype.hpp>
#include <pfs/string.hpp>
#include <pfs/limits.hpp>

/*
 * conversion_specification := '%' *flag [ field_width ] [ prec ] conversion_specifier
 * flag := '0' ; the value should be zero padded (ignored when 'prec' is specified)
 * 		 / '-' ; the value is left-justified
 * 		 / ' ' ; the value should be padded whith spaces (ignored when 'prec' is specified)
 * 		 / '+'
 *
 * field_width := 1*DIGIT
 * prec := '.' [ '-' ] *DIGIT
 * conversion_specifier := 'd' / 'i' / 'o' / 'u' / 'x' / 'X'
 * 		 / 'e' / 'E' / 'f' / 'F' / 'g' / 'G'
 * 		 / 'c' / 's'
 * 		 / 'p'
 * */

/*
 * flag := '0' ; the value should be zero padded (ignored when 'prec' is specified)
 * 	 / '-' ; the value is left-justified
 * 	 / ' ' ; the value should be padded whith spaces (ignored when 'prec' is specified)
 * 	 / '+'
 */

namespace pfs {

/*       Value     сompat_gcc         compat_msc
 * 	%p   123ABC     0x123abc           00123ABC
 *  %+o	  -2875        -5473        37777772305
 */
enum safeformat_compat {
      safeformat_compat_gcc = 0
    , safeformat_compat_msc
    , safeformat_compat_msvc = safeformat_compat_msc
};

template <typename StringImplType
    , int Compat = safeformat_compat_gcc>
class safeformat
{
public:
    typedef string<StringImplType>               string_type;
	typedef typename string_type::const_iterator const_iterator;
	typedef typename string_type::value_type     char_type;

public:
	enum Flag {
		  NoFlag
		, ZeroPadding          = 0x0001 // The value should be zero padded.
		                                // For d, i, o, u, x, X, a, A, e, E, f, F, g, and G conversions,
		                                // the converted value is padded on the left with zeros rather
		                                // than blanks. If the 0 and - flags both appear, the 0 flag is ignored.
		                                // If a precision is given with a numeric conversion (d, i, o, u, x, and X),
		                                // the 0 flag is ignored.  For other conversions, the behavior is undefined.
		, LeftJustify          = 0x0002 // The  converted  value  is  to  be  left adjusted on the field boundary.
		                                // (The default is right justification.)  Except for n conversions,
		                                // the converted value is padded on the right with blanks, rather than on
		                                // the left with blanks or zeros.  A - overrides a 0 if both are given.
		, SpaceBeforePositive  = 0x0004 // Left space before positive number
		, NeedSign             = 0x0008 // A sign (+ or -) should always be placed before a number produced by a signed conversion.
		                                // By default a sign is used only for negative numbers.
		                                // @c NeedSign overrides a @c SpaceBeforePositive if both are used.
		, Alternate            = 0x0010 // The value should be converted to an "alternate form".
										// For o conversions, the first character of the output  string
        								// is  made  zero  (by  prefixing  a 0 if it was not zero already).
										// For x and X conversions, a nonzero result has the
        								// string "0x" (or "0X" for X conversions) prepended to it.
										// For a, A, e, E, f, F, g, and G  conversions,  the  result
        								// will  always contain a decimal point, even if no digits follow it
										// (normally, a decimal point appears in the results
        								// of those conversions only if a digit follows).
										// For g and G conversions, trailing zeros are not removed from the
        								// result as they would otherwise be.  For other conversions, the result is undefined.
	};

	struct conversion_spec
	{
		int       flags;
		int       width;
		int       prec; // The default precision is 1
		char_type spec_char;
	};

	struct context
	{
		string_type     format;
		const_iterator  pos; // current position in format string
		string_type     result;
		conversion_spec spec;
	};

private:
	context _ctx;

private:
	// Deny copy
	safeformat (const safeformat & );
	safeformat & operator = (const safeformat & sf);

private:
	void advance ()
    {
        const_iterator pos(_ctx.pos);
        const_iterator end(_ctx.format.cend());

        while (pos < end && *pos != '%') {
            _ctx.result.push_back(*pos);
            ++pos;
        }
        _ctx.pos = pos;
    }
    
	void clear_spec ()
	{
		_ctx.spec.flags     = safeformat::NoFlag;
		_ctx.spec.width     = 0;
		_ctx.spec.prec      = 1;
		_ctx.spec.spec_char = char_type(char(0));
	}

	bool is_digit (char_type v)
	{
		return pfs::is_digit<char_type>(v);
	}

	bool is_digit_exclude_zero (char_type v)
	{
		return (is_digit(v) && v != '0');
	}

	bool parse_percent_char ()
    {
        const_iterator pos(_ctx.pos);
        const_iterator end(_ctx.format.cend());

        if (pos < end && *pos == '%') {
            ++pos;
            if (pos < end && *pos == '%') {
                _ctx.result.append(1, '%');
                ++pos;
                _ctx.pos = pos;
                return true;
            }
        }
        return false;        
    }
    
	bool parse_flags ()
    {
        const_iterator pos(_ctx.pos);
        const_iterator end(_ctx.format.cend());

        if (pos == end)
            return false;

        if (*pos != '%')
            return false;

        ++pos;

        while (pos < end) {
            if (*pos == '0') {
                set_zero_padding();
            } else if (*pos == '-') {
                set_left_justify();
            } else if (*pos == ' ') {
                set_space_before_positive();
            } else if (*pos == '+') {
                set_need_sign();
            } else if (*pos == '#') {
                set_alternate();
            } else {
                break;
            }
            ++pos;
        }

        _ctx.pos = pos;
        return true;        
    }
    
	bool parse_field_width ()
    {
        const_iterator pos(_ctx.pos);
        const_iterator end(_ctx.format.cend());

        if (is_digit_exclude_zero(*pos)) {
            intmax_t width = strtointmax(pos, end
                    , 10
                    , numeric_limits<intmax_t>::min()
                    , numeric_limits<uintmax_t>::max()
                    , & pos);

            PFS_ASSERT(!errno && width >= 0 && width <= numeric_limits<int>::max()); // TODO need warning only instead of assertion

            _ctx.pos = pos;
            set_field_width(int(width));
        }
        return true;        
    }

    /*
     * prec := '.' [ '-' ] *DIGIT
     *
     * If the precision is given as just '.', the precision is taken to be zero.
     * A negative precision is taken as if the precision were omitted.
     */
	bool parse_precision ()
    {
        const_iterator pos(_ctx.pos);
        const_iterator end(_ctx.format.cend());
        int sign = 1;
        intmax_t prec = -1;

        if (pos < end && *pos == '.')
            ++pos;

        if (pos < end && *pos == '-') {
            sign = -1;
            ++pos;
        }

        if (is_digit(*pos)) {
            prec = strtointmax(pos, end
                    , 10
                    , min_value<intmax_t>()
                    , max_value<uintmax_t>()
                    , & pos);
            PFS_ASSERT(!errno && prec >= 0 && prec <= max_value<int>()); // TODO need warning only instead of assertion
        }

        if (sign > 0)
            set_precision(int(prec));

        _ctx.pos = pos;
        return true;        
    }

    /*
     * conversion_specifier := 'd' / 'i' / 'o' / 'u' / 'x' / 'X'
     * 		 / 'e' / 'E' / 'f' / 'F' / 'g' / 'G'
     * 		 / 'c' / 's'
     * 		 / 'p'
     */    
	bool parse_conv_spec ()
    {
        const_iterator pos(_ctx.pos);
        const_iterator end(_ctx.format.cend());

        if (pos < end) {
            string convSpecifiers("diouxXeEfFgGcsp");

            PFS_ASSERT(convSpecifiers.contains(*pos)); // Expected conversion specifier: one of 'diouxXeEfFgGcsp';
            set_conv_specifier(char_type(*pos));
            ++pos;
        }

        _ctx.pos = pos;
        return true;        
    }

    /*
     * conversion_specification := '%' *flag [ field_width ] [ prec ] conversion_specifier
     */
	bool parse_spec ()
    {
        if (!parse_percent_char()) {
            if (parse_flags()
                    && parse_field_width()
                    && parse_precision()
                    && parse_conv_spec()) {
                ;
            }
        }

        if (_ctx.spec.spec_char != char_type(char(0))) {
            if (_ctx.spec.flags & safeformat::ZeroPadding) {

                // If the 0 and - flags both appear, the 0 flag is ignored.
                if (_ctx.spec.flags & safeformat::LeftJustify)
                    _ctx.spec.flags &= ~safeformat::ZeroPadding;

                // If a precision is given with a numeric conversion (d, i, o, u, x, and X), the 0 flag is ignored.
                if (_ctx.spec.prec > -1
                        && (_ctx.spec.spec_char == 'd'
                        || _ctx.spec.spec_char == 'i'
                        || _ctx.spec.spec_char == 'o'
                        || _ctx.spec.spec_char == 'u'
                        || _ctx.spec.spec_char == 'x'
                        || _ctx.spec.spec_char == 'X')) {
                    _ctx.spec.flags &= ~safeformat::ZeroPadding;
                }

                // '0' flag used with ‘%c’ or '%s' specifier in format string
                if (_ctx.spec.spec_char == 'c'
                        || _ctx.spec.spec_char == 's') {
                    _ctx.spec.flags &= ~safeformat::ZeroPadding;
                }

            }

            // A + overrides a space if both are used
            if ((_ctx.spec.flags & safeformat::NeedSign)
                    && (_ctx.spec.flags & safeformat::SpaceBeforePositive))
                _ctx.spec.flags &= ~safeformat::SpaceBeforePositive;

            // '+' flag used with '%c' or '%s' specifier in format string
            if ((_ctx.spec.flags & safeformat::NeedSign)
                    && (_ctx.spec.spec_char == 'c'
                    || _ctx.spec.spec_char == 's')) {
                _ctx.spec.flags &= ~safeformat::NeedSign;
            }


            return true;
        }
        return false;        
    }

	void set_zero_padding ()              { _ctx.spec.flags |= ZeroPadding; }
	void set_left_justify ()              { _ctx.spec.flags |= LeftJustify; }
	void set_right_justify ()             { _ctx.spec.flags &= ~LeftJustify; }
	void set_space_before_positive ()     { _ctx.spec.flags |= SpaceBeforePositive; }
	void set_need_sign ()                 { _ctx.spec.flags |= NeedSign; }
	void set_alternate ()                 { _ctx.spec.flags |= Alternate; }
	void set_field_width (int w)          { _ctx.spec.width = w; }
	void set_precision (int p)            { _ctx.spec.prec = p; }
	void set_conv_specifier (char_type c) { _ctx.spec.spec_char = c; }

	void prepend_sign (string_type & r)
    {
        bool isNegative = r.starts_with("-");

        // When 0 is printed with an explicit precision 0, the output is empty.
        if (_ctx.spec.prec == 0 && r == string_type("0"))
            r.clear();

        // The precision, if any, gives the minimum number of digits that must appear;
        // if the converted value requires fewer digits, it is padded on the left with zeros.
        if (_ctx.spec.prec > 0 && r.length() < size_t(_ctx.spec.prec))
            r.prepend(string_type(_ctx.spec.prec - r.length(), '0'));

        if (!isNegative) {
            // A sign (+ or -) should always be placed before a number produced by a signed conversion
            if (_ctx.spec.flags & NeedSign) {
                r.prepend(1, '+');
            }                // A blank should be left before a positive number
            else if (_ctx.spec.flags & SpaceBeforePositive) {
                r.prepend(1, ' ');
            }
        }        
    }
    
	void do_padding (string_type & r)
    {
        PFS_ASSERT(_ctx.spec.width >= 0);

        if (r.length() < size_t(_ctx.spec.width)) {
            size_t count = size_t(_ctx.spec.width) - r.length();
            char paddingChar = (_ctx.spec.flags & safeformat::ZeroPadding) ? '0' : ' ';

            if (_ctx.spec.flags & safeformat::LeftJustify)
                r.append(string(count, paddingChar));
            else
                r.prepend(count, paddingChar);
        }        
    }

	//safeformat & arg (const __sf_base_traits * v);

private:    
    safeformat ();
    
public:
	explicit safeformat (string_type const & format)
	{
		_ctx.format = format;
		_ctx.pos = _ctx.format.cbegin();
		clear_spec();
	}

	explicit safeformat (const char * latin1Format)
	{
		_ctx.format = string_type(latin1Format);
		_ctx.pos = _ctx.format.cbegin();
		clear_spec();
	}

//	template <typename T>
//	safeformat & operator () (T const & v)
//	{
//		__sf_traits<T> t(v);
//		return arg(& t);
//	}
    
//    template <typename T>
//  	safeformat & operator () (T const * p)
//    {
//        return operator () (static_cast<void const *>(p));
//    }

//    template <typename T>
//  	safeformat & operator ()<T *> (T * p)
//    {
//        return operator () (static_cast<void const *>(p));
//    }

	safeformat & operator () (char c);
	safeformat & operator () (signed char n);
	safeformat & operator () (unsigned char n);
	safeformat & operator () (short n);
	safeformat & operator () (unsigned short n);
	safeformat & operator () (int n);
	safeformat & operator () (unsigned int n);
	safeformat & operator () (long n);
	safeformat & operator () (unsigned long n);

#ifdef PFS_HAVE_LONGLONG
	safeformat & operator () (long long n);
	safeformat & operator () (unsigned long long n);
#endif

	safeformat & operator () (float n);
	safeformat & operator () (double n);

#ifdef PFS_HAVE_LONG_DOUBLE
	safeformat & operator () (long double n);
#endif

//	safeformat & operator () (typename string_type::value_type c);
	safeformat & operator () (string_type const & s);
	safeformat & operator () (char const * s)
	{
		string_type ss(s);
		return operator () (ss);
	}
    
    safeformat & operator () (void const * p);

	string_type const & operator () ()
	{
		return str();
	}

	string_type const & str ()
	{
		advance();
		return _ctx.result;
	}

//--- boost-like operators
//	safeformat & operator % (char c)               { return operator () (c); }
	safeformat & operator % (signed char n)        { return operator () (n); }
	safeformat & operator % (unsigned char n)      { return operator () (n); }
	safeformat & operator % (short n)              { return operator () (n); }
	safeformat & operator % (unsigned short n)     { return operator () (n); }
	safeformat & operator % (int n)                { return operator () (n); }
	safeformat & operator % (unsigned int n)       { return operator () (n); }
	safeformat & operator % (long n)               { return operator () (n); }
	safeformat & operator % (unsigned long n)      { return operator () (n); }

#ifdef PFS_HAVE_LONGLONG
	safeformat & operator % (long long n)          { return operator () (n); }
	safeformat & operator % (unsigned long long n) { return operator () (n); }
#endif

	safeformat & operator % (float n)              { return operator () (n); }
	safeformat & operator % (double n)             { return operator () (n); }

#ifdef PFS_HAVE_LONG_DOUBLE
	safeformat & operator % (long double n)        { return operator () (n); }
#endif

//	safeformat & operator % (typename string_type::value_type c) { return operator () (c); }
	safeformat & operator % (string_type const & s) { return operator () (s); }
	safeformat & operator % (const char * s)        { return operator () (s); }
	safeformat & operator % (void const * p)        { return operator () (p); }

//--- Qt-like methods
	safeformat & arg (char c)               { return operator () (c); }
	safeformat & arg (signed char n)        { return operator () (n); }
	safeformat & arg (unsigned char n)      { return operator () (n); }
	safeformat & arg (short n)              { return operator () (n); }
	safeformat & arg (unsigned short n)     { return operator () (n); }
	safeformat & arg (int n)                { return operator () (n); }
	safeformat & arg (unsigned int n)       { return operator () (n); }
	safeformat & arg (long n)               { return operator () (n); }
	safeformat & arg (unsigned long n)      { return operator () (n); }

#ifdef PFS_HAVE_LONGLONG
	safeformat & arg (long long n)          { return operator () (n); }
	safeformat & arg (unsigned long long n) { return operator () (n); }
#endif

	safeformat & arg (float n)              { return operator () (n); }
	safeformat & arg (double n)             { return operator () (n); }

#ifdef PFS_HAVE_LONG_DOUBLE
	safeformat & arg (long double n)        { return operator () (n); }
#endif

//	safeformat & arg (typename string_type::value_type c) { return operator () (c); }
	safeformat & arg (string_type const & s) { return operator () (s); }
	safeformat & arg (const char * s)        { return operator () (s); }
	safeformat & arg (void const * p)        { return operator () (p); }
};

} // pfs

#endif /* __PFS_SAFEFORMAT_HPP__ */

