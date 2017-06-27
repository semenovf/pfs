#ifndef __PFS_JSON_RFC4627_HPP__
#define __PFS_JSON_RFC4627_HPP__

#include <pfs/fsm/fsm.hpp>
#include <pfs/lexical_cast.hpp>
#include <pfs/traits/stack.hpp>
#include <pfs/traits/stdcxx/stack.hpp>
#include <pfs/unicode/unicode_iterator.hpp>
#include <pfs/json/constants.hpp>

//#include <pfs/mpl/algo/find.hpp>
//#include <pfs/stack.hpp>
//#include <pfs/fsm.hpp>
//#include <pfs/fsm/aliases.hpp>
//#include <pfs/string.hpp>
//#include <pfs/notification.hpp>
//#include "pfs/json/value.hpp"

namespace pfs { namespace json {

/*
   RFC 4627:  The application/json Media Type for JavaScript Object Notation (JSON).
   URL: http://www.ietf.org/rfc/rfc4627.txt
  -----------------------------------------------------------------------

	A JSON text is a sequence of tokens.  The set of tokens includes six
	structural characters, strings, numbers, and three literal names.

	A JSON text is a serialized object or array.

	JSON-text = object / array

	ws = *(
			%x20 /              ; Space
			%x09 /              ; Horizontal tab
			%x0A /              ; Line feed or New line
			%x0D                ; Carriage return
		)

	begin-array     = ws %x5B ws  ; [ left square bracket
	begin-object    = ws %x7B ws  ; { left curly bracket
	end-array       = ws %x5D ws  ; ] right square bracket
	end-object      = ws %x7D ws  ; } right curly bracket
	name-separator  = ws %x3A ws  ; : colon
	value-separator = ws %x2C ws  ; , comma

	Values.
	------------------------------------------------------------------
	A JSON value MUST be an object, array, number, or string, or one of
	the following three literal names:

		false null true

	The literal names MUST be lowercase.  No other literal names are
	allowed.

	value = false / null / true / object / array / number / string
	false = %x66.61.6c.73.65   ; false
	null  = %x6e.75.6c.6c      ; null
	true  = %x74.72.75.65      ; true


	Objects.
	------------------------------------------------------------------
	object = begin-object [ member *( value-separator member ) ] end-object

	member = string name-separator value

	Arrays.
	------------------------------------------------------------------
	array = begin-array [ value *( value-separator value ) ] end-array


	Numbers.
	------------------------------------------------------------------
    number = [ minus ] int [ frac ] [ exp ]
    decimal-point = %x2E       ; .
	digit1-9 = %x31-39         ; 1-9
	e = %x65 / %x45            ; e E
	exp = e [ minus / plus ] 1*DIGIT
	minus = %x2D               ; -
	plus = %x2B                ; +
	zero = %x30                ; 0
	frac = decimal-point 1*DIGIT
	int = zero / ( digit1-9 *DIGIT )

	Strings.
	------------------------------------------------------------------
	string = quotation-mark *char quotation-mark

	char = unescaped /
		escape (
			%x22 /          ; "    quotation mark  U+0022
			%x5C /          ; \    reverse solidus U+005C
			%x2F /          ; /    solidus         U+002F
			%x62 /          ; b    backspace       U+0008
			%x66 /          ; f    form feed       U+000C
			%x6E /          ; n    line feed       U+000A
			%x72 /          ; r    carriage return U+000D
			%x74 /          ; t    tab             U+0009
			%x75 4HEXDIG )  ; uXXXX                U+XXXX

	escape = %x5C              ; \

	quotation-mark = %x22      ; "

	unescaped = %x20-21 / %x23-5B / %x5D-10FFFF
*/

template <typename ValueT>
struct sax_context
{
    typedef typename ValueT::string_traits sequence_type;
    
    virtual bool on_begin_json   (void * user_context, data_type_t) = 0;
    virtual bool on_end_json     (void * user_context, data_type_t) = 0;
    virtual bool on_begin_object (void * user_context, sequence_type const &) = 0;
    virtual bool on_end_object   (void * user_context, sequence_type const &) = 0;
    virtual bool on_begin_array  (void * user_context, sequence_type const &) = 0;
    virtual bool on_end_array    (void * user_context, sequence_type const &) = 0;
    virtual bool on_null_value   (void * user_context, sequence_type const &) = 0;
    virtual bool on_boolean_value(void * user_context, sequence_type const &, bool) = 0;
    virtual bool on_number_value (void * user_context, sequence_type const &, real_t) = 0;
    virtual bool on_string_value (void * user_context, sequence_type const &, sequence_type const &) = 0;
};
    

template <typename ValueT, template <typename> class StackT = traits::stdcxx::stack>
struct grammar
{
    typedef typename ValueT::string_traits         sequence_type;
    typedef fsm::fsm<sequence_type>                fsm_type;
    typedef typename fsm_type::transition_type     transition_type;
    
    typedef typename unicode::unicode_iterator_traits<
            typename sequence_type::const_iterator> const_iterator;
    typedef typename unicode::char_t                value_type;

    struct parse_context
    {
        void *         user_context;
        bool           is_json_begin;
        sequence_type  member_name;
        pfs::traits::stack<sequence_type, StackT> objects;
        pfs::traits::stack<sequence_type, StackT> arrays;
        sax_context<ValueT> & sax;
    };
    
    grammar ();
    
    static sequence_type unescape_chars (sequence_type & s)
    {
        sequence_type r;
        bool escaped = false;
        const_iterator it = s.cbegin();
        const_iterator end = s.cend();

        while (it != end) {
            if (!escaped && *it == value_type('\\')) {
                escaped = true;
            } else {
                if (escaped) {
                    if (*it == value_type('b'))
                        r.append(1, value_type('\b'));
                    else if (*it == value_type('f'))
                        r.append(1, value_type('\f'));
                    else if (*it == value_type('n'))
                        r.append(1, value_type('\n'));
                    else if (*it == value_type('r'))
                        r.append(1, value_type('\r'));
                    else if (*it == value_type('t'))
                        r.append(1, value_type('\t'));
                    else
                        r.append(1, *it);
                } else {
                    r.append(1, *it);
                }
                escaped = false;
            }
            ++it;
        }

        // TODO Replace by exception
        PFS_ASSERT(!escaped);

        return r;

        // TODO unescape hexdigits in form: \uXXXX
    }     
    
    static bool false_value (const_iterator begin
            , const_iterator end
            , void * context
            , void * action_args)
    {
        if (!context) return true;

        parse_context * ctx = static_cast<parse_context *>(context);
        bool result = ctx->sax.on_boolean_value(ctx->user_context, ctx->member_name, false);
        ctx->member_name.clear();
        return result;
    }

    static bool null_value (const_iterator begin
            , const_iterator end
            , void * context
            , void * action_args)
    {
        if (!context) return true;

        parse_context * ctx = static_cast<parse_context *>(context);
        bool result = ctx->sax.on_null_value(ctx->user_context, ctx->member_name);
        ctx->member_name.clear();
        return result;
    }

    static bool true_value (const_iterator begin
            , const_iterator end
            , void * context
            , void * action_args)
    {
        if (!context) return true;

        parse_context * ctx = static_cast<parse_context *>(context);
        bool result = ctx->sax.on_boolean_value(ctx->user_context, ctx->member_name, true);
        ctx->member_name.clear();
        return result;
    }

    static bool number_value (const_iterator begin
            , const_iterator end
            , void * context
            , void * action_args)
    {
        if (!context) return true;
        
        parse_context * ctx = static_cast<parse_context *>(context);

        sequence_type number(begin, end);
        bool result = false;

        real_t d = lexical_cast<real_t>(number, & result);

        if (!result) {
            // TODO Exception
//            string errstr(string_traits("invalid number value: "));
//            errstr.append(string(begin, end));
//            ctx->nx->append(errstr);
        } else {
            result = ctx->sax.on_number_value(ctx->user_context, ctx->member_name, d);
        }

        ctx->member_name.clear();
        return result;
    }
    
    bool string_value (const_iterator begin
                , const_iterator end
                , void * context
                , void * action_args)
    {
        if (!context) return true;
        
        parse_context * ctx = static_cast<parse_context *>(context);
        sequence_type str(begin, end);

        str = str.substr(1, str.length()-2);
        str = unescape_chars(str);
        bool result = ctx->sax.on_string_value(ctx->user_context, ctx->member_name, str);
        ctx->member_name.clear();
        return result;
    }
    
    static bool success_end_json (const_iterator begin
            , const_iterator end
            , void * context
            , void * action_args)
    {
        if (!context) return true;

        parse_context * ctx = static_cast<parse_context *>(context);
        return ctx->sax.on_end_json(ctx->user_context, true);
    }

    static bool failed_end_json (const_iterator begin
            , const_iterator end
            , void * context
            , void * action_args)
    {
        if (!context) return true;

        parse_context * ctx = static_cast<parse_context *>(context);
        return ctx->sax.on_end_json(ctx->user_context, false);
    }
    
    static bool begin_member (const_iterator begin
            , const_iterator end
            , void * context
            , void * action_args)
    {
        if (context) {
            parse_context * ctx = static_cast<parse_context *>(context);
            sequence_type str(begin, end);

            ctx->member_name = str.substr(1, str.length()-2);
            ctx->member_name = unescape_chars(ctx->member_name);
        }

        return true;
    }

    static bool end_member (const_iterator begin
            , const_iterator end
            , void * context
            , void * action_args)
    {
        if (context) {
            parse_context * ctx = static_cast<parse_context *>(context);
            ctx->member_name.clear();
        }
        return true;
    }

    static bool begin_object (const_iterator begin
            , const_iterator end
            , void * context
            , void * action_args)
    {
        if (!context) return true;

        parse_context * ctx = static_cast<parse_context *>(context);
        ctx->objects.push(ctx->member_name);

        bool result = ctx->is_json_begin
                ? ctx->sax.on_begin_json(ctx->user_context, data_type::object)
                : ctx->sax.on_begin_object(ctx->user_context, ctx->member_name);

        ctx->is_json_begin = false;
        ctx->member_name.clear();

        return result;
    }
    
    static bool end_object (const_iterator begin
            , const_iterator end
            , void * context
            , void * action_args)
    {
        if (!context) return true;

        parse_context * ctx = static_cast<parse_context *>(context);
        PFS_ASSERT(ctx->objects.size() > 0);
        sequence_type member_name(ctx->objects.top());
        ctx->objects.pop();
        return ctx->sax.on_end_object(ctx->user_context, member_name);
    }
    
    static bool begin_array (const_iterator begin
            , const_iterator end
            , void * context
            , void * action_args)
    {
        if (!context) return true;

        parse_context * ctx = static_cast<parse_context *>(context);
        ctx->arrays.push(ctx->member_name);
        bool result = ctx->is_json_begin
                ? ctx->sax.on_begin_json(ctx->user_context, data_type::array)
                : ctx->sax.on_begin_array(ctx->user_context, ctx->member_name);
        ctx->is_json_begin = false;
        ctx->member_name.clear();

        return result;
    }    
    
    static bool end_array (const_iterator begin
            , const_iterator end
            , void * context
            , void * action_args)
    {
        if (!context) return true;

        parse_context * ctx = static_cast<parse_context *>(context);
        PFS_ASSERT(ctx->arrays.size() > 0);
        sequence_type member_name(ctx->arrays.top());
        ctx->arrays.pop();
        return ctx->sax.on_end_array(ctx->user_context, member_name);
    }
    
#if PFS_TEST
    transition_type const * p_number_tr;
#endif
    transition_type const * p_value_tr;
};

template <typename ValueT, template <typename> class StackT>
grammar<ValueT, StackT>::grammar ()
{
//    static string_traits JSON_ALPHA("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");
    static sequence_type const HEXDIGIT("0123456789ABCDEFabcdef"); /* DIGIT / "A" / "B" / "C" / "D" / "E" / "F" */
    static sequence_type const DIGIT("0123456789");
    static sequence_type const DECPOINT(".");
    static sequence_type const DIGIT1_9("123456789");
    static sequence_type const E("eE");
    static sequence_type const MINUS("-");
//    static string_traits JSON_PLUS("+");
    static sequence_type const MINUSPLUS("-+");
    static sequence_type const ZERO("0");
    static sequence_type const WS(" \t\n\r");
    static sequence_type const ESC("\\");
    static sequence_type const DQUOTE("\"");

    /*
     * exp = e [ minus / plus ] 1*DIGIT
     */
    static transition_type const exp_tr[] = {
          { 1, -1, fsm_type::one_of(E)               , fsm_type::normal, 0, 0}
        , { 2, -1, fsm_type::opt_one_of(MINUSPLUS)   , fsm_type::normal, 0, 0}
        , {-1, -1, fsm_type::rpt_one_of(DIGIT, 1, -1), fsm_type::accept, 0, 0}
    };

    /*
     * frac = decimal-point 1*DIGIT
     */
    static transition_type const frac_tr[] = {
          { 1, -1, fsm_type::one_of(DECPOINT)        , fsm_type::normal, 0, 0}
        , {-1, -1, fsm_type::rpt_one_of(DIGIT, 1, -1), fsm_type::accept, 0, 0}
    };

    /*
     * decimal_number_fsm = digit1-9 *DIGIT
     */
    static transition_type const decimal_num_tr[] = {
          { 1, -1, fsm_type::one_of(DIGIT1_9)         , fsm_type::normal, 0, 0}
        , {-1, -1, fsm_type::rpt_one_of(DIGIT, -1, -1), fsm_type::accept, 0, 0}
    };

    /*
     * int = zero / ( digit1-9 *DIGIT )
     */
    static transition_type const int_tr[] = {
          {-1, 1, fsm_type::one_of(ZERO)       , fsm_type::accept, 0, 0}
        , {-1, -1, fsm_type::tr(decimal_num_tr), fsm_type::accept, 0, 0}
    };

    /*
     * number = [ minus ] int [ frac ] [ exp ]
     */
    static transition_type const number_tr[] = {
          { 1,  1, fsm_type::opt_one_of(MINUS), fsm_type::normal, 0, 0}
        , { 2, -1, fsm_type::tr(int_tr)       , fsm_type::normal, 0, 0}
        , { 3, -1, fsm_type::opt_tr(frac_tr)  , fsm_type::normal, 0, 0}
        , {-1, -1, fsm_type::opt_tr(exp_tr)   , fsm_type::accept, 0, 0}
    };

    /*
     * begin-array     = ws %x5B ws  ; [ left square bracket
     */
    static transition_type const begin_array_tr[] = {
          { 1,-1, fsm_type::rpt_one_of(WS, 0,-1)      , fsm_type::normal, 0, 0 }
        , { 2,-1, fsm_type::one_of(sequence_type("[")), fsm_type::normal, 0, 0 }
        , {-1,-1, fsm_type::rpt_one_of(WS, 0,-1)      , fsm_type::accept, 0, 0 }
    };

    /* end-array       = ws %x5D ws  ; ] right square bracket */
    static transition_type const end_array_tr[] = {
          { 1,-1, fsm_type::rpt_one_of(WS, 0,-1)      , fsm_type::normal, 0, 0 }
        , { 2,-1, fsm_type::one_of(sequence_type("]")), fsm_type::normal, 0, 0 }
        , {-1,-1, fsm_type::rpt_one_of(WS, 0,-1)      , fsm_type::accept, 0, 0 }
    };

    /* begin-object    = ws %x7B ws  ; { left curly bracket */
    static transition_type const begin_object_tr[] = {
          { 1,-1, fsm_type::rpt_one_of(WS, 0,-1)      , fsm_type::normal, 0, 0 }
        , { 2,-1, fsm_type::one_of(sequence_type("{")), fsm_type::normal, 0, 0 }
        , {-1,-1, fsm_type::rpt_one_of(WS, 0,-1)      , fsm_type::accept, 0, 0 }
    };

    /* end-object      = ws %x7D ws  ; } right curly bracket */
    static transition_type const end_object_tr[] = {
          { 1,-1, fsm_type::rpt_one_of(WS, 0,-1)      , fsm_type::normal, 0, 0 }
        , { 2,-1, fsm_type::one_of(sequence_type("}")), fsm_type::normal, 0, 0 }
        , {-1,-1, fsm_type::rpt_one_of(WS, 0,-1)      , fsm_type::accept, 0, 0 }
    };

    /* name-separator  = ws %x3A ws  ; : colon */
    static transition_type const name_separator_tr[] = {
          { 1,-1, fsm_type::rpt_one_of(WS, 0,-1)      , fsm_type::normal, 0, 0 }
        , { 2,-1, fsm_type::one_of(sequence_type(":")), fsm_type::normal, 0, 0 }
        , {-1,-1, fsm_type::rpt_one_of(WS, 0,-1)      , fsm_type::accept, 0, 0 }
    };

    /* value-separator = ws %x2C ws  ; , comma */
    static transition_type const value_separator_tr[] = {
          { 1,-1, fsm_type::rpt_one_of(WS, 0,-1)      , fsm_type::normal, 0, 0 }
        , { 2,-1, fsm_type::one_of(sequence_type(",")), fsm_type::normal, 0, 0 }
        , {-1,-1, fsm_type::rpt_one_of(WS, 0,-1)      , fsm_type::accept, 0, 0 }
    };


    /* === Strings === */

    //
    // unescaped = %x20-21 / %x23-5B / %x5D-10FFFF
    //
    static value_type const unescaped_char[] = {
          0x20u, 0x21u
        , 0x23u, 0x5Bu
        , 0x5Du, 0x10FFFFu
    };
    
    static transition_type const unescaped_char_tr[] = {
          {-1, 1, fsm_type::range(unescaped_char[0], unescaped_char[1]), fsm_type::accept, 0, 0 }
        , {-1, 2, fsm_type::range(unescaped_char[2], unescaped_char[3]), fsm_type::accept, 0, 0 }
        , {-1,-1, fsm_type::range(unescaped_char[4], unescaped_char[5]), fsm_type::accept, 0, 0 }
    };

    /* %x75 4HEXDIG ; uXXXX */
    static transition_type const unicode_char_tr[] = {
          { 1,-1, fsm_type::one_of(sequence_type("uU")), fsm_type::normal, 0, 0 }
        , {-1,-1, fsm_type::rpt_one_of(HEXDIGIT, 4, 4) , fsm_type::accept, 0, 0 }
    };


    /*
        %x22 /          ; "    quotation mark  U+0022
        %x5C /          ; \    reverse solidus U+005C
        %x2F /          ; /    solidus         U+002F
        %x62 /          ; b    backspace       U+0008
        %x66 /          ; f    form feed       U+000C
        %x6E /          ; n    line feed       U+000A
        %x72 /          ; r    carriage return U+000D
        %x74 /          ; t    tab             U+0009
        %x75 4HEXDIG    ; uXXXX                U+XXXX
    */
    static transition_type const escaped_char_tr[] = {
          {-1, 1, fsm_type::one_of(sequence_type("\"\\/bfnrt")), fsm_type::accept, 0, 0 }
        , {-1,-1, fsm_type::tr(unicode_char_tr)                , fsm_type::accept, 0, 0 }
    };

    /* escape escaped_char */
    static transition_type const escaped_tr[] = {
          { 1,-1, fsm_type::one_of(ESC)        , fsm_type::normal, 0, 0 }
        , {-1,-1, fsm_type::tr(escaped_char_tr), fsm_type::accept, 0, 0 }
    };

    /*
    char = unescaped /
        escape (
            %x22 /          ; "    quotation mark  U+0022
            %x5C /          ; \    reverse solidus U+005C
            %x2F /          ; /    solidus         U+002F
            %x62 /          ; b    backspace       U+0008
            %x66 /          ; f    form feed       U+000C
            %x6E /          ; n    line feed       U+000A
            %x72 /          ; r    carriage return U+000D
            %x74 /          ; t    tab             U+0009
            %x75 4HEXDIG )  ; uXXXX                U+XXXX
    */
    static transition_type const char_tr[] = {
          {-1, 1, fsm_type::tr(unescaped_char_tr), fsm_type::accept, 0, 0 }
        , {-1,-1, fsm_type::tr(escaped_tr)       , fsm_type::accept, 0, 0 }
    };

    /* string = quotation-mark *char quotation-mark */
    static transition_type const string_tr[] = {
          { 1,-1, fsm_type::one_of(DQUOTE)       , fsm_type::normal, 0, 0 }
        , { 2,-1, fsm_type::rpt_tr(char_tr, 0,-1), fsm_type::normal, 0, 0 }
        , {-1,-1, fsm_type::one_of(DQUOTE)       , fsm_type::accept, 0, 0 }
    };


    /* === Objects ===*/

    /* member = string name-separator value */
    static transition_type const member_tr[] = {
          { 1,-1, fsm_type::tr(string_tr)        , fsm_type::normal, begin_member, 0 }
        , { 2,-1, fsm_type::tr(name_separator_tr), fsm_type::normal, 0, 0 }
        , {-1,-1, fsm_type::tr(p_value_tr)       , fsm_type::accept, end_member, 0 }
    };

    /* next-member = value-separator member */
    static transition_type const next_member_tr[] = {
          { 1,-1, fsm_type::tr(value_separator_tr), fsm_type::normal, 0, 0 }
        , {-1,-1, fsm_type::tr(member_tr)         , fsm_type::accept, 0, 0 }
    };

    /* object-body = member *next-member */
    static transition_type const object_body_tr[] = {
          { 1,-1, fsm_type::tr(member_tr)               , fsm_type::normal, 0, 0 }
        , {-1,-1, fsm_type::rpt_tr(next_member_tr, 0,-1), fsm_type::accept, 0, 0 }
    };

    /* object = begin-object [ member *( value-separator member ) ] end-object */
    /* object = begin-object [ object-body ] end-object */
    static transition_type const object_tr[] = {
          { 1,-1, fsm_type::tr(begin_object_tr)   , fsm_type::normal, begin_object, 0 }
        , { 2,-1, fsm_type::opt_tr(object_body_tr), fsm_type::normal, 0, 0 }
        , {-1,-1, fsm_type::tr(end_object_tr)     , fsm_type::accept, end_object, 0 }
    };

    /* === Arrays === */
    /* next-value = value-separator value  */
    static transition_type const next_value_tr[] = {
          { 1,-1, fsm_type::tr(value_separator_tr), fsm_type::normal, 0, 0 }
        , {-1,-1, fsm_type::tr(p_value_tr)        , fsm_type::accept, 0, 0 }
    };

    /* array-body = value *next-value */
    static transition_type const array_body_tr[] = {
          { 1,-1, fsm_type::tr(p_value_tr)             , fsm_type::normal, 0, 0 }
        , {-1,-1, fsm_type::rpt_tr(next_value_tr, 0,-1), fsm_type::accept, 0, 0 }
    };

    /* array = begin-array [ value *( value-separator value ) ] end-array */
    /* array = begin-array [ array-body ] end-array */
    static transition_type const array_tr[] = {
          { 1,-1, fsm_type::tr(begin_array_tr)   , fsm_type::normal, begin_array, 0 }
        , { 2,-1, fsm_type::opt_tr(array_body_tr), fsm_type::normal, 0, 0 }
        , {-1,-1, fsm_type::tr(end_array_tr)     , fsm_type::accept, end_array, 0 }
    };

    /* value = false / null / true / object / array / number / string */
    static transition_type const value_tr[] = {
          {-1, 1, fsm_type::seq("false") , fsm_type::accept, false_value, 0 }
        , {-1, 2, fsm_type::seq("null")  , fsm_type::accept, null_value , 0 }
        , {-1, 3, fsm_type::seq("true")  , fsm_type::accept, true_value , 0 }
        , {-1, 4, fsm_type::tr(number_tr), fsm_type::accept, number_value, 0 }
        , {-1, 5, fsm_type::tr(string_tr), fsm_type::accept, string_value, 0 }
        , {-1, 6, fsm_type::tr(object_tr), fsm_type::accept, 0, 0 }
        , {-1,-1, fsm_type::tr(array_tr) , fsm_type::accept, 0, 0 }
    };
    
    /* JSON-text = object / array */
    static transition_type const json_tr[] = {
          { 2, 1, fsm_type::tr(object_tr) , fsm_type::normal, 0, 0 }
        , { 2, 3, fsm_type::tr(array_tr)  , fsm_type::normal, 0, 0 }
        , {-1,-1, fsm_type::nothing()     , fsm_type::accept, success_end_json, 0 }
        , {-1,-1, fsm_type::nothing()     , fsm_type::reject, failed_end_json, 0 }
    };
    
#if PFS_TEST
    p_number_tr = number_tr;
#endif
    
    p_value_tr = value_tr;
} // grammar

}} // pfs::json

#endif /* __PFS_JSON_RFC4627_HPP__ */
