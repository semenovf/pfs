
/* 
 * File:   safeformat.hpp
 * Author: wladt
 *
 * Created on July 6, 2017, 4:09 PM
 */

#ifndef __PFS_SAFEFORMAT_HPP__
#define __PFS_SAFEFORMAT_HPP__

#include <cstdio> // sprintf
#include <pfs/ctype.hpp>
#include <pfs/types.hpp>
#include <pfs/string.hpp>
#include <pfs/limits.hpp>
#include <pfs/exception.hpp>
#include <pfs/iterator.hpp>
#include <pfs/lexical_cast.hpp>
//#include "cxx/memory.hpp"

//
// [Formatted Output](http://www.qnx.com/developers/docs/6.5.0/topic/com.qnx.doc.dinkum_en_c99/lib_prin.html)
// [Format Specification Syntax: printf and wprintf Functions](https://msdn.microsoft.com/en-us/en-en/library/56e442dc.aspx)
//

/* Conversion specification grammar
 *==============================================================================
 * conversion_specification := '%' flags [ field_width ] [ prec ] [length_mod] conversion_specifier
 * 
 * flags = *flag
 * flag  = / '-' / '+' / ' ' / '#' / '0'
 *
 * field_width = '*' / 1*DIGIT ; // !!! asterisk unsupported yet
 * 
 * prec = '.' ?( '*' / ['-'] 1*DIGIT ) // !!! asterisk unsupported yet
 * 
 * length_mod = 'hh' / 'h' / 'll' / 'l' / 'j' / 'z' / 't' / 'L'
 * 
 * conversion_specifier = 'd' / 'i' / 'o' / 'u' / 'x' / 'X'
 *                      / 'f' / 'F' / 'e' / 'E' / 'g' / 'G'
 *                      / 'a' / 'A' / 'c' / 's' / 'p' / 'n'
 * 
 */

namespace pfs {

/*       Value     сompat_gcc         compat_msc
 * 	%p   123ABC     0x123abc           00123ABC
 *  %+o	  -2875        -5473        37777772305
 */
enum safeformat_compat
{
      safeformat_compat_gcc = 0
    , safeformat_compat_msc
    , safeformat_compat_msvc = safeformat_compat_msc
};

struct conversion_specification
{
    //
    // Flags
    //==========================================================================
    //
    static int const NO_FLAG = 0;

    // '-'
    //--------------------------------------------------------------------------
    // The result of the conversion is left-justified within the field. 
    // (It is right-justified if this flag is not specified.)
    static int const FL_LEFT_JUSTIFIED = 0x0001;

    // '+'
    //--------------------------------------------------------------------------
    // The result of a signed conversion always begins with a plus or minus sign. 
    // (It begins with a sign only when a negative value is converted 
    // if this flag is not specified)
    static int const FL_NEED_SIGN      = 0x0002;

    // '<space>'
    //--------------------------------------------------------------------------
    // If the first character of a signed conversion is not a sign, or if a 
    // signed conversion results in no characters, a space is prefixed to the 
    // result. If the 'space' and '+' flags both appear, the space flag is ignored.
    static int const FL_SPACE_PADDING = 0x0004;

    // '#'
    //--------------------------------------------------------------------------
    // The result is converted to an 'alternative form'. For 'o' conversion, 
    // it increases the precision, if and only if necessary, to force the 
    // first digit of the result to be a zero (if the value and precision 
    // are both 0, a single 0 is printed). For 'x' (or 'X') conversion, 
    // a nonzero result has '0x' (or '0X') prefixed to it. For 'a', 'A', 'e',
    // 'E', 'f', 'F', 'g', and 'G' conversions, the result of converting a 
    // floating-point number always contains a decimal-point character, 
    // even if no digits follow it. (Normally, a decimal-point character 
    // appears in the result of these conversions only if a digit follows it).
    // For 'g' and 'G' conversions, trailing zeros are not removed from the
    // result. For other conversions, the behavior is undefined.
    static int const FL_ALTERN_FORM = 0x0008;

    // '0'
    //--------------------------------------------------------------------------
    // For 'd', 'i', 'o', 'u', 'x', 'X', 'a', 'A', 'e', 'E', 'f', 'F', 'g', 
    // and 'G' conversions, leading zeros (following any indication of sign 
    // or base) are used to pad to the field width rather than performing space 
    // padding, except when converting an infinity or NaN. If the '0' and '-' 
    // flags both appear, the '0' flag is ignored. For 'd', 'i', 'o', 'u', 'x',
    // and 'X' conversions, if a precision is specified, the '0' flag is 
    // ignored. For other conversions, the behavior is undefined.
    static int const FL_ZERO_PADDING = 0x0010;
    //==========================================================================
    
    
    //
    // Length modifiers
    //==========================================================================
    //

    // 'hh'
    //--------------------------------------------------------------------------
    // Specifies that a following 'd', 'i', 'o', 'u', 'x', or 'X' conversion
    // specifier applies to a 'signed char' or 'unsigned char' argument 
    // (the argument will have been promoted according to the integer 
    // promotions, but its value shall be converted to 'signed char' 
    // or 'unsigned char' before printing); or that a following 'n' conversion 
    // specifier applies to a pointer to a 'signed char' argument.
    static int const LM_APPLY_CHAR       = 1;

    // 'h'
    //--------------------------------------------------------------------------
    static int const LM_APPLY_SHORT      = 2;

    // 'l' (ell)
    //--------------------------------------------------------------------------
    static int const LM_APPLY_LONG       = 3;

    // 'll' (ell-ell)
    //--------------------------------------------------------------------------
    static int const LM_APPLY_LONGLONG   = 4;

    // 'j'
    //--------------------------------------------------------------------------
    static int const LM_APPLY_INTMAX     = 5;

    // 'z'
    //--------------------------------------------------------------------------
    static int const LM_APPLY_SIZE_T     = 6;

    // 't'
    //--------------------------------------------------------------------------
    static int const LM_APPLY_PTRDIFF    = 7;

    // 'L'
    //--------------------------------------------------------------------------
    static int const LM_APPLY_LONGDOUBLE = 8;
    //==========================================================================
    
    bool good;
    char spec_char;
    int  flags;

    bool field_width_asterisk; // unsupported yet
    int field_width;

    bool prec_asterisk; // unsupported yet
    int prec;
    int prec_sign;

    int length_mod;

    conversion_specification ()
        : good(true)
        , spec_char(0)
        , flags(0)
        , field_width_asterisk(false)
        , field_width(0)
        , prec_asterisk(false)
        , prec(0)
        , prec_sign(1)
    {}
};

template <typename T>
struct printf_length_modifier
{
    static char const * value () { return ""; } 
};

template <> struct printf_length_modifier<char>               { static char const * value () { return "hh"; } };
template <> struct printf_length_modifier<signed char>        { static char const * value () { return "hh"; } };
template <> struct printf_length_modifier<unsigned char>      { static char const * value () { return "hh"; } };
template <> struct printf_length_modifier<short int>          { static char const * value () { return "h"; } };
template <> struct printf_length_modifier<unsigned short int> { static char const * value () { return "h"; } };
template <> struct printf_length_modifier<int>                { static char const * value () { return ""; } };
template <> struct printf_length_modifier<unsigned int>       { static char const * value () { return ""; } };
template <> struct printf_length_modifier<long int>           { static char const * value () { return "l"; } };
template <> struct printf_length_modifier<unsigned long int>  { static char const * value () { return "l"; } };

#if PFS_HAVE_LONG_LONG
template <> struct printf_length_modifier<long long int>          { static char const * value () { return "ll"; } };
template <> struct printf_length_modifier<unsigned long long int> { static char const * value () { return "ll"; } };
#endif

template <> struct printf_length_modifier<float>  { static char const * value () { return ""; } };
template <> struct printf_length_modifier<double> { static char const * value () { return ""; } };

#if PFS_HAVE_LONG_DOUBLE
template <> struct printf_length_modifier<long double> { static char const * value () { return "L"; } };
#endif

template <typename BackInsertIt>
struct base_stringifier
{
    typedef BackInsertIt back_insert_iterator;
    
    void prepare_format (char * format
            , char const * length_modifier
            , conversion_specification const & conv_spec) const
    {
        char * p = format;
        
        *p++ = '%';
        //
        // Flags
        //
        if (conv_spec.flags & conversion_specification::FL_LEFT_JUSTIFIED)
            *p++ = '-';
        
        if (conv_spec.flags & conversion_specification::FL_NEED_SIGN)
            *p++ = '+';
        
        //
        // There is no matter the placement order of '0' and ' ' flags
        // printf("%0 6d", 123) => ' 00123'
        // printf("% 06d", 123) => ' 00123'
        //
        if (conv_spec.flags & conversion_specification::FL_ZERO_PADDING)
            *p++ = '0';
        
        if (conv_spec.flags & conversion_specification::FL_SPACE_PADDING)
            *p++ = ' ';
        
        if (conv_spec.flags & conversion_specification::FL_ALTERN_FORM)
            *p++ = '#';
        
        //*p = '\x0';
        
        //
        // Field width
        //
        if (conv_spec.field_width > 0)
            p += sprintf(p, "%d", conv_spec.field_width);
        
        //
        // Precision
        //
        if (conv_spec.prec_sign < 0 || conv_spec.prec > 0)
            *p++ = '.';
        
        if (conv_spec.prec_sign < 0)
            *p++ = '-';
        
        if (conv_spec.prec > 0)
            p += sprintf(p, "%d", conv_spec.prec);

        //
        // Length modifiers (determine from actual integer type)
        //
        p += sprintf(p, "%s", length_modifier);
        
        //
        // Conversion specifier
        //
        *p++ = conv_spec.spec_char;
        
        *p = '\x0';
    }
    
    virtual void stringify (back_insert_iterator out
            , conversion_specification const & conv_spec) const = 0;
};

template <typename BackInsertIt, typename T>
struct stringifier : public base_stringifier<BackInsertIt>
{
    typedef BackInsertIt back_insert_iterator;
    
    T const & val;
    
    stringifier (T const & v) : val(v) {}
    
    virtual void stringify (back_insert_iterator out
            , conversion_specification const & conv_spec) const
    {
        static size_t const SPRINTF_CONV_SPEC_LENGTH =  1                /* '%' */                  \
                                 + 5                /* flags */                \
                                 + sizeof(int)      /* field-width */          \
                                 + 2 + sizeof(int)  /* precision */            \
                                 + 2                /* length-modifier */      \
                                 + 1                /* conversion-specifier */ \
                                 + 1;                /*'\x0' */
        static size_t const SNPRINTF_DEFAULT_BUFSZ = 64;
        
        char format[SPRINTF_CONV_SPEC_LENGTH];
        this->prepare_format(format, printf_length_modifier<T>::value(), conv_spec);
                
        size_t bufsz = SNPRINTF_DEFAULT_BUFSZ;
        char buf[SNPRINTF_DEFAULT_BUFSZ];
        char * pbuf = buf;
        
        // The glibc implementation of the functions snprintf() and vsnprintf()
        // conforms to the C99 standard, that is, behaves as described above, 
        // since glibc version 2.1. Until glibc 2.0.6 they would return -1 
        // when the output was truncated.

#if PFS_CC_MSC
        int written = _snprintf(buf, bufsz, format, this->val);
#else
      	// FIXME Need the recognition of GLIBC version.
    	// Supporting modern behavior only now.
        int written = snprintf(buf, bufsz, format, this->val);
#endif
        if (written < 0)
            throw pfs::runtime_error("safeformat: snprintf() error (a negative value is returned)");
        
        // A return value of size or more means that the output was truncated.
        //
        if (static_cast<size_t>(written) >= SNPRINTF_DEFAULT_BUFSZ) {
            bufsz = static_cast<size_t>(written) + 1;
            pbuf = pfs::allocator<char>().allocate(bufsz);
            
#if PFS_CC_MSC
            written = _snprintf(pbuf, bufsz, format, this->val);
#else
            written = snprintf(pbuf, bufsz, format, this->val);
#endif
            
            if (written < 0)
                throw pfs::runtime_error("safeformat: snprintf() error (a negative value is returned)");

            PFS_ASSERT(static_cast<size_t>(written) < bufsz);
        }

        char * p = pbuf;
        
        // Copy chars into container (using back insert iterator)
        while (*p)
            *out++ = *p++;
        
        // Was dynamically allocated
        //
        if (bufsz > SNPRINTF_DEFAULT_BUFSZ)
            pfs::allocator<char>().destroy(pbuf);
    }
};

template <typename BackInsertIt, typename StringType>
struct string_stringifier : public base_stringifier<BackInsertIt>
{
    typedef BackInsertIt back_insert_iterator;
    typedef StringType string_type;
    
    string_type const & val;
    
    string_stringifier (string_type const & v) : val(v) {}
    
    virtual void stringify (back_insert_iterator out
            , conversion_specification const & conv_spec) const
    {
        switch (conv_spec.spec_char) {
        case 'd': case 'i': case 'o': case 'u': case 'x': case 'X':
        case 'f': case 'F': case 'e': case 'E': case 'g': case 'G':
        case 'a': case 'A': case 'c': case 'p': /*case 'n':*/
            stringifier<BackInsertIt, void const *>(val.data()).stringify(out, conv_spec);
            return;
            
        case 's':
            break;
        }
        
        // Do padding
        //
        size_t vallen = val.length();
        size_t padding_count = 0;
        typename string_type::value_type padding_char = ' ';

        if (vallen < static_cast<size_t>(conv_spec.field_width)) {
            padding_count = static_cast<size_t>(conv_spec.field_width) - vallen;
            
            if (conv_spec.flags & conversion_specification::FL_ZERO_PADDING
                    && conv_spec.flags & conversion_specification::FL_SPACE_PADDING) {
                *out++ = ' ';
                padding_char = '0';
                --padding_count;
            } else if (conv_spec.flags & conversion_specification::FL_SPACE_PADDING) {
                padding_char = ' ';
            } else if (conv_spec.flags & conversion_specification::FL_ZERO_PADDING) {
                padding_char = '0';
            }
        }

        if (conv_spec.flags & conversion_specification::FL_LEFT_JUSTIFIED) {
            for (typename string_type::const_iterator it = val.cbegin()
                    ; it != val.cend(); ++it) {
                *out++ = *it;
            }

            while (padding_count--)
                *out++ = padding_char;
        } else {
            while (padding_count--)
                *out++ = padding_char;

            for (typename string_type::const_iterator it = val.cbegin()
                    ; it != val.cend(); ++it) {
                *out++ = *it;
            }
        }
    }
};

template <typename StringType
        , typename BackInserterContainer = StringType
        , int Compat = safeformat_compat_gcc>
class safeformat
{
    typedef BackInserterContainer                result_type;
    typedef back_insert_iterator<result_type>    back_inserter_type;
    typedef StringType                           string_type;
    typedef typename string_type::const_iterator const_iterator;
    typedef typename string_type::value_type     value_type;
    
    // Stores intermediate result (and complete at the ends)
    result_type        _result;
    back_inserter_type _out;
    
    // Current position at format string
    const_iterator _p;
    
    // End position at format string
    const_iterator _end;
    
public:
    safeformat (string_type const & format)
        : _out(_result)
        , _p(format.cbegin())
        , _end(format.cend())
    {}

private:
    void parse_regular_chars ()
    {
        while (_p != _end && to_ascii<value_type>(*_p) != '%')
            *_out++ = *_p++;
    }
    
    void finalize ()
    {
        // TODO replace by string::append(_p, _end) after implementing according method
        
        while (_p != _end)
            *_out++ = *_p++;
    }
    
    // flags = *flag
    // flag  = / '-' / '+' / ' ' / '#' / '0'
    void parse_spec_flags (conversion_specification * conv_spec)
    {
        if (! conv_spec->good)
            return;

        while (_p != _end) {
            switch (to_ascii<value_type>(*_p)) {
            case '-':
                conv_spec->flags |= conversion_specification::FL_LEFT_JUSTIFIED;
                break;
            case '+':
                conv_spec->flags |= conversion_specification::FL_NEED_SIGN;
                break;
            case ' ':
                conv_spec->flags |= conversion_specification::FL_SPACE_PADDING;
                break;
            case '#':
                conv_spec->flags |= conversion_specification::FL_ALTERN_FORM;
                break;
            case '0':
                conv_spec->flags |= conversion_specification::FL_ZERO_PADDING;
                break;
            default:
                return;
            }
            
            ++_p;
        }
        
        // If the '0' and '-' flags both appear, the '0' flag is ignored.
        if (conv_spec->flags & conversion_specification::FL_LEFT_JUSTIFIED) {
            conv_spec->flags &= ~conversion_specification::FL_ZERO_PADDING;
        }
    }
    
    void parse_spec_field_width (conversion_specification * conv_spec)
    {
        if (! conv_spec->good)
            return;
            
        if (to_ascii<value_type>(*_p) == '*') {
            conv_spec->field_width_asterisk = true;
            ++_p;
            
            conv_spec->good = false; // asterisk unsupported yet
            return;
        }
        
        if (is_digit(*_p) && to_ascii<value_type>(*_p) != '0') {
            const_iterator first = _p;
            ++_p;
            
            while (_p != _end && is_digit(*_p))
                ++_p;
            
            if (_p != first) {
                const_iterator badpos;
                conv_spec->field_width = string_to_int<int, const_iterator>(first, _p, & badpos, 10);

                // bad value for field_width
                if (badpos != _p)
                    conv_spec->good = false;
            }
        }
    }
    
    void parse_spec_precision (conversion_specification * conv_spec)
    {
        if (! conv_spec->good)
            return;
        
        if (to_ascii<value_type>(*_p) != '.')
            return;
        
        ++_p;
        
        if (_p == _end)
            return;
        
        if (to_ascii<value_type>(*_p) == '*') {
            conv_spec->prec_asterisk = true;
            ++_p;
            
            conv_spec->good = false; // asterisk unsupported yet
            return;
        }
        
        if (to_ascii<value_type>(*_p) != '-') {
            conv_spec->prec_sign = -1;
            ++_p;

            if (_p == _end)
                return;
        }
        
        
        if (is_digit(*_p) && to_ascii<value_type>(*_p) != '0') {
            const_iterator first = _p;
            ++_p;
            
            while (_p != _end && is_digit(*_p))
                ++_p;
            
            if (_p != first) {
                const_iterator badpos;
                conv_spec->prec = string_to_int<int, const_iterator>(first, _p, & badpos, 10);

                // bad value for precision
                if (badpos != _p)
                    conv_spec->good = false;
            }
        }
    }

    void parse_spec_length_modifier (conversion_specification * conv_spec)
    {
        if (! conv_spec->good)
            return;

        switch (to_ascii<value_type>(*_p)) {
        case 'h':
            conv_spec->length_mod = conversion_specification::LM_APPLY_SHORT;
            ++_p;
            
            if (_p != _end && *_p == value_type('h')) {
                conv_spec->length_mod = conversion_specification::LM_APPLY_CHAR;
                ++_p;
            }

            break;

            case 'l':
                conv_spec->length_mod = conversion_specification::LM_APPLY_LONG;
                ++_p;

                if (_p != _end && *_p == value_type('l')) {
                    conv_spec->length_mod = conversion_specification::LM_APPLY_LONGLONG;
                    ++_p;
                }

                break;
                    
            case 'j':
                conv_spec->length_mod = conversion_specification::LM_APPLY_INTMAX;
                ++_p;
                break;
                   
            case 'z':
                conv_spec->length_mod = conversion_specification::LM_APPLY_SIZE_T;
                ++_p;
                break;

            case 't':
                conv_spec->length_mod = conversion_specification::LM_APPLY_PTRDIFF;
                ++_p;
                break;

            case 'L':
                conv_spec->length_mod = conversion_specification::LM_APPLY_LONGDOUBLE;
                ++_p;
                break;
        }
    }
    
    void parse_spec_conversion_specifier (conversion_specification * conv_spec)
    {
        if (! conv_spec->good)
            return;

        switch (to_ascii<value_type>(*_p)) {
        case 'd': case 'i': case 'o': case 'u': case 'x': case 'X':
        case 'f': case 'F': case 'e': case 'E': case 'g': case 'G':
        case 'a': case 'A': case 'c': case 's': case 'p': /*case 'n':*/
            conv_spec->spec_char = to_ascii<value_type>(*_p);
            ++_p;
            break;
            
        case 'n': // unsupported yet
        default:
            conv_spec->good = false;
            break;
        }
    }
   
    void parse_conversion_specification (conversion_specification * conv_spec)
    {
        if (! conv_spec->good)
            return;

        if (to_ascii<value_type>(*_p) == '%') {
            ++_p;
            parse_spec_flags(conv_spec);
            parse_spec_field_width(conv_spec);
            parse_spec_precision(conv_spec);
            parse_spec_length_modifier(conv_spec);
            parse_spec_conversion_specifier(conv_spec);
        }
    }

    void advance (base_stringifier<back_inserter_type> const & stringifier)
    {
        parse_regular_chars();

        if (_p != _end) {
            conversion_specification conv_spec;
            parse_conversion_specification(& conv_spec);

            if (conv_spec.good) {
                process_conversion_specification(stringifier, conv_spec);
            } else {
                throw invalid_argument("safeformat: bad conversion specification");
            }
        }
    }

    void process_conversion_specification (
              base_stringifier<back_inserter_type> const & stringifier
            , conversion_specification const & conv_spec)
    {
        stringifier.stringify(_out, conv_spec);
    }
    
public:
    safeformat & operator () (char c)
    {
        advance(stringifier<back_inserter_type, char>(c));
        return *this;
    }
    
    safeformat & operator () (signed char n)
    {
        advance(stringifier<back_inserter_type, signed char>(n));
        return *this;
    }
    
    safeformat & operator () (unsigned char n)
    {
        advance(stringifier<back_inserter_type, unsigned char>(n));
        return *this;
    }
    
    safeformat & operator () (short n)
    {
        advance(stringifier<back_inserter_type, short>(n));
        return *this;
    }
    
    safeformat & operator () (unsigned short n)
    {
        advance(stringifier<back_inserter_type, unsigned short>(n));
        return *this;
    }
    
    safeformat & operator () (int n)
    {
        advance(stringifier<back_inserter_type, int>(n));
        return *this;
    }
    
    safeformat & operator () (unsigned int n)
    {
        advance(stringifier<back_inserter_type, unsigned int>(n));
        return *this;
    }
    
    safeformat & operator () (long n)
    {
        advance(stringifier<back_inserter_type, long>(n));
        return *this;
    }
    
    safeformat & operator () (unsigned long n)
    {
        advance(stringifier<back_inserter_type, unsigned long>(n));
        return *this;
    }
    
#ifdef PFS_HAVE_LONG_LONG
    safeformat & operator () (long long n)
    {
        advance(stringifier<back_inserter_type, long long>(n));
        return *this;
    }
    
    safeformat & operator () (unsigned long long n)
    {
        advance(stringifier<back_inserter_type, unsigned long long>(n));
        return *this;
    }
#endif

    safeformat & operator () (float n)
    {
        advance(stringifier<back_inserter_type, float>(n));
        return *this;
    }
    
    safeformat & operator () (double n)
    {
        advance(stringifier<back_inserter_type, double>(n));
        return *this;
    }
    
#ifdef PFS_HAVE_LONG_DOUBLE
    safeformat & operator () (long double n)
    {
        advance(stringifier<back_inserter_type, long double>(n));
        return *this;
    }
#endif

    safeformat & operator () (string_type const & s)
    {
        advance(string_stringifier<back_inserter_type, string_type>(s));
        return *this;
    }
    
    safeformat & operator () (typename string_type::const_native_reference s)
    {
        return operator() (string_type(s));
    }
    
    safeformat & operator () (char const * s)
    {
        string_type ss(s);
        return operator() (ss);
    }

    safeformat & operator () (void const * p)
    {
        advance(stringifier<back_inserter_type, void const *>(p));
        return *this;
    }
    
    operator string_type const & ()
    {
        return str();
    }

    //--- boost-like operators
    //	safeformat & operator % (char c)               { return operator () (c); }

    safeformat & operator % (signed char n)
    {
        return operator() (n);
    }

    safeformat & operator % (unsigned char n)
    {
        return operator() (n);
    }

    safeformat & operator % (short n)
    {
        return operator() (n);
    }

    safeformat & operator% (unsigned short n)
    {
        return operator() (n);
    }

    safeformat & operator % (int n)
    {
        return operator() (n);
    }

    safeformat & operator % (unsigned int n)
    {
        return operator() (n);
    }

    safeformat & operator % (long n)
    {
        return operator() (n);
    }

    safeformat & operator % (unsigned long n)
    {
        return operator() (n);
    }

#ifdef PFS_HAVE_LONG_LONG

    safeformat & operator % (long long n)
    {
        return operator() (n);
    }

    safeformat & operator % (unsigned long long n)
    {
        return operator() (n);
    }
#endif

    safeformat & operator % (float n)
    {
        return operator() (n);
    }

    safeformat & operator % (double n)
    {
        return operator() (n);
    }

#ifdef PFS_HAVE_LONG_DOUBLE

    safeformat & operator % (long double n)
    {
        return operator() (n);
    }
#endif

    //	safeformat & operator % (typename string_type::value_type c) { return operator () (c); }

    safeformat & operator % (string_type const & s)
    {
        return operator() (s);
    }

    safeformat & operator % (typename string_type::const_native_reference s)
    {
        return operator() (s);
    }

    safeformat & operator % (const char * s)
    {
        return operator() (s);
    }

    safeformat & operator % (void const * p)
    {
        return operator() (p);
    }
    
    //
    //--- Qt-like methods
    //
    safeformat & arg (char c)
    {
        return operator() (c);
    }

    safeformat & arg (signed char n)
    {
        return operator() (n);
    }

    safeformat & arg (unsigned char n)
    {
        return operator() (n);
    }

    safeformat & arg (short n)
    {
        return operator() (n);
    }

    safeformat & arg (unsigned short n)
    {
        return operator() (n);
    }

    safeformat & arg (int n)
    {
        return operator() (n);
    }

    safeformat & arg (unsigned int n)
    {
        return operator() (n);
    }

    safeformat & arg (long n)
    {
        return operator() (n);
    }

    safeformat & arg (unsigned long n)
    {
        return operator() (n);
    }

#ifdef PFS_HAVE_LONG_LONG

    safeformat & arg (long long n)
    {
        return operator() (n);
    }

    safeformat & arg (unsigned long long n)
    {
        return operator() (n);
    }
#endif

    safeformat & arg (float n)
    {
        return operator() (n);
    }

    safeformat & arg (double n)
    {
        return operator() (n);
    }

#ifdef PFS_HAVE_LONG_DOUBLE

    safeformat & arg (long double n)
    {
        return operator() (n);
    }
#endif

    safeformat & arg (string_type const & s)
    {
        return operator() (s);
    }

    safeformat & arg (typename string_type::const_native_reference s)
    {
        return operator() (s);
    }
    
    safeformat & arg (const char * s)
    {
        return operator() (s);
    }

    safeformat & arg (void const * p)
    {
        return operator() (p);
    }
    
    result_type const & str ()
    {
        finalize();
        return _result;
    }
};

#if __TODO__

template <typename StringImplType
        , int Compat = safeformat_compat_gcc>
class safeformat
{
public:
    typedef string<StringImplType>               string_type;
    typedef typename string_type::const_iterator const_iterator;
    typedef typename string_type::value_type     char_type;

private:
    typedef safeformat_iterator<StringImplType>  iterator;
        
private:
        string_type _format;
        iterator _it;
//        const_iterator pos; // current position in format string
        string_type _result;

private:
#if __cplusplus >= 201103L
    safeformat (const safeformat &) = delete;
    safeformat & operator= (const safeformat & sf) = delete;
#else
    // Deny copy
    safeformat (const safeformat &);
    safeformat & operator= (const safeformat & sf);
#endif

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
        _ctx.spec.flags = safeformat::NO_FLAG;
        _ctx.spec.width = 0;
        _ctx.spec.prec = 0;
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
            if (_ctx.spec.flags & safeformat::FL_ZERO_PADDING) {

                // If the 0 and - flags both appear, the 0 flag is ignored.
                if (_ctx.spec.flags & safeformat::LEFT_JUSTIFIED)
                    _ctx.spec.flags &= ~safeformat::FL_ZERO_PADDING;

                // If a precision is given with a numeric conversion (d, i, o, u, x, and X), the 0 flag is ignored.
                if (_ctx.spec.prec > -1
                        && (_ctx.spec.spec_char == 'd'
                        || _ctx.spec.spec_char == 'i'
                        || _ctx.spec.spec_char == 'o'
                        || _ctx.spec.spec_char == 'u'
                        || _ctx.spec.spec_char == 'x'
                        || _ctx.spec.spec_char == 'X')) {
                    _ctx.spec.flags &= ~safeformat::FL_ZERO_PADDING;
                }

                // '0' flag used with ‘%c’ or '%s' specifier in format string
                if (_ctx.spec.spec_char == 'c'
                        || _ctx.spec.spec_char == 's') {
                    _ctx.spec.flags &= ~safeformat::FL_ZERO_PADDING;
                }

            }

            // A + overrides a space if both are used
            if ((_ctx.spec.flags & safeformat::NEED_SIGN)
                    && (_ctx.spec.flags & safeformat::FL_SPACE_PADDING))
                _ctx.spec.flags &= ~safeformat::FL_SPACE_PADDING;

            // '+' flag used with '%c' or '%s' specifier in format string
            if ((_ctx.spec.flags & safeformat::NEED_SIGN)
                    && (_ctx.spec.spec_char == 'c'
                    || _ctx.spec.spec_char == 's')) {
                _ctx.spec.flags &= ~safeformat::NEED_SIGN;
            }


            return true;
        }
        return false;
    }

    void set_zero_padding ()
    {
        _ctx.spec.flags |= FL_ZERO_PADDING;
    }

    void set_left_justify ()
    {
        _ctx.spec.flags |= LEFT_JUSTIFIED;
    }

    void set_right_justify ()
    {
        _ctx.spec.flags &= ~LEFT_JUSTIFIED;
    }

    void set_space_before_positive ()
    {
        _ctx.spec.flags |= FL_SPACE_PADDING;
    }

    void set_need_sign ()
    {
        _ctx.spec.flags |= NEED_SIGN;
    }

    void set_alternate ()
    {
        _ctx.spec.flags |= FL_ALTERN_FORM;
    }

    void set_field_width (int w)
    {
        _ctx.spec.width = w;
    }

    void set_precision (int p)
    {
        _ctx.spec.prec = p;
    }

    void set_conv_specifier (char_type c)
    {
        _ctx.spec.spec_char = c;
    }

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
            if (_ctx.spec.flags & NEED_SIGN) {
                r.prepend(1, '+');
            }// A blank should be left before a positive number
            else if (_ctx.spec.flags & FL_SPACE_PADDING) {
                r.prepend(1, ' ');
            }
        }
    }

    void do_padding (string_type & r)
    {
        PFS_ASSERT(_ctx.spec.width >= 0);

        if (r.length() < size_t(_ctx.spec.width)) {
            size_t count = size_t(_ctx.spec.width) - r.length();
            char paddingChar = (_ctx.spec.flags & safeformat::FL_ZERO_PADDING) ? '0' : ' ';

            if (_ctx.spec.flags & safeformat::LEFT_JUSTIFIED)
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

};

#endif

} // pfs

#endif /* __PFS_SAFEFORMAT_HPP__ */

