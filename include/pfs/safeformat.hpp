
/* 
 * File:   safeformat.hpp
 * Author: wladt
 *
 * Created on July 6, 2017, 4:09 PM
 */

#ifndef __PFS_SAFEFORMAT_HPP__
#define __PFS_SAFEFORMAT_HPP__

namespace pfs {

template <typename StringT>
class safeformat
{
public:
    typedef StringT string_type;
	typedef typename string_type::const_iterator iterator;
	typedef typename string_type::value_type char_type;

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

	/*       Value     сompat_gcc         compat_msc
	 * 	%p   123ABC     0x123abc           00123ABC
	 *  %+o	  -2875        -5473        37777772305
	 */
	enum compat_enum {
		  compat_gcc = 0
		, compat_msc
		, compat_msvc = compat_msc
	};

	struct conversion_spec
	{
		int    flags;
		int    width;
		int    prec; // The default precision is 1
		char_type spec_char;
	};

	struct context
	{
		string_type     format;
		iterator        pos; // current position in format string
		string_type     result;
		conversion_spec spec;
		compat_enum     compat;
	};

private:
	context _ctx;

private:
	// Deny copy
	safeformat (const safeformat & );
	safeformat & operator = (const safeformat & sf);

private:
	void advance ();
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

	bool parse_percent_char ();
	bool parse_flags ();
	bool parse_field_width ();
	bool parse_precision ();
	bool parse_conv_spec ();
	bool parse_spec ();

	void set_zero_padding ()              { _ctx.spec.flags |= ZeroPadding; }
	void set_left_justify ()              { _ctx.spec.flags |= LeftJustify; }
	void set_right_justify ()             { _ctx.spec.flags &= ~LeftJustify; }
	void set_space_before_positive ()     { _ctx.spec.flags |= SpaceBeforePositive; }
	void set_need_sign ()                 { _ctx.spec.flags |= NeedSign; }
	void set_alternate ()                 { _ctx.spec.flags |= Alternate; }
	void set_field_width (int w)          { _ctx.spec.width = w; }
	void set_precision (int p)            { _ctx.spec.prec = p; }
	void set_conv_specifier (char_type c) { _ctx.spec.spec_char = c; }

	void prepend_sign (string_type & r);
	void do_padding (string_type & r);

	//safeformat & arg (const __sf_base_traits * v);

private:    
    safeformat ();
    
public:
	explicit safeformat (string_type const & format)
	{
		_ctx.format = format;
		_ctx.pos = _ctx.format.cbegin();
		set_compat((compat_enum)global_compat());
		clear_spec();
	}

	explicit safeformat (const char * latin1Format)
	{
		_ctx.format = string_type(latin1Format);
		_ctx.pos = _ctx.format.cbegin();
		set_compat((compat_enum)global_compat());
		clear_spec();
	}

	/**
	 * @brief Set compatibility flag.
	 *
	 * @param c Compatibility flag to set.
	 * @return Instance itself.
	 */
	safeformat & set_compat (compat_enum c) { _ctx.compat = c; return *this; }

	/**
	 * @brief Set compatibility flag globally
	 *
	 * @param c Compatibility flag to set.
	 */
	static void set_global_compat (compat_enum c);

	/**
	 * @brief Set global compatibility flag.
	 *
	 * @return Compatibility flag.
	 */
	static int global_compat ();

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

