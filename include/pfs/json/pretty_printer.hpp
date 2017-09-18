/* 
 * File:   pretty_print.hpp
 * Author: wladt
 *
 * Created on September 5, 2017, 11:51 AM
 */

#ifndef __PFS_JSON_PRETTY_PRINTER_HPP__
#define __PFS_JSON_PRETTY_PRINTER_HPP__

namespace pfs {
namespace json {

enum brace_position_enum
{
      brace_same_line
    , brace_next_line
};

enum comma_position_enum
{
      comma_same_line
    , comma_next_line
};

struct print_format
{
    const char * ws_symbol;       //<! whitespace symbol, default is ' ' (space)
    size_t base_indent;           //<! base indent in symbols, default is 4 
    size_t brace_indent;          //<! indent in symbols for brace, default is 0
    size_t first_item_indent;     //<! indent for first item (for comma_next_line), default is 0
    size_t ws_before_vseparator;  //<! whitespaces before value separator (comma), default is 0
    size_t ws_after_vseparator;   //<! whitespaces after value separator (comma), default is 1
    size_t ws_before_kvseparator; //<! whitespaces before key/value separator (colon), default is 0
    size_t ws_after_kvseparator;  //<! whitespaces after key/value separator (colon), default is 1
    brace_position_enum brace_position; //<! brace position, default is @c brace_same_line
    comma_position_enum comma_position; //<! vseparator position, default is @c comma_same_line
};

// See description at https://en.wikipedia.org/wiki/Indent_style
//
enum print_style_enum
{
      style_plain              // all in one line
    , style_kr                 // Kernighan and Ritchie, base_indent = 4, brace_position = brace_same_line, comma_position = comma_same_line
    , style_bsd                // BSD (Allman), base_indent = 4, brace_position = brace_next_line, comma_position = comma_same_line
    , style_allman = style_bsd // BSD (Allman), base_indent = 4, brace_position = brace_next_line, comma_position = comma_same_line
    , style_gnu                // base_indent = 2, brace_position = brace_next_line, comma_position = comma_same_line
    , style_whitesmiths        // base_indent = 4, brace_position = brace_next_line_indent, comma_position = comma_same_line
    , style_favorite           // base_indent = 4, brace_position = brace_same_line, comma_position = comma_next_line
};

template <typename JsonTraits>
class pretty_printer
{
    typedef json<JsonTraits>                json_type;
    typedef typename json_type::string_type string_type;
    typedef typename json_type::key_type    key_type;
    
    /*
     * Brace Positions:
     * ===================
     *
     * brace_same_line:
     * -------------------
     * "object" : {
     * 		"foo" : 100,
     * 		"bar" : "Hello"
     * }
     *
     * brace_next_line
     * -------------------
     * "object" :
     * {
     * 		"foo" : 100,
     * 		"bar" : "Hello"
     * }
     *
     * brace_next_line_indent
     * -------------------
     * "object" :
     * 		{
     * 		    "foo" : 100,
     * 		    "bar" : "Hello"
     * 		    "fee" :
     * 		        [
     *                  100,
     * 		            200
     * 		        ]
     * 		}
     *
     *
     * Comma Positions:
     * ===================
     *
     * comma_same_line:
     * -------------------
     *
     * 	"foo" : 100,
     * 	"bar" : "Hello"
     *
     * comma_next_line
     * -------------------
     * 	"foo" : 100
     * 	, "bar" : "Hello"
     *
     * comma_next_line_indent
     * -------------------
     * 	  "foo" : 100
     * 	, "bar" : "Hello"
     *
     */
    struct print_spec
    {
        string_type new_line;
        string_type key_separator;
        string_type value_separator;
        string_type open_object_brace;
        string_type close_object_brace;
        string_type open_array_brace;
        string_type close_array_brace;

        // from print_format
        string_type ws_symbol;
        int         base_indent;
        int         brace_indent;
        int         first_item_indent;
        string_type ws_before_vseparator;
        string_type ws_after_vseparator;
        string_type ws_before_kvseparator;
        string_type ws_after_kvseparator;
        brace_position_enum brace_position;
        comma_position_enum comma_position;
    };

    static void __init_print_spec (print_spec & pspec, print_format const & format)
    {
        pspec.new_line           = "\n";
        pspec.key_separator      = ":";
        pspec.value_separator    = ",";
        pspec.open_object_brace  = "{";
        pspec.close_object_brace = "}";
        pspec.open_array_brace   = "[";
        pspec.close_array_brace  = "]";

        pspec.ws_symbol             = format.ws_symbol;     //_u8(" ")
        pspec.base_indent           = format.base_indent;   // 4
        pspec.brace_indent          = format.brace_indent;  // 0;
        pspec.first_item_indent     = format.first_item_indent; // 0
        pspec.ws_before_vseparator  = __repeat(format.ws_symbol, format.ws_before_vseparator);
        pspec.ws_after_vseparator   = __repeat(format.ws_symbol, format.ws_after_vseparator);
        pspec.ws_before_kvseparator = __repeat(format.ws_symbol, format.ws_before_kvseparator);
        pspec.ws_after_kvseparator  = __repeat(format.ws_symbol, format.ws_after_kvseparator);
        pspec.brace_position        = format.brace_position;        // json::brace_same_line;
        pspec.comma_position        = format.comma_position;        // json::comma_same_line;
    }
    
    static inline string_type & __repeat (string_type & result
            , string_type const & s
            , int n)
    {
        while (n--) {
            result.append(s);
        }
        return result;
    }

    static inline string_type __repeat (string_type const & s, int n)
    {
        string_type r;
        return __repeat(r, s, n);
    }
    
    static void __print_scalar (string_type & result
            , json_type const & v)
    {
        if (v.is_null()) {
            result.append("null");
        } else if (v.is_string()) {
            result.append(1, '"');
            result.append(v.template get<string_type>());
            result.append(1, '"');
        } else {
            result.append(v.template get<string_type>());
        }
    }

    static inline void __print_indent (string_type & result
            , print_spec const & pspec
            , int indent)
    {
        __repeat(result, pspec.ws_symbol, indent);
    }


    static inline void __print_brace_indent (string_type & result
            , print_spec const & pspec
            , int /*indent*/)
    {
        __repeat(result, pspec.ws_symbol, pspec.brace_indent);
    }

    static void __print_comma (string_type & result
            , print_spec const & pspec
            , int indent)
    {
        switch (pspec.comma_position) {

        // <ws_before_vseparator><comma><ws_after_vseparator><new_line>
        //        
        case comma_same_line:
            result.append(pspec.ws_before_vseparator);
            result.append(pspec.value_separator);
            result.append(pspec.ws_after_vseparator);
            result.append(pspec.new_line);
            __print_indent(result, pspec, indent);
            break;

        // <new_line><indent><ws_before_vseparator><comma><ws_after_vseparator>
        //        
        case comma_next_line:
            result.append(pspec.new_line);
            __print_indent(result, pspec, indent);
            //__repeat(result, pspec.ws_symbol, indent);
            result.append(pspec.ws_before_vseparator);
            result.append(pspec.value_separator);
            result.append(pspec.ws_after_vseparator);
            break;
        }
    }

    static inline void __print_open_brace (string_type & result
            , json_type const & v
            , print_spec const & pspec)
    {
        if (v.is_array()) {
            result.append(pspec.open_array_brace);
        } else {
            result.append(pspec.open_object_brace);
        }
    }

    static inline void __print_close_brace (string_type & result
            , json_type const & v
            , print_spec const & pspec)
    {
        if (v.is_array()) {
            result.append(pspec.close_array_brace);
        } else {
            result.append(pspec.close_object_brace);
        }
    }

    static void __print_value (string_type & result
            , json_type const & value
            , print_spec const & pspec
            , int indent)
    {
    //    __print_indent(result, pspec, indent);

        if (value.is_scalar()) {
            __print_scalar(result, value);
        } else {
            __print_container(result, value, pspec, indent);
        }
    }    

    static void __print_value (string_type & result
            , key_type const & key
            , json_type const & value
            , print_spec const & pspec
            , int indent)
    {
        // Enclose key into quotes
        result.append(1, '"');
        result.append(key);
        result.append(1, '"');

        result.append(pspec.ws_before_kvseparator);
        result.append(pspec.key_separator);

        if (value.is_scalar()) {
            result.append(pspec.ws_after_kvseparator);
            __print_scalar(result, value);
        } else {
            if (pspec.brace_position == brace_same_line) {
                result.append(pspec.ws_after_kvseparator);
            } else if (pspec.brace_position == brace_next_line) {
                result.append(pspec.new_line);
                __print_indent(result, pspec, indent);
                __print_brace_indent(result, pspec, pspec.brace_indent);
            }

            indent += pspec.brace_indent;
            __print_container(result, value, pspec, indent);
            indent -= pspec.brace_indent;
        }
    }

    static void __print_container (string_type & result
            , json_type const & value
            , print_spec const & pspec
            , int indent)
    {
        if (value.size() == 0) { 
            __print_open_brace(result, value, pspec);
            __print_close_brace(result, value, pspec);
            return;
        }

        typename json_type::const_iterator it_begin = value.cbegin();
        typename json_type::const_iterator it_end   = value.cend();
        typename json_type::const_iterator it       = it_begin;
        typename json_type::const_iterator it_next  = it_begin;

        ++it_next;

        indent += pspec.base_indent;

        __print_open_brace(result, value, pspec);
        result.append(pspec.new_line); // container content always begin after new line

        for (; it != it_end; ++it, ++it_next) {
            if (it == it_begin) {
                __print_indent(result, pspec, indent);

                if (pspec.comma_position == comma_next_line) {
                    __print_indent(result, pspec, pspec.first_item_indent);
                }
            }

            if (value.is_object()) {
                __print_value(result, it.key(), *it, pspec, indent);
            } else {
                __print_value(result, *it, pspec, indent);
            }

            if (it_next != it_end) {
                __print_comma(result, pspec, indent);
            }
        }

        indent -= pspec.base_indent;    

        result.append(pspec.new_line);
        __print_indent(result, pspec, indent);
        __print_close_brace(result, value, pspec);
    }

public:
    static void to_string (string_type & result
            , json_type const & v
            , print_spec const & pspec)
    {
        if (v.is_scalar())
            __print_scalar(result, v);
        else
            __print_container(result, v, pspec, 0);
    }

    static void to_string (string_type & result
            , json_type const & v
            , print_format const & format)
    {
        print_spec pspec;
        __init_print_spec(pspec, format);
        to_string(result, v, pspec);
    }
    
    static void to_string (string_type & result
            , json_type const & v
            , print_style_enum style)
    {
        print_format format;

        format.ws_symbol = " ";
        format.base_indent           = 0;
        format.brace_indent          = 0;
        format.first_item_indent     = 0;
        format.ws_before_vseparator  = 0;
        format.ws_after_vseparator   = 0;
        format.ws_before_kvseparator = 0;
        format.ws_after_kvseparator  = 0;

        switch (style) {
        // Kernighan and Ritchie
        //-------------------
        // while (x == y) {
        //     something();
        //     somethingelse();
        // }
        case style_kr:
            format.base_indent = 4;
            format.ws_after_kvseparator = 1;
            format.brace_position = brace_same_line;
            format.comma_position = comma_same_line;
            break;

        // BSD (Allman)
        //-------------------
        // while (x == y)
        // {
        //     something();
        //     somethingelse();
        // }
        case style_bsd:
            format.base_indent = 4;
            format.ws_after_kvseparator = 1;
            format.brace_position = brace_next_line;
            format.comma_position = comma_same_line;
            break;

        // GNU
        //-------------------
        // while (x == y)
        //   {
        //     something();
        //     somethingelse();
        //   }
        case style_gnu:
            format.base_indent = 2;
            format.brace_indent = 2;
            format.ws_after_kvseparator = 1;
            format.brace_position = brace_next_line;
            format.comma_position = comma_same_line;
            break;

        // Whitesmiths
        //-------------------
        // while (x == y)
        //     {
        //     something();
        //     somethingelse();
        //     }
        case style_whitesmiths:
            format.base_indent = 0;
            format.brace_indent = 4;
            format.ws_after_kvseparator = 1;
            format.brace_position = brace_next_line;
            format.comma_position = comma_same_line;
            break;

        case style_favorite:
            format.base_indent = 4;
            format.ws_after_kvseparator = 1;
            format.ws_after_vseparator  = 1;
            format.first_item_indent    = 2;
            format.brace_position = brace_same_line;
            format.comma_position = comma_next_line;
            break;

        case style_plain:       // all in one line
        default:
            format.brace_position = brace_same_line;
            format.comma_position = comma_same_line;
            break;
        }

        print_spec pspec;
        __init_print_spec(pspec, format);

        if (style == style_plain)
            pspec.new_line = "";

        to_string(result, v, pspec);
    }
};

}} // pfs::json

namespace pfs {

template <typename StringType, typename JsonTraits>
inline typename enable_if<is_same<StringType, typename json::json<JsonTraits>::string_type>::value, StringType>::type
to_string (json::json<JsonTraits> const & v, json::print_format const & format)
{
    StringType r;
    json::pretty_printer<JsonTraits>::to_string(r, v, format);
    return r;
}

template <typename StringType, typename JsonTraits>
typename enable_if<is_same<StringType, typename json::json<JsonTraits>::string_type>::value, StringType>::type
to_string (json::json<JsonTraits> const & v, json::print_style_enum style)
{
    StringType r;
    json::pretty_printer<JsonTraits>::to_string(r, v, style);
    return r;
}

} // pfs

#endif /* __PFS_JSON_PRETTY_PRINTER_HPP__ */

