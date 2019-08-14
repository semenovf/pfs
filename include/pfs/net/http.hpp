////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2019 Vladislav Trifochkin
//
// This file is part of [pfs](https://github.com/semenovf/pfs) library.
//
// Changelog:
//      2019.08.14 Initial version
////////////////////////////////////////////////////////////////////////////////

#pragma once
#if __cplusplus >= 201103
#   include <utility>
#endif

////////////////////////////////////////////////////////////////////////////////
// 1. [Hypertext Transfer Protocol -- HTTP/1.0](https://tools.ietf.org/html/rfc1945)
// 2. [Hypertext Transfer Protocol -- HTTP/1.1](https://www.ietf.org/rfc/rfc2616.txt)
////////////////////////////////////////////////////////////////////////////////

namespace pfs {
namespace net {
namespace http {

template <typename PropertyTree>
struct property_tree_traits
{
    typedef typename PropertyTree::key_type key_type;
    typedef typename PropertyTree::mapped_type mapped_type;
};

// NOTE There is a difference between this implementation and
// std::map::merge() (C++17): if there is an element in @a target with key
// equivalent to the key of an element from @a source, then that element is
// extracted from source (in C++17 element is NOT extracted from source).
//
// Default implementation corresponds to std::map
template <typename PropertyTree>
void merge (PropertyTree & target, PropertyTree const & source)
{
    typedef typename PropertyTree::const_iterator iterator;

    for (iterator p = source.begin(), last = source.end(); p != last; ++p) {
        target[p->first] = p->second;
    }
}

////////////////////////////////////////////////////////////////////////////////
// DIGIT = <any US-ASCII digit "0".."9">
////////////////////////////////////////////////////////////////////////////////
template <typename CharT>
bool is_digit (CharT ch)
{
    return (ch >= '0' && ch <= '9');
}

////////////////////////////////////////////////////////////////////////////////
// CTL = <any US-ASCII control character (octets 0 - 31) and DEL (127)>
////////////////////////////////////////////////////////////////////////////////
template <typename CharT>
bool is_control (CharT ch)
{
    return (ch >= 0 && ch <= CharT(31)) || ch == CharT(127);
}

////////////////////////////////////////////////////////////////////////////////
// separators = "(" | ")" | "<" | ">" | "@"
//                      | "," | ";" | ":" | "\" | <">
//                      | "/" | "[" | "]" | "?" | "="
//                      | "{" | "}" | SP | HT
////////////////////////////////////////////////////////////////////////////////
template <typename CharT>
inline bool is_separator (CharT ch)
{
    return ch == CharT('(')
            || ch == CharT(')')
            || ch == CharT('<')
            || ch == CharT('>')
            || ch == CharT('@')
            || ch == CharT(',')
            || ch == CharT(';')
            || ch == CharT(':')
            || ch == CharT('\'')
            || ch == CharT('"')
            || ch == CharT('/')
            || ch == CharT('[')
            || ch == CharT(']')
            || ch == CharT('?')
            || ch == CharT('=')
            || ch == CharT('{')
            || ch == CharT('}')
            || ch == CharT(' ') // SP
            || ch == CharT('\t'); // HT
}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
template <typename ForwardIterator>
inline void skip_spaces (ForwardIterator & p, ForwardIterator last)
{
    while (p != last && (*p == ' ' || *p == '\t'))
        ++p;
}
////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
template <typename ForwardIterator>
inline bool parse_crlf (ForwardIterator & pos, ForwardIterator last)
{
    ForwardIterator p = pos;

    if (p != last && *p++ == '\n') {
        pos = p;
        return true;
    }

    p = pos;

    if (p != last && *p++ == '\r' && p != last && *p++ == '\n') {
        pos = p;
        return true;
    }

    return false;
}

////////////////////////////////////////////////////////////////////////////////
// token = 1*<any CHAR except CTLs or separators>
////////////////////////////////////////////////////////////////////////////////
template <typename ForwardIterator>
inline bool parse_token (ForwardIterator & p, ForwardIterator last)
{
    while (p != last && !is_control(*p) && !is_separator(*p))
        ++p;
    return p == last ? false : true;
}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
template <typename ForwardIterator>
inline bool parse_token_until_lws (ForwardIterator & p, ForwardIterator last)
{
    while (p != last && *p != ' ' && *p != '\n' && *p != '\r' && *p != '\t')
        ++p;
    return true;
}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
template <typename ForwardIterator>
inline bool parse_token_until_crlf (ForwardIterator & p, ForwardIterator last)
{
    while (p != last && *p != '\n' && *p != '\r')
        ++p;
    return true;
}

////////////////////////////////////////////////////////////////////////////////
// This is naive implementation of URI token parsing.
////////////////////////////////////////////////////////////////////////////////
template <typename ForwardIterator>
inline bool parse_uri_token (ForwardIterator & p, ForwardIterator last)
{
    ForwardIterator first = p;
    return parse_token_until_lws(p, last) && p != first;
}

////////////////////////////////////////////////////////////////////////////////
// HTTP-Version = "HTTP" "/" 1*DIGIT "." 1*DIGIT
////////////////////////////////////////////////////////////////////////////////
template <typename ForwardIterator>
bool parse_http_version (ForwardIterator & pos
        , ForwardIterator last)
{
    ForwardIterator p = pos;

    if (!(p != last && *p++ == 'H'
            && p != last && *p++ == 'T'
            && p != last && *p++ == 'T'
            && p != last && *p++ == 'P'
            && p != last && *p++ == '/'))
        return false;

    if (p == last)
        return false;

    if (!is_digit(*p++))
        return false;

    while (p != last && is_digit(*p))
        ++p;

    if (p == last)
        return false;

    if (*p++ != '.')
        return false;

    if (p == last)
        return false;

    if (!is_digit(*p++))
        return false;

    while (p != last && is_digit(*p))
        ++p;

    pos = p;

    return true;
}

////////////////////////////////////////////////////////////////////////////////
// HTTP/0.9
//      Request-Line = Method SP Request-URI CRLF
//
// HTTP/1.0
// HTTP/1.1
//      Request-Line = Method SP Request-URI SP HTTP-Version CRLF
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
////////////////////////////////////////////////////////////////////////////////
template <typename ForwardIterator>
struct request_line
{
    ForwardIterator method[2];
    ForwardIterator uri[2];
    ForwardIterator version[2];
};

template <typename ForwardIterator>
bool parse_request_line (ForwardIterator & pos, ForwardIterator last
        , request_line<ForwardIterator> & rqline)
{
    ForwardIterator p = pos;
    ForwardIterator first = p;

    if (!parse_token(p, last))
        return false;

    rqline.method[0] = first;
    rqline.method[1] = p;

    skip_spaces(p, last);

    first = p;

    if (!parse_uri_token(p, last))
        return false;

    rqline.uri[0] = first;
    rqline.uri[1] = p;

    skip_spaces(p, last);

    first = p;

    if (parse_crlf(p, last)) {
        rqline.version[0] = first;
        rqline.version[1] = first;
    } else {
        if (!parse_http_version(p, last))
            return false;

        rqline.version[0] = first;
        rqline.version[1] = p;

        if (!parse_crlf(p, last))
            return false;
    }

    pos = p;
    return true;
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
    ForwardIterator key[2];
    ForwardIterator value[2];
};

template <typename ForwardIterator>
inline bool parse_field_value (ForwardIterator & pos, ForwardIterator last)
{
    return parse_token_until_crlf(pos, last);
}

template <typename ForwardIterator>
bool parse_field (ForwardIterator & pos, ForwardIterator last
        , request_field<ForwardIterator> & field)
{
    ForwardIterator p = pos;
    ForwardIterator first = p;

    if (!parse_token(p, last))
        return false;

    field.key[0] = first;
    field.key[1] = p;

    if (p == last)
        return false;

    if (*p++ != ':')
        return false;

    if (p == last)
        return false;

    skip_spaces(p, last);

    first = p;

    if (!parse_field_value(p, last))
        return false;

    field.value[0] = first;
    field.value[1] = p;

    if (p == last)
        return false;

    if (!parse_crlf(p, last))
        return false;

    pos = p;
    return true;
}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
/**
 * Simple parser for HTTP request header.
 *
 * @return property tree with header data or empty tree if header is incomplete.
 */
template <typename PropertyTree, typename ForwardIterator>
bool parse_request (PropertyTree & props
        , ForwardIterator & pos
        , ForwardIterator last)
{
    typedef typename property_tree_traits<PropertyTree>::key_type key_type;
    typedef typename property_tree_traits<PropertyTree>::mapped_type mapped_type;

    request_line<ForwardIterator> r;
    ForwardIterator p = pos;
    PropertyTree tmp;

    if (!http::parse_request_line(p, last, r))
        return false;

#if __cplusplus >= 201103L
    tmp[key_type("#method")]  = std::move(mapped_type(r.method[0], r.method[1]));
    tmp[key_type("#uri")]     = std::move(mapped_type(r.uri[0], r.uri[1]));
    tmp[key_type("#version")] = std::move(mapped_type(r.version[0], r.version[1]));
#else
    tmp[key_type("#method")]  = mapped_type(r.method[0], r.method[1]);
    tmp[key_type("#uri")]     = mapped_type(r.uri[0], r.uri[1]);
    tmp[key_type("#version")] = mapped_type(r.version[0], r.version[1]);
#endif

    do {
        http::request_field<ForwardIterator> f;

        if (!parse_field(p, last, f))
            return false;

#if __cplusplus >= 201103L
        tmp[key_type(f.key[0], f.key[1])]  = std::move(mapped_type(f.value[0], f.value[1]));
#else
        tmp[key_type(f.key[0], f.key[1])]  = mapped_type(f.value[0], f.value[1]);
#endif

        // end of HTTP header
        if (parse_crlf(p, last))
            break;

        // Unexpected end of HTTP header
        if (p == last)
            return false;

    } while (true);

    pos = p;
    merge(props, tmp);
    return true;
}

}}} // namespace pfs::net::http
