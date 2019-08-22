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

char const * to_cstring (method_enum method)
{
    switch (method) {
        case RTSP_METHOD_DESCRIBE:      return "DESCRIBE";
        case RTSP_METHOD_ANNOUNCE:      return "ANNOUNCE";
        case RTSP_METHOD_GET_PARAMETER: return "GET_PARAMETER";
        case RTSP_METHOD_OPTIONS:       return "OPTIONS";
        case RTSP_METHOD_PAUSE:         return "PAUSE";
        case RTSP_METHOD_PLAY:          return "PLAY";
        case RTSP_METHOD_RECORD:        return "RECORD";
        case RTSP_METHOD_REDIRECT:      return "REDIRECT";
        case RTSP_METHOD_SETUP:         return "SETUP";
        case RTSP_METHOD_SET_PARAMETER: return "SET_PARAMETER";
        case RTSP_METHOD_TEARDOWN:      return "TEARDOWN";
        default: break;
    }

    return "";
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
    bool multicast;
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
        if (tran) tran->multicast = false;
    } else if (casecompare(parm_name[0], parm_name[1], "multicast") == 0) {
        if (tran) tran->multicast = true;
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
        tran.multicast = true; // default value
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

        if (tran.lower_transport != RTSP_LOWER_TRANSPORT_UDP)
            tran.multicast = false;

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
};

template <typename StringT>
class headers
{
public:
    typedef StringT string_type;
    typedef std::vector<method_enum> method_container;

    struct transport
    {
        string_type protocol;
        string_type profile;
        lower_transport_enum lower_transport;
        string_type destination;
        bool multicast;
        std::pair<int,int> interleaved;
        bool append;
        int ttl;
        int layers;
        std::pair<int,int> port;
        std::pair<int,int> client_port;
        std::pair<int,int> server_port;
        int ssrc;
        method_container methods;
    };

    typedef property<string_type, string_type> string_property;
    typedef property<int, string_type> int_property;

public:
    string_property method;
    string_property uri;
    string_property protocol;
    string_property version;

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

    string_property session_id;
//    transport_property transport;

private:
    std::map<string_type, string_type> _all_headers;
    std::vector<transport> _transports;

public:
    headers ()
        : sequence_number("CSeq")
        , session_id("Session")
    {}

    string_type & operator [] (string_type const & key)
    {
        return _all_headers[key];
    }

    int transport_count () const
    {
        return _transports.size();
    }

    transport * transport_at (int index)
    {
        return index >= 0 && index < _transports.size()
                ? & _transports[index]
                : static_cast<transport *>(0);
    }

    transport const * transport_at (int index) const
    {
        return index >= 0 && index < _transports.size()
                ? & _transports[index]
                : static_cast<transport const *>(0);
    }

    bool parse (typename string_traits<string_type>::const_iterator first
            , typename string_traits<string_type>::const_iterator last)
    {
        return advance_headers(first, last, this);
    }

    bool insert (string_type const & key, string_type const & value);
};

template <typename StringT>
bool headers<StringT>::insert (StringT const & key, StringT const & value)
{
    bool ok = true;

    if (key == "#method") {
        method = value;
    } else if (key == "#uri") {
        uri = value;
    } else if (key == "#proto") {
        protocol = value;
    } else if (key == "#version") {
        version = value;
    } else if (key == "cseq") {
        sequence_number = to_integer<int>(begin(value), end(value), 10, & ok);
        if (!ok) return false;
    } else if (key == "session") {
        session_id = value;
    } else if (key == "transport") {
        typename string_traits<StringT>::const_iterator pos = begin(value);
        std::vector<transport_entry<typename
                string_traits<StringT>::const_iterator> > transports;

        if (!advance_transport(pos, end(value), std::back_inserter(transports)))
            return false;

        for (size_t i = 0; i < transports.size(); i++) {
            _transports.push_back(transport());
            transport & tran = _transports.back();

            tran.protocol = string_type(transports[i].protocol.first
                    , transports[i].protocol.second);
            tran.profile = string_type(transports[i].profile.first
                    , transports[i].profile.second);
            tran.lower_transport = transports[i].lower_transport;
            tran.destination = string_type(transports[i].destination.first
                    , transports[i].destination.second);
            tran.multicast = transports[i].multicast;
            tran.interleaved = std::make_pair(transports[i].interleaved.first
                    , transports[i].interleaved.second);
            tran.append = transports[i].append;
            tran.ttl = transports[i].ttl;
            tran.layers = transports[i].layers;
            tran.port = std::make_pair(transports[i].port.first
                    , transports[i].port.second);
            tran.client_port = std::make_pair(transports[i].client_port.first
                    , transports[i].client_port.second);
            tran.server_port = std::make_pair(transports[i].server_port.first
                    , transports[i].server_port.second);
            tran.ssrc = transports[i].ssrc;
            tran.methods = transports[i].methods;
        }
    }

    _all_headers[key] = value;

    return true;
}

}}} // namespace pfs::net::rtsp

namespace pfs {
namespace net {
namespace proto {

template <>
struct property_tree_traits<rtsp::headers<std::string> >
{
    typedef std::string key_type;
    typedef std::string mapped_type;
    //typedef typename PropertyTree::iterator iterator;
    //typedef typename PropertyTree::const_iterator const_iterator;
};

template <>
bool inserter<rtsp::headers<std::string> >::insert (std::string const & key
        , std::string const & value)
{
    if (_p)
        return _p->insert(key, value);
    return true;
}

}}} // pfs::net::proto

