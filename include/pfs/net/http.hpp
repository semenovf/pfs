////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2019 Vladislav Trifochkin
//
// This file is part of [pfs](https://github.com/semenovf/pfs) library.
//
// Changelog:
//      2019.08.14 Initial version
////////////////////////////////////////////////////////////////////////////////
// References
//
// 1. [Hypertext Transfer Protocol -- HTTP/1.0](https://tools.ietf.org/html/rfc1945)
// 2. [Hypertext Transfer Protocol -- HTTP/1.1](https://www.ietf.org/rfc/rfc2616.txt)
////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "protocols_common.hpp"
#include <vector>

namespace pfs {
namespace net {
namespace http {

using namespace pfs::net::proto;

/**
 * RFC-2616 (HTTP/1.1)
 * 14.32 Pragma
 *
 * Syntax:
 *
 * Pragma            = "Pragma" ":" 1#pragma-directive
 * pragma-directive  = "no-cache" | extension-pragma
 * extension-pragma  = token [ "=" ( token | quoted-string ) ]
 */
template <typename ForwardIterator, typename OutputIterator>
bool advance_pragma (ForwardIterator & pos
        , ForwardIterator last
        , OutputIterator * out = 0)
{
    ForwardIterator p = pos;

    do {
        range<ForwardIterator> pragma;
        pragma.first = pragma.second = last;

        ForwardIterator last_pos = p;

        if (!proto::advance_token(p, last))
            return false;

        // Extension pragma
        if (casecompare(last_pos, p, "no-cache") != 0) {
            if (!proto::advance_token(p, last))
                return false;

            if (p != last && *p == '=') {
                ++p;

                if (p == last)
                    return false;

                if (*p == '"') {
                    ++p;

                    while (p != last && *p != '"')
                        ++p;

                    if (p == last)
                        return false;

                    ++p;
                } else if (!proto::advance_token(p, last)) {
                    return false;
                }
            }
        }

        pragma.first = last_pos;
        pragma.second = p;

        if (out)
            *(*out)++ = pragma;

        if (p != last && *p == ',') {
            ++p;

            if (p == last)
                return false;
        } else {
            break;
        }
    } while (pos != last);

    return proto::compare_and_assign(pos, p);
}

/**
 */
template <typename ForwardIterator, typename OutputIterator>
inline bool advance_pragma (ForwardIterator & pos
        , ForwardIterator last
        , OutputIterator out)
{
    return advance_pragma(pos, last, & out);
}

/**
 * RFC-2616 (HTTP/1.1)
 * 14.41 Transfer-Encoding
 *
 * Syntax:
 *
 * Transfer-Encoding  = "Transfer-Encoding" ":" 1#transfer-coding
 * transfer-coding    = "chunked" | transfer-extension
 * transfer-extension = token *( ";" parameter )
 * parameter          = attribute "=" value
 * attribute          = token
 * value              = token | quoted-string
 */
template <typename ForwardIterator, typename OutputIterator>
bool advance_transfer_encoding (ForwardIterator & pos
        , ForwardIterator last
        , OutputIterator * out = 0)
{
    ForwardIterator p = pos;

    do {
        range<ForwardIterator> coding;
        coding.first = coding.second = last;

        ForwardIterator last_pos = p;

        if (!proto::advance_token(p, last))
            return false;

        // Transfer extension
        if (casecompare(last_pos, p, "chunked") != 0) {
            while (p != last && *p == ';') {
                ++p;

                if (!proto::advance_token(p, last))
                    return false;

                if (p == last)
                    return false;

                if (*p++ != '=')
                    return false;

                if (p == last)
                    return false;

                if (*p == '"') {
                    ++p;
                    while (p != last && *p != '"')
                        ++p;

                    if (p == last)
                        return false;

                    ++p;
                } else if (!proto::advance_token(p, last)) {
                    return false;
                }
            }
        }

        coding.first = last_pos;
        coding.second = p;

        if (out)
            *(*out)++ = coding;

        if (p != last && *p == ',') {
            ++p;

            if (p == last)
                return false;
        } else {
            break;
        }
    } while (pos != last);

    return proto::compare_and_assign(pos, p);
}

/**
 */
template <typename ForwardIterator, typename OutputIterator>
inline bool advance_transfer_encoding (ForwardIterator & pos
        , ForwardIterator last
        , OutputIterator out)
{
    return advance_transfer_encoding(pos, last, & out);
}

/**
 */
template <typename StringT>
class message : public proto::message<StringT>
{
    typedef proto::message<StringT> base_class;

public:
    typedef StringT string_type;
    typedef string_type key_type;    // for property_tree_traits
    typedef string_type mapped_type; // for property_tree_traits
    typedef std::vector<string_type> stringlist_type;
    typedef property<string_type, string_type> string_property;
    //typedef property<int, string_type> int_property;
    typedef property<stringlist_type, string_type> stringlist_property;

public:
    string_property user_agent;

    /**
     * RFC-2616 (HTTP/1.1)
     * 14.32 Pragma
     * The Pragma general-header field is used to include implementation-
     * specific directives that might apply to any recipient along the
     * request/response chain. All pragma directives specify optional
     * behavior from the viewpoint of the protocol; however, some systems
     * MAY require that behavior be consistent with the directives.
     */
    stringlist_property pragma;

    /**
     * RFC-2616 (HTTP/1.1)
     * 14.41 Transfer-Encoding
     * The Transfer-Encoding general-header field indicates what (if any)
     * type of transformation has been applied to the message body in order
     * to safely transfer it between the sender and the recipient. This
     * differs from the content-coding in that the transfer-coding is a
     * property of the message, not of the entity.
     */
    stringlist_property transfer_encoding;

public:
    message ()
        : base_class()
        , user_agent("User-Agent")
        , transfer_encoding("Transfer-Encoding")
    {}

    bool insert (string_type const & key, string_type const & value);
};

template <typename StringT>
bool message<StringT>::insert (StringT const & key, StringT const & value)
{
    typedef typename string_traits<StringT>::const_iterator const_iterator;

    if (key == "user-agent") {
        user_agent = value;
    } else if (key == "pragma") {
        if (casecompare(begin(value), end(value), "no-cache")) {
            pragma->push_back("no-cache");
        } else {
            const_iterator pos = begin(value);

            std::vector<range<const_iterator> > pragma_entries;

            if (!advance_pragma(pos, end(value)
                    , std::back_inserter(pragma_entries)))
                return false;

            for (size_t i = 0, count = pragma_entries.size(); i < count; i++) {
                pragma->push_back(
                        StringT(pragma_entries[i].first
                            , pragma_entries[i].second));
            }
        }
    } else if (key == "transfer-encoding") {
        if (casecompare(begin(value), end(value), "chunked")) {
            transfer_encoding->push_back("chunked");
        } else {
            const_iterator pos = begin(value);

            std::vector<range<const_iterator> > transfer_encoding_entries;

            if (!advance_transfer_encoding(pos, end(value)
                    , std::back_inserter(transfer_encoding_entries)))
                return false;

            for (size_t i = 0, count = transfer_encoding_entries.size(); i < count; i++) {
                transfer_encoding->push_back(
                        StringT(transfer_encoding_entries[i].first
                            , transfer_encoding_entries[i].second));
            }
        }
    } else {
        base_class::insert(key, value);
    }

    return true;
}

}}} // namespace pfs::net::http

namespace pfs {
namespace net {
namespace proto {

template <>
inline bool inserter<http::message<std::string> >::insert (std::string const & key
        , std::string const & value)
{
    if (_p)
        return _p->insert(key, value);
    return true;
}

}}} // pfs::net::proto

