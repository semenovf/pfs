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
#include <map>
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

    // TODO Mey be need extend valid characters
    while (p != last
            && (is_token_char(*p)
                || *p == ':'
                || *p == '/'
                || *p == '?'
                || *p == '='))
        ++p;

    return compare_and_assign(pos, p);
}

////////////////////////////////////////////////////////////////////////////////
// Version = 1*DIGIT "." 1*DIGIT
////////////////////////////////////////////////////////////////////////////////
struct proto_version
{
    int major;
    int minor;
};

template <typename ForwardIterator>
bool advance_version (ForwardIterator & pos, ForwardIterator last
        , proto_version * version = 0)
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
struct protocol
{
    proto_enum proto;
    proto_version version;
    range<ForwardIterator> s;
};

template <typename ForwardIterator>
bool advance_proto_version (ForwardIterator & pos, ForwardIterator last
        , protocol<ForwardIterator> * protocol = 0/*
        , proto_enum * proto = 0
        , proto_version * version = 0*/)
{
    proto_enum proto = UNKNOWN_PROTO;
    ForwardIterator p = pos;

    if (advance_sequence(p, last, "HTTP")) {
        proto = HTTP_PROTO;
    } else if (advance_sequence(p, last, "RTSP")) {
        proto = RTSP_PROTO;
    } else {
        return false;
    }

    if (p == last)
        return false;

    if (*p++ != '/')
        return false;

    proto_version * version = protocol ? & protocol->version : 0;

    if (!advance_version(p, last, version))
        return false;

    if (protocol) {
        protocol->proto = proto;
        protocol->s.first = pos;
        protocol->s.second = p;
    }

    return compare_and_assign(pos, p);
}

/**
 * Converts protocol enumeration value to C-string.
 */
template <typename StringT>
StringT to_string (proto_enum proto)
{
    switch (proto) {
        case HTTP_PROTO: return StringT("HTTP");
        case RTSP_PROTO: return StringT("RTSP");
        default: break;
    }

    return StringT();
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
    protocol<ForwardIterator> proto;
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

    protocol<ForwardIterator> proto;

    if (!advance_proto_version(p, last, & proto))
        return false;

    advance_spaces(p, last);

    if (!advance_crlf(p, last))
        return false;

    if (rq_line) {
        rq_line->proto = proto;
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

    inserter<PropertyTree> inserter(props);

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

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
template <typename StringT>
class property_base
{
protected:
    StringT _pretty_name;
    bool _set;

private:
    property_base (property_base const &) {}
    property_base & operator = (property_base const &) { return *this; }

public:
    property_base ()
        : _set(false)
    {}

    explicit property_base (char const * pretty_name)
        : _pretty_name(pretty_name)
        , _set(false)
    {}

    bool is_set () const
    {
        return _set;
    }

    StringT const & pretty_name () const
    {
        return _pretty_name;
    }
};

/**
 */
template <typename T, typename StringT>
class property : public property_base<StringT>
{
    typedef property_base<StringT> base_class;

    T _value;

private:
    property (property const &) {}
    T & operator = (property const &) { return *this; }

public:
    property ()
        : base_class()
    {}

    explicit property (char const * pretty_name)
        : base_class(pretty_name)
    {}

    property (char const * pretty_name, T const & value)
        : base_class(pretty_name)
        , _value(value)
    {
        this->_set = true;
    }

    property & operator = (T const & value)
    {
        _value = value;
        this->_set = true;
        return *this;
    }

    T const & value () const
    {
        return _value;
    }

    T * operator -> ()
    {
        return & _value;
    }

    T const * operator -> () const
    {
        return & _value;
    }
};

template <typename StringT>
class message
{
    typedef std::map<StringT, StringT> property_tree;

public:
    typedef StringT string_type;
    typedef string_type key_type;
    typedef string_type mapped_type;
    typedef typename property_tree::iterator iterator;
    typedef typename property_tree::const_iterator const_iterator;

protected:
    string_type   _method;
    string_type   _uri;
    string_type   _proto_version;
    proto_enum    _protocol;
    int           _major_version;
    int           _minor_version;
    property_tree _headers;

public:
    message ()
        : _protocol(UNKNOWN_PROTO)
        , _major_version(0)
        , _minor_version(0)
    {}

    string_type const & method () const { return _method; }
    string_type const & uri () const { return _uri; }
    string_type const & proto_version () const { return _proto_version; }
    proto_enum  protocol () const { return _protocol; }
    int major_version () const { return _major_version; }
    int minor_version () const { return _minor_version; }

    string_type & operator [] (string_type const & key)
    {
        return _headers[key];
    }

    string_type const * operator [] (string_type const & key) const
    {
        return _headers[key];
    }

    bool insert (string_type const & key, string_type const & value)
    {
        _headers[key] = value;
        return true;
    }

template <typename ForwardIterator, typename StringU, template <typename> class MessageT>
friend ForwardIterator parse (ForwardIterator first
        , ForwardIterator last
        , MessageT<StringU> * msg);
};

template <typename ForwardIterator, typename StringT, template <typename> class MessageT>
ForwardIterator parse (ForwardIterator first
        , ForwardIterator last
        , MessageT<StringT> * msg)
{
    ForwardIterator pos = first;
    proto::request_line<ForwardIterator> request_line;

    if (advance_request_line(pos, last, & request_line)
            && advance_headers(pos, last, msg)) {

        msg->_method = StringT(request_line.method.first, request_line.method.second);
        msg->_uri = StringT(request_line.uri.first, request_line.uri.second);
        msg->_proto_version = StringT(request_line.proto.s.first, request_line.proto.s.second);
        msg->_protocol = request_line.proto.proto;
        msg->_major_version = request_line.proto.version.major;
        msg->_minor_version = request_line.proto.version.minor;
        return pos;
    }

    return first;
}

}}} // namespace pfs::net::proto
