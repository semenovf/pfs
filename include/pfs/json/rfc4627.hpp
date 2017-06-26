#ifndef __PFS_JSON_RFC4627_HPP__
#define __PFS_JSON_RFC4627_HPP__

#include <pfs/fsm/fsm.hpp>
#include <pfs/lexical_cast.hpp>
#include <pfs/traits/stack.hpp>
#include <pfs/traits/stdcxx/stack.hpp>

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


template <typename ValueT, template <typename> class StackT = traits::stdcxx::stack>
struct grammar
{
    typedef typename ValueT::string_traits         string_traits;
    typedef fsm::fsm<string_traits>                fsm_type;
    typedef typename fsm_type::transition_type     transition_type;
    typedef typename string_traits::const_iterator const_iterator;

    struct user_context
    {
        void *         userContext;
        bool           isBeginJson;
        string_traits  memberName;
        pfs::traits::stack<string_traits, StackT> objects;
        pfs::traits::stack<string_traits, StackT> arrays;

        bool (* on_begin_json)   (void *, bool);
        bool (* on_end_json)     (void *, bool);
        bool (* on_begin_object) (void *, const string_traits &);
        bool (* on_end_object)   (void *, const string_traits &);
        bool (* on_begin_array)  (void *, const string_traits &);
        bool (* on_end_array)    (void *, const string_traits &);
        bool (* on_null_value)   (void *, const string_traits &);
        bool (* on_boolean_value)(void *, const string_traits &, bool);
        bool (* on_number_value) (void *, const string_traits &, real_t);
        bool (* on_string_value) (void *, const string_traits &, const string_traits &);
    };
    
    grammar ();
    
    static string_type unescape_chars (string_type & s);
    
    static bool scalar_value (const_iterator begin
            , const_iterator end
            , void * context
            , void * action_args);

    static bool end_json (const_iterator begin
            , const_iterator end
            , void * context
            , void * action_args);
            
    static bool begin_member (const_iterator begin
            , const_iterator end
            , void * context
            , void * action_args);

    static bool end_member (const_iterator begin
            , const_iterator end
            , void * context
            , void * action_args);

    static bool begin_object (const_iterator begin
            , const_iterator end
            , void * context
            , void * action_args);
    
    static bool end_object (const_iterator begin
            , const_iterator end
            , void * context
            , void * action_args);
    
    static bool begin_array (const_iterator begin
            , const_iterator end
            , void * context
            , void * action_args);
    
    static bool end_array (const_iterator begin
            , const_iterator end
            , void * context
            , void * action_args);
    
    
#if PFS_TEST
    static transition_type const * p_number_tr;
#endif
    static transition_type const * p_value_tr;
};


template <typename ValueT, template <typename> class StackT>
grammar<ValueT, StackT>::grammar ()
{
//    static string_traits JSON_ALPHA("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");
    static string_traits const HEXDIGIT("0123456789ABCDEFabcdef"); /* DIGIT / "A" / "B" / "C" / "D" / "E" / "F" */
    static string_traits const DIGIT("0123456789");
    static string_traits const DECPOINT(".");
    static string_traits const DIGIT1_9("123456789");
    static string_traits const E("eE");
    static string_traits const MINUS("-");
//    static string_traits JSON_PLUS("+");
    static string_traits const MINUSPLUS("-+");
    static string_traits const ZERO("0");
    static string_traits const WS(" \t\n\r");
    static string_traits const ESC("\\");
    static string_traits const DQUOTE("\"");

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
          { 1, -1, fsm_type::one_of(DECPOINT)       , fsm_type::normal, 0, 0}
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
          { 1, 1, fsm_type::opt_one_of(MINUS), fsm_type::normal, 0, 0}
        , { 2, -1, fsm_type::tr(int_tr)      , fsm_type::normal, 0, 0}
        , { 3, -1, fsm_type::opt_tr(frac_tr) , fsm_type::normal, 0, 0}
        , {-1, -1, fsm_type::opt_tr(exp_tr)  , fsm_type::accept, 0, 0}
    };

    /*
     * begin-array     = ws %x5B ws  ; [ left square bracket
     */
    static transition_type const begin_array_tr[] = {
          { 1,-1, fsm_type::rpt_one_of(WS, 0,-1)      , fsm_type::normal, 0, 0 }
        , { 2,-1, fsm_type::one_of(string_traits("[")), fsm_type::normal, 0, 0 }
        , {-1,-1, fsm_type::rpt_one_of(WS, 0,-1)      , fsm_type::accept, 0, 0 }
    };

    /* end-array       = ws %x5D ws  ; ] right square bracket */
    static transition_type const end_array_tr[] = {
          { 1,-1, fsm_type::rpt_one_of(WS, 0,-1)      , fsm_type::normal, 0, 0 }
        , { 2,-1, fsm_type::one_of(string_traits("]")), fsm_type::normal, 0, 0 }
        , {-1,-1, fsm_type::rpt_one_of(WS, 0,-1)      , fsm_type::accept, 0, 0 }
    };

    /* begin-object    = ws %x7B ws  ; { left curly bracket */
    static transition_type const begin_object_tr[] = {
          { 1,-1, fsm_type::rpt_one_of(WS, 0,-1) , fsm_type::normal, 0, 0 }
        , { 2,-1, fsm_type::one_of(string_traits("{"))          , fsm_type::normal, 0, 0 }
        , {-1,-1, fsm_type::rpt_one_of(WS, 0,-1) , fsm_type::accept, 0, 0 }
    };

    /* end-object      = ws %x7D ws  ; } right curly bracket */
    static transition_type const end_object_tr[] = {
          { 1,-1, fsm_type::rpt_one_of(WS, 0,-1) , fsm_type::normal, 0, 0 }
        , { 2,-1, fsm_type::one_of(string_traits("}"))          , fsm_type::normal, 0, 0 }
        , {-1,-1, fsm_type::rpt_one_of(WS, 0,-1) , fsm_type::accept, 0, 0 }
    };

    /* name-separator  = ws %x3A ws  ; : colon */
    static transition_type const name_separator_tr[] = {
          { 1,-1, fsm_type::rpt_one_of(WS, 0,-1) , fsm_type::normal, 0, 0 }
        , { 2,-1, fsm_type::one_of(string_traits(":"))          , fsm_type::normal, 0, 0 }
        , {-1,-1, fsm_type::rpt_one_of(WS, 0,-1) , fsm_type::accept, 0, 0 }
    };

    /* value-separator = ws %x2C ws  ; , comma */
    static transition_type const value_separator_tr[] = {
          { 1,-1, fsm_type::rpt_one_of(WS, 0,-1) , fsm_type::normal, 0, 0 }
        , { 2,-1, fsm_type::one_of(string_traits(","))          , fsm_type::normal, 0, 0 }
        , {-1,-1, fsm_type::rpt_one_of(WS, 0,-1) , fsm_type::accept, 0, 0 }
    };


    /* === Strings === */

    //
    // unescaped = %x20-21 / %x23-5B / %x5D-10FFFF
    //
    static typename string_traits::value_type const unescaped_char[] = {
          0x20u, 0x21u
        , 0x23u, 0x5Bu
        , 0x5Du, 0x10FFFFu
    };
    
    static transition_type const unescaped_char_tr[] = {
          {-1, 1, fsm_type::range(unescaped_char[0], unescaped_char[1]) , fsm_type::accept, 0, 0 }
        , {-1, 2, fsm_type::range(unescaped_char[2], unescaped_char[3]) , fsm_type::accept, 0, 0 }
        , {-1,-1, fsm_type::range(unescaped_char[4], unescaped_char[5]) , fsm_type::accept, 0, 0 }
    };

    /* %x75 4HEXDIG ; uXXXX */
    static transition_type const unicode_char_tr[] = {
          { 1,-1, fsm_type::one_of(string_traits("uU"))                , fsm_type::normal, 0, 0 }
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
          {-1, 1, fsm_type::one_of(string_traits("\"\\/bfnrt")) , fsm_type::accept, 0, 0 }
        , {-1,-1, fsm_type::tr(unicode_char_tr)   , fsm_type::accept, 0, 0 }
    };

    /* escape escaped_char */
    static transition_type const escaped_tr[] = {
          { 1,-1, fsm_type::one_of(ESC)         , fsm_type::normal, 0, 0 }
        , {-1,-1, fsm_type::tr(escaped_char_tr) , fsm_type::accept, 0, 0 }
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
          {-1, 1, fsm_type::tr(unescaped_char_tr) , fsm_type::accept, 0, 0 }
        , {-1,-1, fsm_type::tr(escaped_tr)        , fsm_type::accept, 0, 0 }
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
          { 1,-1, fsm_type::tr(string_tr)         , fsm_type::normal, begin_member, 0 }
        , { 2,-1, fsm_type::tr(name_separator_tr) , fsm_type::normal, 0, 0 }
        , {-1,-1, fsm_type::tr(p_value_tr)        , fsm_type::accept, end_member, 0 }
    };

    /* next-member = value-separator member */
    static transition_type const next_member_tr[] = {
          { 1,-1, fsm_type::tr(value_separator_tr), fsm_type::normal, 0, 0 }
        , {-1,-1, fsm_type::tr(member_tr)         , fsm_type::accept, 0, 0 }
    };

    /* object-body = member *next-member */
    static transition_type const object_body_tr[] = {
          { 1,-1, fsm_type::tr(member_tr)                , fsm_type::normal, 0, 0 }
        , {-1,-1, fsm_type::rpt_tr(next_member_tr, 0,-1) , fsm_type::accept, 0, 0 }
    };

    /* object = begin-object [ member *( value-separator member ) ] end-object */
    /* object = begin-object [ object-body ] end-object */
    static transition_type const object_tr[] = {
          { 1,-1, fsm_type::tr(begin_object_tr)    , fsm_type::normal, begin_object, 0 }
        , { 2,-1, fsm_type::opt_tr(object_body_tr) , fsm_type::normal, 0, 0 }
        , {-1,-1, fsm_type::tr(end_object_tr)      , fsm_type::accept, end_object, 0 }
    };

    /* === Arrays === */
    /* next-value = value-separator value  */
    static transition_type const next_value_tr[] = {
          { 1,-1, fsm_type::tr(value_separator_tr), fsm_type::normal, 0, 0 }
        , {-1,-1, fsm_type::tr(p_value_tr)          , fsm_type::accept, 0, 0 }
    };

    /* array-body = value *next-value */
    static transition_type const array_body_tr[] = {
          { 1,-1, fsm_type::tr(p_value_tr)              , fsm_type::normal, 0, 0 }
        , {-1,-1, fsm_type::rpt_tr(next_value_tr, 0,-1) , fsm_type::accept, 0, 0 }
    };

    /* array = begin-array [ value *( value-separator value ) ] end-array */
    /* array = begin-array [ array-body ] end-array */
    static transition_type const array_tr[] = {
          { 1,-1, fsm_type::tr(begin_array_tr)    , fsm_type::normal, begin_array, 0 }
        , { 2,-1, fsm_type::opt_tr(array_body_tr) , fsm_type::normal, 0, 0 }
        , {-1,-1, fsm_type::tr(end_array_tr)      , fsm_type::accept, end_array, 0 }
    };


    /* === Values === */
    static string_traits JSON_FALSE("false");
    static string_traits JSON_NULL("null");
    static string_traits JSON_TRUE("true");

    static const int VALUE_NULL   = 1;
    static const int VALUE_FALSE  = 2;
    static const int VALUE_TRUE   = 3;
    static const int VALUE_NUMBER = 4;
    static const int VALUE_STRING = 5;

    /* value = false / null / true / object / array / number / string */
    static transition_type const value_tr[] = {
          {-1, 1, fsm_type::seq(JSON_FALSE), fsm_type::accept, scalar_value, (void *)& VALUE_FALSE }
        , {-1, 2, fsm_type::seq(JSON_NULL) , fsm_type::accept, scalar_value, (void *)& VALUE_NULL }
        , {-1, 3, fsm_type::seq(JSON_TRUE) , fsm_type::accept, scalar_value, (void *)& VALUE_TRUE }
        , {-1, 4, fsm_type::tr(number_tr)  , fsm_type::accept, scalar_value, (void *)& VALUE_NUMBER }
        , {-1, 5, fsm_type::tr(string_tr)  , fsm_type::accept, scalar_value, (void *)& VALUE_STRING }
        , {-1, 6, fsm_type::tr(object_tr)  , fsm_type::accept, 0, 0 }//value, (void*)&_JSON_VALUE_OBJECT }
        , {-1,-1, fsm_type::tr(array_tr)   , fsm_type::accept, 0, 0 }//value, (void*)&_JSON_VALUE_ARRAY }
    };
    
    /* JSON-text = object / array */
    const bool JSON_STATUS_OK = true;
    const bool JSON_STATUS_FAIL = false;
    static transition_type const json_tr[] = {
          { 2, 1, fsm_type::tr(object_tr) , fsm_type::normal, 0, 0 }
        , { 2, 3, fsm_type::tr(array_tr)  , fsm_type::normal, 0, 0 }
        , {-1,-1, fsm_type::nothing()     , fsm_type::accept, end_json, (void *)& JSON_STATUS_OK }
        , {-1,-1, fsm_type::nothing()     , fsm_type::reject, end_json, (void *)& JSON_STATUS_FAIL }
    };
    
#if PFS_TEST
    p_number_tr = number_tr;
#endif
    
    p_value_tr = value_tr;
}

#if PFS_TEST
template <typename ValueT, template <typename> class StackT>
typename grammar<ValueT, StackT>::transition_type const * grammar<ValueT, StackT>::p_number_tr = 0;
#endif

template <typename ValueT, template <typename> class StackT>
typename grammar<ValueT, StackT>::string_traits 
grammar<ValueT, StackT>::unescape_chars (string_traits & s)
{
	string_traits r;
	bool escaped = false;
//	string::const_iterator it = s.cbegin();
//	string::const_iterator itEnd = s.cend();
//
//	while (it != itEnd) {
//		if (!escaped && *it == string::value_type('\\')) {
//			escaped = true;
//		} else {
//			if (escaped) {
//				if (*it == string::value_type('b'))
//					r.append(1, string::value_type('\b'));
//				else if (*it == string::value_type('f'))
//					r.append(1, string::value_type('\f'));
//				else if (*it == string::value_type('n'))
//					r.append(1, string::value_type('\n'));
//				else if (*it == string::value_type('r'))
//					r.append(1, string::value_type('\r'));
//				else if (*it == string::value_type('t'))
//					r.append(1, string::value_type('\t'));
//				else
//					r.append(1, *it);
//			} else {
//				r.append(1, *it);
//			}
//			escaped = false;
//		}
//		++it;
//	}
//
//	PFS_ASSERT(!escaped);
//	return r;

	// TODO unescape hexdigits in form: \uXXXX
}

template <typename ValueT, template <typename> class StackT>
bool grammar<ValueT, StackT>::scalar_value (const_iterator begin
            , const_iterator end
            , void * context
            , void * action_args)
{
    
}

template <typename ValueT, template <typename> class StackT>
bool grammar<ValueT, StackT>::end_json (const_iterator begin
            , const_iterator end
            , void * context
            , void * action_args)
{
    
}

template <typename ValueT, template <typename> class StackT>
bool grammar<ValueT, StackT>::begin_member (const_iterator begin
            , const_iterator end
            , void * context
            , void * action_args)
{
	if (context) {
		user_context * ctx = static_cast<user_context *>(context);
		string_type str(begin, end);
//
//		PFS_ASSERT(str.length() >= 2);
//		PFS_ASSERT(str.starts_with(dquote));
//		PFS_ASSERT(str.ends_with(dquote));
//
		ctx->memberName = str.substr(1, str.length()-2);
		ctx->memberName = unescape_chars(ctx->memberName);
	}
    
	return true;
}

template <typename ValueT, template <typename> class StackT>
bool grammar<ValueT, StackT>::end_member (const_iterator begin
            , const_iterator end
            , void * context
            , void * action_args)
{
    
}

template <typename ValueT, template <typename> class StackT>
bool grammar<ValueT, StackT>::begin_object (const_iterator begin
            , const_iterator end
            , void * context
            , void * action_args)
{
    
}

template <typename ValueT, template <typename> class StackT>
bool grammar<ValueT, StackT>::end_object (const_iterator begin
            , const_iterator end
            , void * context
            , void * action_args)
{
    
}

template <typename ValueT, template <typename> class StackT>
bool grammar<ValueT, StackT>::begin_array (const_iterator begin
            , const_iterator end
            , void * context
            , void * action_args)
{
    
}

template <typename ValueT, template <typename> class StackT>
bool grammar<ValueT, StackT>::end_array (const_iterator begin
            , const_iterator end
            , void * context
            , void * action_args)
{
    
}

#if __COMMENT__



inline parse_context * context_cast (void * p)
{
	return static_cast<parse_context *>(p);
}


static const bool JSON_OBJECT = true;
static const bool JSON_ARRAY  = false;


static bool end_member (string::const_iterator /*begin*/, string::const_iterator /*end*/, void * context, void * /*action_args*/)
{
	if (context) {
		parse_context *ctx = context_cast(context);
		ctx->memberName.clear();
	}
	return true;
}

static bool end_json (string::const_iterator /*begin*/, string::const_iterator /*end*/, void * context, void * action_args)
{
	if (context) {
		const bool * pstatus = (const bool*)action_args;
		parse_context *ctx = context_cast(context);

		// Bad source or unexpected end of json
		if (!*pstatus) {
			ctx->nx->append(string_traits("bad source or unexpected end of json"));
			return false;
		}
		return ctx->on_end_json(ctx->userContext, *pstatus);
	}
	return true;
}

static bool begin_object (string::const_iterator /*begin*/, string::const_iterator /*end*/, void * context, void * /*action_args*/)
{
	bool r = true;
	if (context) {
		parse_context * ctx = context_cast(context);
		ctx->objects.push(ctx->memberName);
		r = ctx->isBeginJson
				? ctx->on_begin_json(ctx->userContext, JSON_OBJECT)
				: ctx->on_begin_object(ctx->userContext, ctx->memberName);
		ctx->isBeginJson = false;
		ctx->memberName.clear();
	}
	return r;
}

static bool end_object (string::const_iterator /*begin*/, string::const_iterator /*end*/, void * context, void * /*action_args*/)
{
	if (context) {
		parse_context * ctx = context_cast(context);
		PFS_ASSERT(ctx->objects.size() > 0);
		string memberName(ctx->objects.top());
		ctx->objects.pop();
		return ctx->on_end_object(ctx->userContext, memberName);
	}
	return true;
}


static bool begin_array (string::const_iterator /*begin*/, string::const_iterator /*end*/, void * context, void * /*action_args*/)
{
	bool r = true;
	if (context) {
		parse_context * ctx = context_cast(context);
		ctx->arrays.push(ctx->memberName);
		r = ctx->isBeginJson
				? ctx->on_begin_json(ctx->userContext, JSON_ARRAY)
				: ctx->on_begin_array(ctx->userContext, ctx->memberName);
		ctx->isBeginJson = false;
		ctx->memberName.clear();
	}
	return r;
}

static bool end_array (string::const_iterator /*begin*/, string::const_iterator /*end*/, void * context, void * /*action_args*/)
{
	if (context) {
		parse_context * ctx = context_cast(context);
		PFS_ASSERT(ctx->arrays.size() > 0);
		string memberName(ctx->arrays.top());
		ctx->arrays.pop();
		return ctx->on_end_array(ctx->userContext, memberName);
	}
	return true;
}

static bool scalar_value (string::const_iterator begin, string::const_iterator end, void * context, void * action_args)
{
	if (!context)
		return true;

	bool r = true;

	PFS_ASSERT(action_args);
	parse_context * ctx = context_cast(context);
	int flag = *static_cast<const int *>(action_args);

	PFS_ASSERT(flag == JSON_VALUE_NULL
			|| flag == JSON_VALUE_FALSE
			|| flag == JSON_VALUE_TRUE
			|| flag == JSON_VALUE_NUMBER
			|| flag == JSON_VALUE_STRING);

	switch (flag) {
	case JSON_VALUE_NULL:  r = ctx->on_null_value(ctx->userContext, ctx->memberName); break;
	case JSON_VALUE_FALSE: r = ctx->on_boolean_value(ctx->userContext, ctx->memberName, false); break;
	case JSON_VALUE_TRUE:	r = ctx->on_boolean_value(ctx->userContext, ctx->memberName, true); break;

	case JSON_VALUE_NUMBER: {
			string number(begin, end);
			bool ok = false;

			real_t d = lexical_cast<real_t>(number, & ok);

			if (!ok) {
				string errstr(string_traits("invalid number value: "));
				errstr.append(string(begin, end));
				ctx->nx->append(errstr);
				r = false;
			} else {
				r = ctx->on_number_value(ctx->userContext, ctx->memberName, d);
			}
		}
		break;
	case JSON_VALUE_STRING: {
			string str(begin, end);

			PFS_ASSERT(str.length() >= 2);
			PFS_ASSERT(str.starts_with(dquote));
			PFS_ASSERT(str.ends_with(dquote));

			str = str.substr(1, str.length()-2);
			str = unescape_chars(str);
			r = ctx->on_string_value(ctx->userContext, ctx->memberName, str);
		}
		break;
	default:
		break;
	}

	ctx->memberName.clear();
	return r;
}

#endif

}} // pfs::json

#endif /* __PFS_JSON_RFC4627_HPP__ */
