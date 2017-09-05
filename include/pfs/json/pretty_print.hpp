/* 
 * File:   pretty_print.hpp
 * Author: wladt
 *
 * Created on September 5, 2017, 11:51 AM
 */

#ifndef __PFS_JSON_PRETTY_PRINT_HPP__
#define __PFS_JSON_PRETTY_PRINT_HPP__

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

template <typename JsonType>
class pretty_printer
{
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
        string new_line;
        string key_separator;
        string value_separator;
        string open_object_brace;
        string close_object_brace;
        string open_array_brace;
        string close_array_brace;

        // from print_format
        string ws_symbol;
        size_t base_indent;
        size_t brace_indent;
        size_t first_item_indent;
        string ws_before_vseparator;
        string ws_after_vseparator;
        string ws_before_kvseparator;
        string ws_after_kvseparator;
        brace_position_enum brace_position;
        comma_position_enum comma_position;
    };
    
};

}} // pfs::json

#endif /* __PFS_JSON_PRETTY_PRINT_HPP__ */

