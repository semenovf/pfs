////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2019 Vladislav Trifochkin
//
// This file is part of [pfs](https://github.com/semenovf/pfs) library.
//
// Changelog:
//      2019.08.18 Initial version
////////////////////////////////////////////////////////////////////////////////
#pragma once
#include <pfs/algo/advance.hpp>
#include <pfs/algo/compare.hpp>
#include <algorithm>
#include <iterator>
#include <limits>
#include <string>
#include <utility>
#include <cerrno>
#include <cstdlib>
#include <cstdio>

namespace pfs {
namespace net {
namespace proto {

#if __cplusplus >= 201103L
    template <typename T>
    using range = std::pair<T,T>;

    using std::begin;
    using std::end;
#else
    template <typename T>
    struct range: public std::pair<T,T> {};

    template <typename C>
    inline typename C::const_iterator begin (C const & c) { return c.begin(); }

    template <typename C>
    inline typename C::iterator begin (C & c) { return c.begin(); }

    template <typename C>
    inline typename C::const_iterator end (C const & c) { return c.end(); }

    template <typename C>
    inline typename C::iterator end (C & c) { return c.end(); }

#endif

// Default implementation corresponds to std::map
template <typename PropertyTree>
struct property_tree_traits
{
    typedef typename PropertyTree::key_type key_type;
    typedef typename PropertyTree::mapped_type mapped_type;
    typedef typename PropertyTree::iterator iterator;
    typedef typename PropertyTree::const_iterator const_iterator;
};

template <typename StringT>
struct string_traits
{
    typedef typename StringT::value_type value_type;
    typedef typename StringT::iterator iterator;
    typedef typename StringT::const_iterator const_iterator;
};

//// Default implementation corresponds to std::map
//template <typename PropertyTree>
//inline typename property_tree_traits<PropertyTree>::mapped_type *
//mapped_value (PropertyTree & props
//        , typename property_tree_traits<PropertyTree>::key_type const & key)
//{
//    typedef proto::property_tree_traits<PropertyTree> traits;
//    typedef typename traits::mapped_type mapped_type;
//    typedef typename traits::iterator iterator;

//    iterator it = props.find(key);
//    return it == end(props)
//            ? static_cast<mapped_type *>(0)
//            : & it->second;
//}

//// Default implementation corresponds to std::map
//template <typename PropertyTree>
//inline typename property_tree_traits<PropertyTree>::mapped_type const *
//mapped_value (PropertyTree const & props
//        , typename property_tree_traits<PropertyTree>::key_type const & key)
//{
//    typedef proto::property_tree_traits<PropertyTree> traits;
//    typedef typename traits::mapped_type mapped_type;
//    typedef typename traits::const_iterator const_iterator;

//    const_iterator it = props.find(key);
//    return it == end(props)
//            ? static_cast<mapped_type const *>(0)
//            : & it->second;
//}

// NOTE There is a difference between this implementation and
// std::map::merge() (C++17): if there is an element in @a target with key
// equivalent to the key of an element from @a source, then that element is
// extracted from source (in C++17 element is NOT extracted from source).
//
// Default implementation corresponds to std::map
//template <typename PropertyTree>
//void merge (PropertyTree & target, PropertyTree const & source)
//{
//    typedef typename PropertyTree::const_iterator iterator;
//
//    for (iterator p = source.begin(), last = source.end(); p != last; ++p) {
//        target[p->first] = p->second;
//    }
//}

/**
 * Inserter trait for insert value into property tree by key.
 */
template <typename PropertyTree>
class inserter
{
    typedef property_tree_traits<PropertyTree> traits;
    typedef typename traits::key_type key_type;
    typedef typename traits::mapped_type mapped_type;

    PropertyTree * _p;

public:
    inserter (PropertyTree * p = static_cast<PropertyTree *>(0)) : _p(p) {}

    bool insert (key_type const & key, mapped_type const & value)
    {
        if (_p) (*_p)[key] = value;
        return true;
    }
};

/**
 * Accessor trait for access mapped value from property tree.
 */
template <typename PropertyTree>
class accessor
{
    typedef property_tree_traits<PropertyTree> traits;
    typedef typename traits::key_type key_type;
    typedef typename traits::mapped_type mapped_type;
    typedef typename traits::iterator iterator;
    typedef typename traits::const_iterator const_iterator;

    PropertyTree * _p;

public:
    accessor (PropertyTree * p = static_cast<PropertyTree *>(0)) : _p(p) {}

    mapped_type * get (key_type const & key)
    {
        if (!_p)
            return static_cast<mapped_type *>(0);

        iterator it = _p->find(key);
        return it == end(*_p) ? static_cast<mapped_type *>(0) : & it->second;
    }

    mapped_type const * get (key_type const & key) const
    {
        if (!_p)
            return static_cast<mapped_type *>(0);

        const_iterator it = _p->find(key);
        return it == end(*_p)? static_cast<mapped_type *>(0) : & it->second;
    }
};

/**
 *
 */
template <typename CharT>
inline CharT to_lower_char (CharT);

template <>
inline char to_lower_char (char ch)
{
    return static_cast<char>(std::tolower(ch));
}

/**
 */
template <typename StringT>
inline StringT to_lower (typename string_traits<StringT>::const_iterator first
        , typename string_traits<StringT>::const_iterator last)
{
    StringT result;
    std::transform(first, last, std::back_inserter(result)
            , to_lower_char<typename string_traits<StringT>::value_type>);
    return result;
}

template <typename StringT>
inline StringT to_lower (typename string_traits<StringT>::iterator first
        , typename string_traits<StringT>::iterator last)
{
    StringT result;
    std::transform(first, last, std::back_inserter(result)
            , to_lower_char<typename string_traits<StringT>::value_type>);
    return result;
}

template <typename StringT>
inline StringT to_lower (StringT const & s)
{
    return to_lower(begin(s), end(s));
}

/**
 * Checks whether @a ch is decimal digit.
 *
 * Syntax:
 * DIGIT = < any US-ASCII digit "0".."9" >
 */
template <typename CharT>
bool is_digit (CharT ch)
{
    return (ch >= CharT('0') && ch <= CharT('9'));
}

/**
 * Checks whether @a ch is hexidecimal digit.
 *
 * Syntax:
 * HEX = DIGIT | "A" | "B" | "C" | "D" | "E" | "F"
 *             | "a" | "b" | "c" | "d" | "e" | "f"
 */
template <typename CharT>
bool is_hexdigit (CharT ch)
{
    return is_digit(ch)
            || (ch >= CharT('A') && ch <= CharT('F'))
            || (ch >= CharT('a') && ch <= CharT('f'));
}

////////////////////////////////////////////////////////////////////////////////
// CTL = <any US-ASCII control character (octets 0 - 31) and DEL (127)>
////////////////////////////////////////////////////////////////////////////////
template <typename CharT>
bool is_control (CharT ch)
{
    return (ch >= 0 && ch <= CharT(31)) || ch == CharT(127);
}

/**
 * tspecials = "(" | ")" | "<" | ">" | "@"
 *             | "," | ";" | ":" | "\" | <">
 *             | "/" | "[" | "]" | "?" | "="
 *             | "{" | "}" | SP | HT
 */
template <typename CharT>
inline bool is_tspecials (CharT ch)
{
    return ch == CharT('(')
            || ch == CharT(')')
            || ch == CharT('<')
            || ch == CharT('>')
            || ch == CharT('@')
            || ch == CharT(',')
            || ch == CharT(';')
            || ch == CharT(':')
            || ch == CharT('\\')
            || ch == CharT('"')
            || ch == CharT('/')
            || ch == CharT('[')
            || ch == CharT(']')
            || ch == CharT('?')
            || ch == CharT('=')
            || ch == CharT('{')
            || ch == CharT('}')
            || ch == CharT(' ')   // SP
            || ch == CharT('\t'); // HT
}

/**
 */
template <typename CharT>
inline bool is_separator (CharT ch)
{
    return is_tspecials(ch);
}

/**
 * Helper function assigns @a b to @a a if @a a != @a b.
 */
template <typename ForwardIterator>
inline bool compare_and_assign (ForwardIterator & a, ForwardIterator b)
{
    if (a != b) {
        a = b;
        return true;
    }

    return false;
}

/**
 * RFC-7826 (RTSP 2.0)
 * 20.1.Base Syntax
 * ------------------------------------------------------------------------------
 * token =  1 * (%x21 ; '!'
 *      / %x23-27     ; '#', '$', '%', '&', '''
 *      / %x2A-2B     ; '*', '+'
 *      / %x2D-2E     ; '-', '.'
 *      / %x30-39     ; '0' - '9'
 *      / %x41-5A     ; 'A' - 'Z'
 *      / %x5E-7A     ; '^', '_', '`', 'a' - 'z'
 *      / %x7C        ; '|'
 *      / %x7E)       ; '~'
 *
 * RFC-2326 (RTPS 1.0)
 * 15.1 Base Syntax
 *------------------------------------------------------------------------------
 * token = 1 * < any CHAR except CTLs or tspecials >
 *
 * RFC-2626 (HTTP 1.1)
 * 2.2 Basic Rules
 *------------------------------------------------------------------------------
 * token = 1 * < any CHAR except CTLs or separators >
 *
 * RFC-1945 (HTTP 1.0)
 * token = 1 * < any CHAR except CTLs or tspecials >
 *
 * Using more strict rule: RFC-7826
 *
 */
template <typename CharT>
inline bool is_token_char (CharT ch)
{
    return (ch == CharT('\x21'))
        || (ch >= CharT('\x23') && ch <= CharT('\x27'))
        || (ch >= CharT('\x2A') && ch <= CharT('\x2B'))
        || (ch >= CharT('\x2D') && ch <= CharT('\x2E'))
        || (ch >= CharT('\x30') && ch <= CharT('\x39'))
        || (ch >= CharT('\x41') && ch <= CharT('\x5A'))
        || (ch >= CharT('\x5E') && ch <= CharT('\x7A'))
        || (ch == CharT('\x7C'))
        || (ch == CharT('\x7E'));
}

/**
 * @return @c true if advanced by at least one position, otherwise @c false.
 */
template <typename ForwardIterator>
inline bool advance_token (ForwardIterator & pos, ForwardIterator last)
{
    ForwardIterator last_pos = pos;

    while (pos != last && is_token_char(*pos))
        ++pos;

    return pos != last_pos;
}

/**
 * @brief Advance by sequence of digits with radix @a radix.
 *
 * @param pos On input - first position, on output - last good position.
 * @param last End of sequence position.
 * @param radix Number system base.
 * @param ok If not null, stores result of conversion: @c true - success,
 *        @c false - fail.
 *
 * @return @c true if advanced by at least one position, otherwise @c false.
 */
template <typename Int, typename ForwardIterator>
bool advance_integer (ForwardIterator & pos, ForwardIterator last
        , Int * result
        , int radix = 10)
{
    Int n = 0;

    ForwardIterator p = pos;

    Int cutoff_value = std::numeric_limits<Int>::max() / radix;
    Int cutoff_limit = std::numeric_limits<Int>::max() % radix;

    while (p != last) {
        int ch = static_cast<int>(*p);
        int digit = 0;

        if (ch >= '0' && ch <= '9')
            digit = ch - '0';
        else if (ch >= 'a' && ch <= 'z')
            digit = ch - 'a' + 10;
        else if (ch >= 'A' && ch <= 'Z')
            digit = ch - 'A' + 10;
        else
            break;

        if (digit >= radix)
            break;

        if (n < cutoff_value
                || (n == cutoff_value
                        && static_cast<Int>(digit) <= cutoff_limit)) {
            n *= radix;
            n += digit;
        } else {
            // Overflow
            break;
        }

        ++p;
    }

    if (result) *result = n;

    return compare_and_assign(pos, p);
}

/**
 */
template <typename Int, typename ForwardIterator>
Int to_integer (ForwardIterator first, ForwardIterator last
        , int radix = 10
        , bool * pok = 0)
{
    Int result = 0;
    bool ok = true;

    if (!advance_integer(first, last, & result, radix)) {
        ok = false;
    }

    if (first != last)
        ok = false;

    if (pok)
        *pok = ok;

    return result;
}

////////////////////////////////////////////////////////////////////////////////
// Compare utilities
////////////////////////////////////////////////////////////////////////////////
struct until_nullchar
{
    bool operator () (char const * x) { return *x == '\0'; }
};

struct case_less
{
    bool operator () (int a, int b)
    {
        return std::toupper(a) < std::toupper(b);
    }
};

template <typename ForwardIterator>
inline int compare (ForwardIterator first, ForwardIterator last
        , char const * s)
{
    return compare_until(first, last, s, until_nullchar(), std::less<int>());
}

/**
 * Case insesitive comparing of character sequence [@a first, @a last)
 * with C-string @a s.
 */
template <typename ForwardIterator>
inline int casecompare (ForwardIterator first, ForwardIterator last
        , char const * s)
{
    return compare_until(first, last, s, until_nullchar(), case_less());
}

/**
 */
template <typename ForwardIterator>
inline bool advance_sequence (ForwardIterator & pos, ForwardIterator last
        , char const * s)
{
    ForwardIterator p = pos;
    advance_sequence_until(p, last, s, until_nullchar());
    return compare_and_assign(pos, p);
}

/**
 */
template <typename ForwardIterator>
inline bool advance_spaces (ForwardIterator & pos, ForwardIterator last)
{
    ForwardIterator p = pos;
    while (p != last && (*p == ' ' || *p == '\t'))
        ++p;
    return compare_and_assign(pos, p);
}

/**
 */
template <typename ForwardIterator>
inline bool advance_crlf (ForwardIterator & pos, ForwardIterator last)
{
    ForwardIterator p = pos;

    if (p != last && *p == '\n') {
        pos = ++p;
        return true;
    } else if (p != last && *p == '\r' && ++p != last && *p == '\n') {
        pos = ++p;
        return true;
    }

    return false;
}

/**
 */
template <typename ForwardIterator>
inline bool advance_until_crlf (ForwardIterator & pos, ForwardIterator last)
{
    ForwardIterator p = pos;
    while (p != last && *p != '\n' && *p != '\r')
        ++p;
    return compare_and_assign(pos, p);
}

/**
 * Naive implementation of URI token parsing.
 */
template <typename ForwardIterator>
inline bool advance_uri (ForwardIterator & pos, ForwardIterator last)
{
    ForwardIterator p = pos;

    while (p != last
            && (is_token_char(*p)
                || *p == ':'
                || *p == '/'))
        ++p;

    return compare_and_assign(pos, p);
}

////////////////////////////////////////////////////////////////////////////////
// Version = 1*DIGIT "." 1*DIGIT
////////////////////////////////////////////////////////////////////////////////
template <typename ForwardIterator>
struct proto_version
{
    int major;
    int minor;
};

template <typename ForwardIterator>
bool advance_version (ForwardIterator & pos, ForwardIterator last
        , proto_version<ForwardIterator> * version = 0)
{
    int major = 0;
    int minor = 0;

    ForwardIterator p = pos;

    if (p == last) return false;

    if (!advance_integer(p, last, & major))
        return false;

    if (*p++ != '.') return false;

    if (!advance_integer(p, last, & minor))
        return false;

    if (version) {
        version->major = major;
        version->minor = minor;
    }

    return compare_and_assign(pos, p);
}

/**
 * HTTP/1.0
 *      HTTP-Version = "HTTP/1.0"
 *
 * HTTP/1.1
 *      HTTP-Version = "HTTP/1.1"
 *
 * RTSP/1.0
 *      RTSP-Version = "RTSP/1.0"
 */
enum proto_enum {
      UNKNOWN_PROTO
    , HTTP_PROTO
    , RTSP_PROTO
};

template <typename ForwardIterator>
bool advance_proto_version (ForwardIterator & pos, ForwardIterator last
        , proto_enum * proto = 0
        , proto_version<ForwardIterator> * version = 0)
{
    proto_enum pr = UNKNOWN_PROTO;
    proto_version<ForwardIterator> ver;

    ForwardIterator p = pos;

    if (advance_sequence(p, last, "HTTP")) {
        pr = HTTP_PROTO;
    } else if (advance_sequence(p, last, "RTSP")) {
        pr = RTSP_PROTO;
    } else {
        return false;
    }

    if (p == last)
        return false;

    if (*p++ != '/')
        return false;

    if (!advance_version(p, last, & ver))
        return false;

    if (proto)
        *proto = pr;

    if (version)
        *version = ver;

    return compare_and_assign(pos, p);
}

/**
 * Converts protocol enumeration value to C-string.
 */
char const * to_cstring (proto_enum proto)
{
    switch (proto) {
        case HTTP_PROTO: return "HTTP";
        case RTSP_PROTO: return "RTSP";
        default: break;
    }

    return "";
}
////////////////////////////////////////////////////////////////////////////////
// HTTP/1.0
// HTTP/1.1
//      Request-Line = Method SP Request-URI SP HTTP-Version CRLF
//
// RTSP/1.0
//      Request-Line = Method SP Request-URI SP RTSP-Version CRLF
//
// HTTP/1.0
//      Method = "GET"
//             | "HEAD"
//             | "POST"
//             | extension-method
//
// HTTP/1.1
//      Method = "OPTIONS"
//             | "GET"
//             | "HEAD"
//             | "POST"
//             | "PUT"
//             | "DELETE"
//             | "TRACE"
//             | "CONNECT"
//             | extension-method
//
// RTSP/1.0
//      Method = "DESCRIBE"
//             | "ANNOUNCE"
//             | "GET_PARAMETER"
//             | "OPTIONS"
//             | "PAUSE"
//             | "PLAY"
//             | "RECORD"
//             | "REDIRECT"
//             | "SETUP"
//             | "SET_PARAMETER"
//             | "TEARDOWN"
//             | extension-method
////////////////////////////////////////////////////////////////////////////////
template <typename ForwardIterator>
struct request_line
{
    range<ForwardIterator> method;
    range<ForwardIterator> uri;
    proto_enum proto;
    proto_version<ForwardIterator> version;
};

template <typename ForwardIterator>
bool advance_request_line (ForwardIterator & pos, ForwardIterator last
        , request_line<ForwardIterator> * rq_line = 0)
{
    ForwardIterator p = pos;
    ForwardIterator last_pos = p;

    advance_token(p, last);

    if (p == last_pos) return false;

    if (rq_line) {
        rq_line->method.first = last_pos;
        rq_line->method.second = p;
    }

    if (!advance_spaces(p, last))
        return false;

    last_pos = p;

    if (!advance_uri(p, last))
        return false;

    if (rq_line) {
        rq_line->uri.first = last_pos;
        rq_line->uri.second = p;
    }

    if (!advance_spaces(p, last))
        return false;

    proto_enum proto = UNKNOWN_PROTO;
    proto_version<ForwardIterator> version;

    if (!advance_proto_version(p, last, & proto, & version))
        return false;

    advance_spaces(p, last);

    if (!advance_crlf(p, last))
        return false;

    if (rq_line) {
        rq_line->proto   = proto;
        rq_line->version = version;
    }

    return compare_and_assign(pos, p);
}

////////////////////////////////////////////////////////////////////////////////
// HTTP-header    = field-name ":" [ field-value ] CRLF
// field-name     = token
// field-value    = *( field-content | LWS )
// field-content  = <the OCTETs making up the field-value
//                  and consisting of either *TEXT or combinations
//                  of token, tspecials, and quoted-string>
////////////////////////////////////////////////////////////////////////////////
template <typename ForwardIterator>
struct request_field
{
    range<ForwardIterator> name;
    range<ForwardIterator> value;
};

template <typename ForwardIterator>
bool advance_field (ForwardIterator & pos, ForwardIterator last
        , request_field<ForwardIterator> * field = 0)
{
    ForwardIterator p = pos;
    ForwardIterator last_pos = p;

    if (!advance_token(p, last))
        return false;

    if (field) {
        field->name.first = last_pos;
        field->name.second = p;
    }

    if (p == last)
        return false;

    if (*p++ != ':')
        return false;

    if (p == last)
        return false;

    advance_spaces(p, last);

    last_pos = p;

    advance_until_crlf(p, last);

    if (field) {
        field->value.first = last_pos;
        field->value.second = p;
    }

    if (!advance_crlf(p, last))
        return false;

    return compare_and_assign(pos, p);
}

/**
 * Simple parser for HTTP-like request header.
 *
 * @return property tree with header data or empty tree if header is incomplete.
 */
template <typename PropertyTree, typename ForwardIterator>
bool advance_headers (ForwardIterator & pos
        , ForwardIterator last
        , PropertyTree * props = 0)
{
    typedef typename property_tree_traits<PropertyTree>::key_type key_type;
    typedef typename property_tree_traits<PropertyTree>::mapped_type mapped_type;

    request_line<ForwardIterator> r;
    ForwardIterator p = pos;

    if (!advance_request_line(p, last, & r))
        return false;

    mapped_type proto(to_cstring(r.proto));
    char version_buf[32];
    int index = std::snprintf(version_buf
            , sizeof(version_buf) - 1
            , "%d.%d"
            , r.version.major
            , r.version.minor);
    version_buf[index] = '\0';

    inserter<PropertyTree> inserter(props);

    if (props) {
        if (!inserter.insert(key_type("#method")
                , mapped_type(r.method.first, r.method.second)))
            return false;

        if (!inserter.insert(key_type("#uri")
                , mapped_type(r.uri.first, r.uri.second)))
            return false;

        if (!inserter.insert(key_type("#proto"), proto))
            return false;

        if (!inserter.insert(key_type("#version"), version_buf))
            return false;
    }

    do {
        request_field<ForwardIterator> f;

        if (!advance_field(p, last, & f))
            return false;

        key_type key = to_lower<key_type>(f.name.first, f.name.second);

        if (!inserter.insert(key, mapped_type(f.value.first, f.value.second)))
            return false;

        // end of HTTP header
        if (advance_crlf(p, last))
            break;

        // Unexpected end of HTTP header
        if (p == last)
            return false;

    } while (true);

    return compare_and_assign(pos, p);
}

}}} // namespace pfs::net::proto
