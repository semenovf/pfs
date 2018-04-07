#pragma once

#include <pfs/types.hpp>
#include <pfs/safeformat.hpp>

namespace pfs {

//
// Based on [JSON-RPC 2.0 Specification](http://www.jsonrpc.org/specification)
//

template <int MajorVersion, int MinorVersion, int ReleaseVersion
        , typename String
        , typename MethodType
        , typename IdType
        , typename OStreamType
        , typename IStreamType>
struct rpc
{
    typedef String      string_type;
    typedef MethodType  method_type;
    typedef IdType      id_type;
    typedef OStreamType ostream_type;
    typedef IStreamType istream_type;

    typedef safeformat<string_type> fmt;

    //
    // Error codes
    //
    static int const NO_ERROR = 0;

    // Invalid JSON was received by the server.
    // An error occurred on the server while parsing
    // the JSON text.
    static int const PARSE_ERROR = -32700;

    // The JSON sent is not a valid Request object.
    static int const INVALID_REQUEST = -32600;

    // The method does not exist / is not available.
    static int const METHOD_NOT_FOUND = -32601;

    // Invalid method parameter(s).
    static int const INVALID_PARAMS = -32602;

    // Internal JSON-RPC error.
    static int const INTERNAL_ERROR = -32603;

    // -32000 to -32099 Reserved for implementation-defined server-errors.
    static int const SERVER_ERROR = -32000;

    static int const MAJOR_VERSION   = MajorVersion;
    static int const MINOR_VERSION   = MinorVersion;
    static int const RELEASE_VERSION = ReleaseVersion;

    inline string_type version_s () const
    {
        return fmt("%d.%d.%d")
                % MAJOR_VERSION
                % MINOR_VERSION
                % RELEASE_VERSION;
    }

    inline uint32_t version () const
    {
        return MAJOR_VERSION * 10000000 + MINOR_VERSION * 100000 + RELEASE_VERSION;
    }

    class basic_request
    {
    protected:
        uint32_t    _version
        method_type _method;
    }
};

} // pfs

