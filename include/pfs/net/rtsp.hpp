////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2019 Vladislav Trifochkin
//
// This file is part of [pfs](https://github.com/semenovf/pfs) library.
//
// Changelog:
//      2019.08.18 Initial version
////////////////////////////////////////////////////////////////////////////////

#pragma once
#include <iterator>
#include <map>
#include <utility>
#include <vector>
#include "protocols_common.hpp"

////////////////////////////////////////////////////////////////////////////////
// 1. [Real Time Streaming Protocol (RTSP)](https://tools.ietf.org/html/rfc2326)
// 2. [Real-Time Streaming Protocol Version 2.0](https://tools.ietf.org/html/rfc7826)
////////////////////////////////////////////////////////////////////////////////

namespace pfs {
namespace net {
namespace rtsp {

using namespace pfs::net::proto;

enum method_enum {
      RTSP_METHOD_UNKNOWN
    , RTSP_METHOD_DESCRIBE
    , RTSP_METHOD_ANNOUNCE
    , RTSP_METHOD_GET_PARAMETER
    , RTSP_METHOD_OPTIONS
    , RTSP_METHOD_PAUSE
    , RTSP_METHOD_PLAY
    , RTSP_METHOD_RECORD
    , RTSP_METHOD_REDIRECT
    , RTSP_METHOD_SETUP
    , RTSP_METHOD_SET_PARAMETER
    , RTSP_METHOD_TEARDOWN
};

/**
 * Converts sequence to method enumeration value.
 *
 * Syntax:
 * Method = "DESCRIBE"
 *          | "ANNOUNCE"
 *          | "GET_PARAMETER"
 *          | "OPTIONS"
 *          | "PAUSE"
 *          | "PLAY"
 *          | "RECORD"
 *          | "REDIRECT"
 *          | "SETUP"
 *          | "SET_PARAMETER"
 *          | "TEARDOWN"
 *          | extension-method
 */
template <typename ForwardIterator>
method_enum method_from_string (ForwardIterator first, ForwardIterator last)
{
    if (casecompare(first, last, "DESCRIBE") == 0) {
        return RTSP_METHOD_DESCRIBE;
    } else if (casecompare(first, last, "ANNOUNCE") == 0) {
        return RTSP_METHOD_ANNOUNCE;
    } else if (casecompare(first, last, "GET_PARAMETER") == 0) {
        return RTSP_METHOD_GET_PARAMETER;
    } else if (casecompare(first, last, "OPTIONS") == 0) {
        return RTSP_METHOD_OPTIONS;
    } else if (casecompare(first, last, "PAUSE") == 0) {
        return RTSP_METHOD_PAUSE;
    } else if (casecompare(first, last, "PLAY") == 0) {
        return RTSP_METHOD_PLAY;
    } else if (casecompare(first, last, "RECORD") == 0) {
        return RTSP_METHOD_RECORD;
    } else if (casecompare(first, last, "REDIRECT") == 0) {
        return RTSP_METHOD_REDIRECT;
    } else if (casecompare(first, last, "SETUP") == 0) {
        return RTSP_METHOD_SETUP;
    } else if (casecompare(first, last, "SET_PARAMETER") == 0) {
        return RTSP_METHOD_SET_PARAMETER;
    } else if (casecompare(first, last, "TEARDOWN") == 0) {
        return RTSP_METHOD_TEARDOWN;
    }

    return RTSP_METHOD_UNKNOWN;
}

template <typename StringT>
StringT to_string (method_enum method)
{
    switch (method) {
        case RTSP_METHOD_DESCRIBE:      return StringT("DESCRIBE");
        case RTSP_METHOD_ANNOUNCE:      return StringT("ANNOUNCE");
        case RTSP_METHOD_GET_PARAMETER: return StringT("GET_PARAMETER");
        case RTSP_METHOD_OPTIONS:       return StringT("OPTIONS");
        case RTSP_METHOD_PAUSE:         return StringT("PAUSE");
        case RTSP_METHOD_PLAY:          return StringT("PLAY");
        case RTSP_METHOD_RECORD:        return StringT("RECORD");
        case RTSP_METHOD_REDIRECT:      return StringT("REDIRECT");
        case RTSP_METHOD_SETUP:         return StringT("SETUP");
        case RTSP_METHOD_SET_PARAMETER: return StringT("SET_PARAMETER");
        case RTSP_METHOD_TEARDOWN:      return StringT("TEARDOWN");
        default: break;
    }

    return StringT();
}

/**
 * Parses comma-separated list of methods.
 *
 * RFC-2326
 *------------------------------------------------------------------------------
 * The method name is case sensitive (strange, but RFC allow mode name in
 * any cases in examples).
 * Method names MUST NOT start with a $ character (decimal 36)
 * and MUST be a token (proto::advance_token)
 *
 * @param pos First position of sequence.
 * @param last Last position of sequence.
 * @param methods Pointer to array to store methods (may be null).
 * @param count [in] if not null *count stores initial size of array of @a methods.
 *              [out] if not null *count stores actual number of parsed methods.
 *
 */
template <typename ForwardIterator>
bool advance_method (ForwardIterator & pos, ForwardIterator last
        , method_enum * method = 0)
{
    ForwardIterator p = pos;
    ForwardIterator last_pos = p;

    if (!proto::advance_token(p, last))
        return false;

    method_enum m = method_from_string(last_pos, p);

    if (m == RTSP_METHOD_UNKNOWN)
        return false;

    if (method)
        *method = m;

    return proto::compare_and_assign(pos, p);
}

/**
 * RFC-2326
 * 12.39 Transport
 *
 * Transports are comma separated, listed in order of preference.
 * Parameters may be added to each transport, separated by a semicolon.
 *
 * Syntax:
 * Transport          = "Transport" ":" 1#transport-spec
 * transport-spec     = transport-protocol/profile[/lower-transport] *parameter
 * transport-protocol =    "RTP"
 * profile            =    "AVP"
 * lower-transport    =    "TCP" | "UDP"
 * parameter          =    ( "unicast" | "multicast" )
 *                    |    ";" "destination" [ "=" address ]
 *                    |    ";" "interleaved" "=" channel [ "-" channel ]
 *                    |    ";" "append"
 *                    |    ";" "ttl" "=" ttl
 *                    |    ";" "layers" "=" 1*DIGIT
 *                    |    ";" "port" "=" port [ "-" port ]
 *                    |    ";" "client_port" "=" port [ "-" port ]
 *                    |    ";" "server_port" "=" port [ "-" port ]
 *                    |    ";" "ssrc" "=" ssrc
 *                    |    ";" "mode" = <"> 1#mode <">
 * ttl                =    1*3(DIGIT)
 * port               =    1*5(DIGIT)
 * ssrc               =    8*8(HEX)
 * channel            =    1*3(DIGIT)
 * address            =    host
 * mode               =    <"> *Method <"> | Method
 *
 * Example:
 * Transport: RTP/AVP;multicast;ttl=127;mode="PLAY",
 *            RTP/AVP;unicast;client_port=3456-3457;mode="PLAY"
 */
enum lower_transport_enum {
      RTSP_UNKNOWN_LOWER_TRANSPORT
    , RTSP_LOWER_TRANSPORT_UDP
    , RTSP_LOWER_TRANSPORT_TCP
    , RTSP_LOWER_TRANSPORT_UDP_MULTICAST
};

template <typename ForwardIterator>
struct transport_entry
{
    typedef std::vector<method_enum> method_container;
    typedef std::back_insert_iterator<method_container> method_insert_iterator;

    proto::range<ForwardIterator> protocol;
    proto::range<ForwardIterator> profile;
    lower_transport_enum lower_transport;
    proto::range<ForwardIterator> destination;
    //bool multicast;
    proto::range<int> interleaved;
    bool append;
    int ttl;
    int layers;
    proto::range<int> port;
    proto::range<int> client_port;
    proto::range<int> server_port;
    int ssrc;
    method_container methods;
};

template <typename ForwardIterator>
bool advance_host (ForwardIterator & pos, ForwardIterator last)
{
    ForwardIterator p = pos;

    if (!advance_integer(p, last, static_cast<int *>(0), 10)) return false;
    if (p == last || *p++ != '.') return false;

    if (!advance_integer(p, last, static_cast<int *>(0), 10)) return false;
    if (p == last || *p++ != '.') return false;

    if (!advance_integer(p, last, static_cast<int *>(0), 10)) return false;
    if (p == last || *p++ != '.') return false;

    if (!advance_integer(p, last, static_cast<int *>(0), 10)) return false;

    return proto::compare_and_assign(pos, p);
}

template <typename Int, typename ForwardIterator>
bool advance_integer_range (ForwardIterator & pos
        , ForwardIterator last
        , proto::range<Int> * range
        , int radix = 10)
{
    ForwardIterator p = pos;

    Int from, to;

    if (!advance_integer(p, last, & from, radix))
        return false;

    if (range)
        range->first = range->second = from;

    if (p != last && *p == '-') {
        if (!advance_integer(++p, last, & to, radix))
            return false;

        if (range)
            range->second = to;
    }

    return proto::compare_and_assign(pos, p);
}

template <typename ForwardIterator>
bool advance_transport_parameter (ForwardIterator & pos
        , ForwardIterator last
        , transport_entry<ForwardIterator> * tran)
{
    bool has_arg = false;
    ForwardIterator p = pos;
    ForwardIterator last_pos = p;

    ForwardIterator parm_name[2] = { last, last };
    ForwardIterator parm_value[2] = { last, last };

    if (!proto::advance_token(p, last))
        return false;

    parm_name[0] = last_pos;
    parm_name[1] = p;

    if (p != last) {
        if (*p == '=')
            ++p;

        has_arg = true;

        if (p == last)
            return false;
    }

    if (casecompare(parm_name[0], parm_name[1], "unicast") == 0) {
        ;//if (tran) tran->multicast = false;
    } else if (casecompare(parm_name[0], parm_name[1], "multicast") == 0) {
        if (tran) {
            if (tran->lower_transport == RTSP_LOWER_TRANSPORT_UDP)
                tran->lower_transport = RTSP_LOWER_TRANSPORT_UDP_MULTICAST;
        }
    } else if (casecompare(parm_name[0], parm_name[1], "destination") == 0) {
        if (has_arg) {
            last_pos = p;

            // Parse host
            if (!advance_host(p, last))
                return false;
        }

        if (tran) {
            tran->destination.first = last_pos;
            tran->destination.second = p;
        }
    } else if (casecompare(parm_name[0], parm_name[1], "interleaved") == 0) {
        proto::range<int> range;

        if (!advance_integer_range(p, last, & range, 10))
            return false;

        if (tran)
            tran->interleaved = range;
    } else if (casecompare(parm_name[0], parm_name[1], "append") == 0) {
        if (tran)
            tran->append = true;
    } else if (casecompare(parm_name[0], parm_name[1], "ttl") == 0) {
        int ttl = 0;

        if (!advance_integer(p, last, & ttl, 10))
            return false;

        if (tran)
            tran->ttl = ttl;
    } else if (casecompare(parm_name[0], parm_name[1], "layers") == 0) {
        int layers = 0;

        if (!advance_integer(p, last, & layers, 10))
            return false;

        if (tran)
            tran->layers = layers;

    } else if (casecompare(parm_name[0], parm_name[1], "port") == 0) {
        proto::range<int> range;

        if (!advance_integer_range(p, last, & range, 10))
            return false;

        if (tran)
            tran->port = range;
    } else if (casecompare(parm_name[0], parm_name[1], "client_port") == 0) {
        proto::range<int> range;

        if (!advance_integer_range(p, last, & range, 10))
            return false;

        if (tran)
            tran->client_port = range;
    } else if (casecompare(parm_name[0], parm_name[1], "server_port") == 0) {
        proto::range<int> range;

        if (!advance_integer_range(p, last, & range, 10))
            return false;

        if (tran)
            tran->server_port = range;
    } else if (casecompare(parm_name[0], parm_name[1], "ssrc") == 0) {
        int ssrc = 0;

        if (!advance_integer(p, last, & ssrc, 10))
            return false;

        if (tran)
            tran->ssrc = ssrc;
    } else if (casecompare(parm_name[0], parm_name[1], "mode") == 0) {
        bool quoted = false;

        if (p == last)
            return false;

        if (*p == '"') {
            ++p;
            quoted = true;
        }

        do {
            method_enum method;

            if (*p == '"') {
                if (!quoted)
                    return false;
                else
                    break;
            }

            if (!advance_method(p, last, & method))
                return false;

            if (tran)
                tran->methods.push_back(method);

            if (p != last && *p == ',')
                ++p;
            else
                break;

        } while (p != last);

        if (quoted) {
            if (p != last && *p == '"') {
                ++p;
                quoted = false;
            }
        }

        // Unbalanced qoutes
        if (quoted)
            return false;

    } else {
        // Bad parameter
        return false;
    }

    return proto::compare_and_assign(pos, p);
}

template <typename ForwardIterator, typename OutputIterator>
bool advance_transport (ForwardIterator & pos
        , ForwardIterator last
        , OutputIterator * out = 0)
{
    ForwardIterator p = pos;

    do {
        transport_entry<ForwardIterator> tran;

        // Initialize transport entry
        tran.protocol.first
                = tran.protocol.second
                = tran.profile.first
                = tran.profile.second
                = tran.destination.first
                = tran.destination.second
                = last;
        tran.lower_transport = RTSP_LOWER_TRANSPORT_UDP;
        tran.interleaved.first = tran.interleaved.second = 0;
        tran.append = false;
        tran.ttl = 0;
        tran.layers = 0;
        tran.port.first = tran.port.second = 0;
        tran.client_port.first = tran.client_port.second = 0;
        tran.server_port.first = tran.server_port.second = 0;
        tran.ssrc = 0;

        ForwardIterator last_pos = p;

        // Transport protocol
        if (!proto::advance_token(p, last))
            return false;

        if (casecompare(last_pos, p, "RTP") != 0)
            return false;

        tran.protocol.first = last_pos;
        tran.protocol.second = p;

        if (p == last)
            return false;

        if (*p != '/')
            return false;

        last_pos = ++p;

        // Transport profile
        if (!advance_token(p, last))
            return false;

        if (casecompare(last_pos, p, "AVP") != 0)
            return false;

        tran.profile.first = last_pos;
        tran.profile.second = p;

        if (p != last) {
            if (*p == '/') {
                last_pos = ++p;

                // Transport lower transport
                if (!advance_token(p, last))
                    return false;

                if (casecompare(last_pos, p, "UDP") == 0)
                    tran.lower_transport = RTSP_LOWER_TRANSPORT_UDP;
                else if (casecompare(last_pos, p, "TCP") == 0)
                    tran.lower_transport = RTSP_LOWER_TRANSPORT_TCP;
                else
                    return false;
            }

            if (p != last) {
                // Transport parameters
                while (*p == ';') {
                    if (!advance_transport_parameter(++p, last, & tran))
                        return false;
                }
            }
        }

//         if (tran.lower_transport != RTSP_LOWER_TRANSPORT_UDP)
//             tran.multicast = false;

        if (out)
            *(*out)++ = tran;

        if (p != last && *p == ',') {
            ++p;
            continue;
        } else {
            break;
        }
    } while (pos != last);

    return proto::compare_and_assign(pos, p);
}

template <typename ForwardIterator, typename OutputIterator>
inline bool advance_transport (ForwardIterator & pos
        , ForwardIterator last
        , OutputIterator out)
{
    return advance_transport(pos, last, & out);
}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
template <typename StringT>
struct transport
{
    typedef StringT string_type;
    typedef std::vector<method_enum> method_container;

    string_type protocol;
    string_type profile;
    lower_transport_enum lower_transport;
    string_type destination;
    range<int> interleaved;
    bool append;
    int ttl;
    int layers;
    range<int> port;
    range<int> client_port;
    range<int> server_port;
    int ssrc;
    method_container methods;
};

template <typename StringT>
class transport_container
{
    typedef StringT string_type;
    typedef transport<string_type> transport_type;
    typedef std::vector<transport_type> internal_container;

public:
    typedef typename internal_container::iterator iterator;
    typedef typename internal_container::const_iterator const_iterator;
    typedef typename internal_container::reference reference;
    typedef typename internal_container::const_reference const_reference;

private:
    internal_container _transports;

public:
    transport_container () {}
    ~transport_container () {}

    size_t size () const
    {
        return _transports.size();
    }

    reference at (int pos)
    {
        return _transports[pos];
    }

    const_reference at (int pos) const
    {
        return _transports[pos];
    }

#if __cplusplus < 201103L
    reference emplace_back ()
    {
        if (_transports.capacity() == _transports.size())
            _transports.reserve(_transports.size() + 2) ;
        _transports.resize(_transports.size() + 1);
        return _transports.back();
    }
#elif __cplusplus < 201703L
    reference emplace_back ()
    {
        _transports.emplace_back();
        return _transports.back();
    }
#else
    reference emplace_back ()
    {
        return _transports.emplace_back();
    }
#endif

    /**
     * Returns an iterator to the first element of the container.
     * If the container is empty, the returned iterator will be equal to end().
     */
    iterator begin () { return _transports.begin(); }

    /**
     * Returns an iterator to the element following the last element
     * of the container.
     */
    iterator end () { return _transports.end(); }

    /**
     * Returns a const iterator to the first element of the container.
     * If the container is empty, the returned iterator will be equal to end().
     */
    const_iterator begin () const { return _transports.begin(); }

    /**
     * Returns an iterator to the element following the last element
     * of the container.
     */
    const_iterator end () const { return _transports.end(); }

    /**
     * Returns a const iterator to the first element of the container.
     * If the container is empty, the returned iterator will be equal to end().
     */
    const_iterator cbegin () const { return _transports.begin(); }

    /**
     * Returns an iterator to the element following the last element
     * of the container.
     */
    const_iterator cend () const { return _transports.end(); }
};

template <typename StringT>
class message : public proto::message<StringT>
{
    typedef proto::message<StringT> base_class;

public:
    typedef StringT string_type;
    typedef string_type key_type;    // for property_tree_traits
    typedef string_type mapped_type; // for property_tree_traits
    typedef transport<string_type> transport_type;
    typedef transport_container<string_type> transport_container_type;
    typedef property<string_type, string_type> string_property;
    typedef property<int, string_type> int_property;
    typedef property<transport_container_type, string_type> transports_property;

public:
    /**
     * RFC-2326
     * 12.17 CSeq
     *--------------------------------------------------------------------------
     * The CSeq field specifies the sequence number for an RTSP request-response
     * pair. This field MUST be present in all requests and responses. For every
     * RTSP request containing the given sequence number, there will be a
     * corresponding response having the same number. Any retransmitted request
     * must contain the same sequence number as the original (i.e. the sequence
     * number is not incremented for retransmissions of the same request).
     */
    int_property sequence_number;

    /**
     * RFC-2326
     * 3.4 Session Identifiers
     *--------------------------------------------------------------------------
     * Session identifiers are opaque strings of arbitrary length. Linear
     * white space must be URL-escaped. A session identifier MUST be chosen
     * randomly and MUST be at least eight octets long to make guessing it
     * more difficult.
     */
    string_property session_id;

    /*
     * RFC-2326
     * 12.39 Transport
     *--------------------------------------------------------------------------
     * Transports are comma separated, listed in order of preference.
     * Parameters may be added to each transport, separated by a semicolon.
     */
    transports_property transports;

public:
    message ()
        : base_class()
        , sequence_number("CSeq")
        , session_id("Session")
    {}

    bool insert (string_type const & key, string_type const & value);
};

template <typename StringT>
bool message<StringT>::insert (StringT const & key, StringT const & value)
{
    typedef typename string_traits<StringT>::const_iterator const_iterator;

    bool ok = true;

    if (key == "cseq") {
        sequence_number = to_integer<int>(begin(value), end(value), 10, & ok);
        if (!ok) return false;
    } else if (key == "session") {
        session_id = value;
    } else if (key == "transport") {
        typedef transport_entry<const_iterator> transport_entry;

        const_iterator pos = begin(value);
        std::vector<transport_entry> transport_entries;

        if (!advance_transport(pos, end(value), std::back_inserter(transport_entries)))
            return false;

        for (size_t i = 0, count = transport_entries.size(); i < count; i++) {
            transport_type & tran = transports->emplace_back();
            transport_entry & te = transport_entries[i];

            tran.protocol = string_type(te.protocol.first, te.protocol.second);
            tran.profile = string_type(te.profile.first, te.profile.second);
            tran.lower_transport = te.lower_transport;
            tran.destination = string_type(te.destination.first, te.destination.second);
            tran.interleaved = std::make_pair(te.interleaved.first, te.interleaved.second);
            tran.append = te.append;
            tran.ttl = te.ttl;
            tran.layers = te.layers;
            tran.port = std::make_pair(te.port.first, te.port.second);
            tran.client_port = std::make_pair(te.client_port.first, te.client_port.second);
            tran.server_port = std::make_pair(te.server_port.first, te.server_port.second);
            tran.ssrc = te.ssrc;
            tran.methods = te.methods;
        }
    } else {
        base_class::insert(key, value);
    }

    return true;
}

}}} // namespace pfs::net::rtsp

namespace pfs {
namespace net {
namespace proto {

template <>
inline bool inserter<rtsp::message<std::string> >::insert (std::string const & key
        , std::string const & value)
{
    if (_p)
        return _p->insert(key, value);
    return true;
}

}}} // pfs::net::proto

