#ifndef __PFS_JSON_RFC4627_HPP__
#define __PFS_JSON_RFC4627_HPP__

#include <pfs/fsm/fsm.hpp>
#include <pfs/lexical_cast.hpp>
#include <pfs/traits/stack.hpp>
#include <pfs/traits/stdcxx/stack.hpp>
//#include <pfs/unicode/iterator.hpp>
#include <pfs/json/constants.hpp>
#include "exception.hpp"

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

template <typename JsonType>
struct sax_context
{
    typedef typename JsonType::string_type sequence_type;
    
    virtual bool on_begin_json   (void * user_context, data_type_t) = 0;
    virtual bool on_end_json     (void * user_context, bool) = 0;
    virtual bool on_begin_object (void * user_context, sequence_type const &) = 0;
    virtual bool on_end_object   (void * user_context, sequence_type const &) = 0;
    virtual bool on_begin_array  (void * user_context, sequence_type const &) = 0;
    virtual bool on_end_array    (void * user_context, sequence_type const &) = 0;
    virtual bool on_null_value   (void * user_context, sequence_type const &) = 0;
    virtual bool on_boolean_value(void * user_context, sequence_type const &, bool) = 0;
    virtual bool on_number_value (void * user_context, sequence_type const &, real_t) = 0;
    virtual bool on_string_value (void * user_context, sequence_type const &, sequence_type const &) = 0;
};

template <typename JsonType, template <typename> class StackT = traits::stdcxx::stack>
struct grammar
{
    typedef typename JsonType::string_type         string_type;
    typedef typename string_type::const_iterator   iterator;
    //typedef typename string_type::const_iterator   native_iterator;
    //typedef typename unicode::unicode_iterator_traits<
    //        native_iterator>::iterator             iterator;

    //typedef fsm::fsm<iterator>             fsm_type;
    typedef fsm::fsm<iterator>                     fsm_type;
    typedef typename fsm_type::transition_type     transition_type;
    typedef typename fsm_type::char_type           value_type;

    struct parse_context
    {
        void *         user_context;
        bool           is_json_begin;
        string_type    member_name;
        pfs::traits::stack<string_type, StackT> objects;
        pfs::traits::stack<string_type, StackT> arrays;
        sax_context<JsonType> & sax;
    };
    
    grammar ();
    
    static string_type unescape_chars (iterator first, iterator last)
    {
        string_type r;
        bool escaped = false;
        iterator it = first;

        while (it != last) {
            if (!escaped && *it == value_type('\\')) {
                escaped = true;
            } else {
                if (escaped) {
                    if (*it == value_type('b'))
                        iterator::encode(value_type('\b'), pfs::back_inserter(r));
                    else if (*it == value_type('f'))
                        iterator::encode(value_type('\f'), pfs::back_inserter(r));
                    else if (*it == value_type('n'))
                        iterator::encode(value_type('\n'), pfs::back_inserter(r));
                    else if (*it == value_type('r'))
                        iterator::encode(value_type('\r'), pfs::back_inserter(r));
                    else if (*it == value_type('t'))
                        iterator::encode(value_type('\t'), pfs::back_inserter(r));
                    else
                        iterator::encode(*it, pfs::back_inserter(r));
                } else {
                    iterator::encode(*it, pfs::back_inserter(r));
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
    
    static bool false_value (iterator begin, iterator end, void * context, void * action_args)
    {
        if (!context) return true;

        parse_context * ctx = static_cast<parse_context *>(context);
        bool result = ctx->sax.on_boolean_value(ctx->user_context, ctx->member_name, false);
        ctx->member_name.clear();
        return result;
    }

    static bool null_value (iterator begin, iterator end, void * context, void * action_args)
    {
        if (!context) return true;

        parse_context * ctx = static_cast<parse_context *>(context);
        bool result = ctx->sax.on_null_value(ctx->user_context, ctx->member_name);
        ctx->member_name.clear();
        return result;
    }

    static bool true_value (iterator begin, iterator end, void * context, void * action_args)
    {
        if (!context) return true;

        parse_context * ctx = static_cast<parse_context *>(context);
        bool result = ctx->sax.on_boolean_value(ctx->user_context, ctx->member_name, true);
        ctx->member_name.clear();
        return result;
    }

    static bool number_value (iterator begin, iterator end, void * context, void * action_args)
    {
        if (!context) return true;
        
        parse_context * ctx = static_cast<parse_context *>(context);

        string_type number_str = string_type(begin, end);
        bool result = false;
        real_t d;
        
        try {
            d = lexical_cast<real_t, string_type>(number_str);
        } catch (bad_lexical_cast ex) {
            throw json_exception(json_errc::bad_number);
        }

        result = ctx->sax.on_number_value(ctx->user_context, ctx->member_name, d);
        ctx->member_name.clear();
        return result;
    }
    
    static bool string_value (iterator begin, iterator end, void * context, void * action_args)
    {
        if (!context) return true;
        
        parse_context * ctx = static_cast<parse_context *>(context);
        ++begin;
        --end;
        string_type str = unescape_chars(begin, end);
        bool result = ctx->sax.on_string_value(ctx->user_context, ctx->member_name, str);
        ctx->member_name.clear();
        return result;
    }
    
    static bool success_end_json (iterator begin, iterator end, void * context, void * action_args)
    {
        if (!context) return true;

        parse_context * ctx = static_cast<parse_context *>(context);
        return ctx->sax.on_end_json(ctx->user_context, true);
    }

    static bool failed_end_json (iterator begin, iterator end, void * context, void * action_args)
    {
        if (!context) return true;

        parse_context * ctx = static_cast<parse_context *>(context);
        return ctx->sax.on_end_json(ctx->user_context, false);
    }
    
    static bool begin_member (iterator begin, iterator end, void * context, void * action_args)
    {
        if (context) {
            parse_context * ctx = static_cast<parse_context *>(context);
            ++begin;
            --end;
            ctx->member_name = unescape_chars(begin, end);
        }

        return true;
    }

    static bool end_member (iterator begin, iterator end, void * context, void * action_args)
    {
        if (context) {
            parse_context * ctx = static_cast<parse_context *>(context);
            ctx->member_name.clear();
        }
        return true;
    }

    static bool begin_object (iterator begin, iterator end, void * context, void * action_args)
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
    
    static bool end_object (iterator begin, iterator end, void * context, void * action_args)
    {
        if (!context) return true;

        parse_context * ctx = static_cast<parse_context *>(context);
        PFS_ASSERT(ctx->objects.size() > 0);
        string_type member_name(ctx->objects.top());
        ctx->objects.pop();
        return ctx->sax.on_end_object(ctx->user_context, member_name);
    }
    
    static bool begin_array (iterator begin, iterator end, void * context, void * action_args)
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
    
    static bool end_array (iterator begin, iterator end, void * context, void * action_args)
    {
        if (!context) return true;

        parse_context * ctx = static_cast<parse_context *>(context);
        PFS_ASSERT(ctx->arrays.size() > 0);
        string_type member_name(ctx->arrays.top());
        ctx->arrays.pop();
        return ctx->sax.on_end_array(ctx->user_context, member_name);
    }
    
#if PFS_TEST
    transition_type const * p_decimal_num_tr;
    transition_type const * p_number_tr;
#endif
//    transition_type const * p_value_tr;
};

template <typename ValueT, template <typename> class StackT>
grammar<ValueT, StackT>::grammar ()
{
    static string_type const E("eE");
    static string_type const MINUSPLUS("-+");
    static string_type const DIGIT("0123456789");
    static string_type const HEXDIGIT("0123456789ABCDEFabcdef"); /* DIGIT / "A" / "B" / "C" / "D" / "E" / "F" */
    static string_type const DECPOINT(".");
    static string_type const DIGIT1_9("123456789");
    static string_type const MINUS("-");
    static string_type const ZERO("0");
    static string_type const WS(" \t\n\r");
    static string_type const ESC("\\");
    static string_type const DQUOTE("\"");
    static string_type const SPEC_CHARS("\"\\/bfnrt");
    static string_type const LEFT_SQUARE_BRACKET("[");
    static string_type const RIGHT_SQUARE_BRACKET("]");
    static string_type const LEFT_CURLY_BRACKET("{");
    static string_type const RIGHT_CURLY_BRACKET("}");
    static string_type const COLON(":");
    static string_type const COMMA(",");
    static string_type const FALSE_STR("false");
    static string_type const NULL_STR("null");
    static string_type const TRUE_STR("true");

#undef FSM_SEQ
#undef FSM_ONE_OF
#undef FSM_OPT_ONE_OF
#undef FSM_RPT_ONE_OF
#undef FSM_TR
#undef FSM_OPT_TR
#undef FSM_RANGE
#undef FSM_RPT_TR
#undef FSM_NOTHING
#define FSM_SEQ(x)            fsm_type::seq(x.begin(), x.end())
#define FSM_ONE_OF(x)         fsm_type::one_of(x.begin(), x.end())
#define FSM_OPT_ONE_OF(x)     fsm_type::opt_one_of(x.begin(), x.end())
#define FSM_RPT_ONE_OF(x,a,b) fsm_type::rpt_one_of(x.begin(), x.end(), a, b)
#define FSM_TR(x)             fsm_type::tr(x)
#define FSM_OPT_TR(x)         fsm_type::opt_tr(x)
#define FSM_RANGE(a,b)        fsm_type::range(a,b)
#define FSM_RPT_TR(x,a,b)     fsm_type::rpt_tr(x,a,b)
#define FSM_NOTHING           fsm_type::nothing()
    
    /*
     * exp = e [ minus / plus ] 1*DIGIT
     */
    static transition_type const exp_tr[] = {
          { 1, -1, FSM_ONE_OF(E)               , fsm_type::normal, 0, 0}
        , { 2, -1, FSM_OPT_ONE_OF(MINUSPLUS)   , fsm_type::normal, 0, 0}
        , {-1, -1, FSM_RPT_ONE_OF(DIGIT, 1, -1), fsm_type::accept, 0, 0}
    };

    /*
     * frac = decimal-point 1*DIGIT
     */
    static transition_type const frac_tr[] = {
          { 1, -1, FSM_ONE_OF(DECPOINT)        , fsm_type::normal, 0, 0}
        , {-1, -1, FSM_RPT_ONE_OF(DIGIT, 1, -1), fsm_type::accept, 0, 0}
    };

    /*
     * decimal_number_fsm = digit1-9 *DIGIT
     */
    static transition_type const decimal_num_tr[] = {
          { 1, -1, FSM_ONE_OF(DIGIT1_9)         , fsm_type::normal, 0, 0}
        , {-1, -1, FSM_RPT_ONE_OF(DIGIT, -1, -1), fsm_type::accept, 0, 0}
    };

    /*
     * int = zero / ( digit1-9 *DIGIT )
     */
    static transition_type const int_tr[] = {
          {-1,  1, FSM_ONE_OF(ZERO)      , fsm_type::accept, 0, 0}
        , {-1, -1, FSM_TR(decimal_num_tr), fsm_type::accept, 0, 0}
    };

    /*
     * number = [ minus ] int [ frac ] [ exp ]
     */
    static transition_type const number_tr[] = {
          { 1,  1, FSM_OPT_ONE_OF(MINUS), fsm_type::normal, 0, 0}
        , { 2, -1, FSM_TR(int_tr)       , fsm_type::normal, 0, 0}
        , { 3, -1, FSM_OPT_TR(frac_tr)  , fsm_type::normal, 0, 0}
        , {-1, -1, FSM_OPT_TR(exp_tr)   , fsm_type::accept, 0, 0}
    };

    /*
     * begin-array     = ws %x5B ws  ; [ left square bracket
     */
    static transition_type const begin_array_tr[] = {
          { 1,-1, FSM_RPT_ONE_OF(WS, 0,-1)       , fsm_type::normal, 0, 0 }
        , { 2,-1, FSM_ONE_OF(LEFT_SQUARE_BRACKET), fsm_type::normal, 0, 0 }
        , {-1,-1, FSM_RPT_ONE_OF(WS, 0,-1)       , fsm_type::accept, 0, 0 }
    };

    /* end-array       = ws %x5D ws  ; ] right square bracket */
    static transition_type const end_array_tr[] = {
          { 1,-1, FSM_RPT_ONE_OF(WS, 0,-1)        , fsm_type::normal, 0, 0 }
        , { 2,-1, FSM_ONE_OF(RIGHT_SQUARE_BRACKET), fsm_type::normal, 0, 0 }
        , {-1,-1, FSM_RPT_ONE_OF(WS, 0,-1)        , fsm_type::accept, 0, 0 }
    };

    /* begin-object    = ws %x7B ws  ; { left curly bracket */
    static transition_type const begin_object_tr[] = {
          { 1,-1, FSM_RPT_ONE_OF(WS, 0,-1)      , fsm_type::normal, 0, 0 }
        , { 2,-1, FSM_ONE_OF(LEFT_CURLY_BRACKET), fsm_type::normal, 0, 0 }
        , {-1,-1, FSM_RPT_ONE_OF(WS, 0,-1)      , fsm_type::accept, 0, 0 }
    };

    /* end-object      = ws %x7D ws  ; } right curly bracket */
    static transition_type const end_object_tr[] = {
          { 1,-1, FSM_RPT_ONE_OF(WS, 0,-1)       , fsm_type::normal, 0, 0 }
        , { 2,-1, FSM_ONE_OF(RIGHT_CURLY_BRACKET), fsm_type::normal, 0, 0 }
        , {-1,-1, FSM_RPT_ONE_OF(WS, 0,-1)       , fsm_type::accept, 0, 0 }
    };

    /* name-separator  = ws %x3A ws  ; : colon */
    static transition_type const name_separator_tr[] = {
          { 1,-1, FSM_RPT_ONE_OF(WS, 0,-1), fsm_type::normal, 0, 0 }
        , { 2,-1, FSM_ONE_OF(COLON)       , fsm_type::normal, 0, 0 }
        , {-1,-1, FSM_RPT_ONE_OF(WS, 0,-1), fsm_type::accept, 0, 0 }
    };

    /* value-separator = ws %x2C ws  ; , comma */
    static transition_type const value_separator_tr[] = {
          { 1,-1, FSM_RPT_ONE_OF(WS, 0,-1), fsm_type::normal, 0, 0 }
        , { 2,-1, FSM_ONE_OF(COMMA)       , fsm_type::normal, 0, 0 }
        , {-1,-1, FSM_RPT_ONE_OF(WS, 0,-1), fsm_type::accept, 0, 0 }
    };


    /* === Strings === */

    /* unescaped = %x20-21 / %x23-5B / %x5D-10FFFF */
    static value_type const unescaped_char[] = {
          0x20u, 0x21u
        , 0x23u, 0x5Bu
        , 0x5Du, 0x10FFFFu
    };
    
    static transition_type const unescaped_char_tr[] = {
          {-1, 1, FSM_RANGE(unescaped_char[0], unescaped_char[1]), fsm_type::accept, 0, 0 }
        , {-1, 2, FSM_RANGE(unescaped_char[2], unescaped_char[3]), fsm_type::accept, 0, 0 }
        , {-1,-1, FSM_RANGE(unescaped_char[4], unescaped_char[5]), fsm_type::accept, 0, 0 }
    };

    /* %x75 4HEXDIG ; uXXXX */
    static string_type const uU("uU");
    
    static transition_type const unicode_char_tr[] = {
          { 1,-1, FSM_ONE_OF(uU)                 , fsm_type::normal, 0, 0 }
        , {-1,-1, FSM_RPT_ONE_OF(HEXDIGIT, 4, 4) , fsm_type::accept, 0, 0 }
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
          {-1, 1, FSM_ONE_OF(SPEC_CHARS) , fsm_type::accept, 0, 0 }
        , {-1,-1, FSM_TR(unicode_char_tr), fsm_type::accept, 0, 0 }
    };

    /* escape escaped_char */
    static transition_type const escaped_tr[] = {
          { 1,-1, FSM_ONE_OF(ESC)        , fsm_type::normal, 0, 0 }
        , {-1,-1, FSM_TR(escaped_char_tr), fsm_type::accept, 0, 0 }
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
          {-1, 1, FSM_TR(unescaped_char_tr), fsm_type::accept, 0, 0 }
        , {-1,-1, FSM_TR(escaped_tr)       , fsm_type::accept, 0, 0 }
    };

    /* string = quotation-mark *char quotation-mark */
    static transition_type const string_tr[] = {
          { 1,-1, FSM_ONE_OF(DQUOTE)       , fsm_type::normal, 0, 0 }
        , { 2,-1, FSM_RPT_TR(char_tr, 0,-1), fsm_type::normal, 0, 0 }
        , {-1,-1, FSM_ONE_OF(DQUOTE)       , fsm_type::accept, 0, 0 }
    };


    /* === Objects ===*/

    /* value = false / null / true / object / array / number / string */
    static transition_type value_tr[] = {
          {-1, 1, FSM_SEQ(FALSE_STR), fsm_type::accept, false_value, 0 }
        , {-1, 2, FSM_SEQ(NULL_STR) , fsm_type::accept, null_value , 0 }
        , {-1, 3, FSM_SEQ(TRUE_STR) , fsm_type::accept, true_value , 0 }
        , {-1, 4, FSM_TR(number_tr) , fsm_type::accept, number_value, 0 }
        , {-1, 5, FSM_TR(string_tr) , fsm_type::accept, string_value, 0 }
        // Bellow data will be replaced partially (`match_type m` member)
        , {-1, 6, FSM_NOTHING       , fsm_type::accept, 0, 0 }
        , {-1,-1, FSM_NOTHING       , fsm_type::accept, 0, 0 }
    };
    
    /* member = string name-separator value */
    static transition_type const member_tr[] = {
          { 1,-1, FSM_TR(string_tr)        , fsm_type::normal, begin_member, 0 }
        , { 2,-1, FSM_TR(name_separator_tr), fsm_type::normal, 0, 0 }
        , {-1,-1, FSM_TR(value_tr)         , fsm_type::accept, end_member, 0 }
    };

    /* next-member = value-separator member */
    static transition_type const next_member_tr[] = {
          { 1,-1, FSM_TR(value_separator_tr), fsm_type::normal, 0, 0 }
        , {-1,-1, FSM_TR(member_tr)         , fsm_type::accept, 0, 0 }
    };

    /* object-body = member *next-member */
    static transition_type const object_body_tr[] = {
          { 1,-1, FSM_TR(member_tr)               , fsm_type::normal, 0, 0 }
        , {-1,-1, FSM_RPT_TR(next_member_tr, 0,-1), fsm_type::accept, 0, 0 }
    };

    /* object = begin-object [ member *( value-separator member ) ] end-object */
    /* object = begin-object [ object-body ] end-object */
    static transition_type const object_tr[] = {
          { 1,-1, FSM_TR(begin_object_tr)   , fsm_type::normal, begin_object, 0 }
        , { 2,-1, FSM_OPT_TR(object_body_tr), fsm_type::normal, 0, 0 }
        , {-1,-1, FSM_TR(end_object_tr)     , fsm_type::accept, end_object, 0 }
    };

    /* === Arrays === */
    /* next-value = value-separator value  */
    static transition_type const next_value_tr[] = {
          { 1,-1, FSM_TR(value_separator_tr), fsm_type::normal, 0, 0 }
        , {-1,-1, FSM_TR(value_tr)          , fsm_type::accept, 0, 0 }
    };

    /* array-body = value *next-value */
    static transition_type const array_body_tr[] = {
          { 1,-1, FSM_TR(value_tr)               , fsm_type::normal, 0, 0 }
        , {-1,-1, FSM_RPT_TR(next_value_tr, 0,-1), fsm_type::accept, 0, 0 }
    };

    /* array = begin-array [ value *( value-separator value ) ] end-array */
    /* array = begin-array [ array-body ] end-array */
    static transition_type const array_tr[] = {
          { 1,-1, FSM_TR(begin_array_tr)   , fsm_type::normal, begin_array, 0 }
        , { 2,-1, FSM_OPT_TR(array_body_tr), fsm_type::normal, 0, 0 }
        , {-1,-1, FSM_TR(end_array_tr)     , fsm_type::accept, end_array, 0 }
    };

    /* value = false / null / true / object / array / number / string */
    value_tr[5].m = FSM_TR(object_tr);
    value_tr[6].m = FSM_TR(array_tr);

    /* JSON-text = object / array */
    static transition_type const json_tr[] = {
          { 2, 1, FSM_TR(object_tr), fsm_type::normal, 0, 0 }
        , { 2, 3, FSM_TR(array_tr) , fsm_type::normal, 0, 0 }
        , {-1,-1, FSM_NOTHING      , fsm_type::accept, success_end_json, 0 }
        , {-1,-1, FSM_NOTHING      , fsm_type::reject, failed_end_json, 0 }
    };
    
#if PFS_TEST
    p_decimal_num_tr = decimal_num_tr;
    p_number_tr      = number_tr;
#endif

} // grammar

}} // pfs::json

#endif /* __PFS_JSON_RFC4627_HPP__ */
