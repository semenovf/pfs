#pragma once

#include <string>
#include <sstream>
#include <map>
#include <pfs/memory.hpp>
#include <pfs/types.hpp>
#include <pfs/safeformat.hpp>
#include <pfs/optional.hpp>
#include <pfs/v2/traits/associative_container.hpp>

namespace pfs {

//
// Based on [JSON-RPC 2.0 Specification](http://www.jsonrpc.org/specification)
//

// AssociativeContainer requirements:
//     insert(KEY, VALUE)

template <int MajorVersion, int MinorVersion
        , typename String        = std::string
        , typename MethodType    = String
        , typename IdType        = int32_t
        , typename ErrorCodeType = int32_t
        , template <typename Key
                , typename T>
          class AssociativeContainer = stdcxx::map
        , typename OStreamType   = std::ostringstream
        , typename IStreamType   = std::istringstream>
struct rpc
{
    typedef uint16_t      version_type;
    typedef String        string_type;
    typedef MethodType    method_type;
    typedef IdType        id_type;
    typedef ErrorCodeType error_code_type;
    typedef OStreamType   ostream_type;
    typedef IStreamType   istream_type;

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

    inline string_type version_s () const
    {
        return fmt("%d.%d")
                % MAJOR_VERSION
                % MINOR_VERSION;
    }

    inline version_type version () const
    {
        return MAJOR_VERSION * 100 + MINOR_VERSION;
    }

    struct params {};

    class basic_request
    {
    protected:
        version_type     _version;
        method_type      _method;
        optional<params> _params;
    };

    class request : public basic_request
    {
    protected:
        id_type _id;
    };

    class notification : public basic_request
    {
    };

    class basic_response
    {

    };

    class success : public basic_response
    {
    public:
        virtual ~success () {}
    };

    class error : public basic_response
    {
        virtual ~error () {}
    };

    static shared_ptr<request> make_request ();

    static shared_ptr<notification> make_notification ();

    static shared_ptr<success> make_success ();

    static shared_ptr<error> make_error ();

    template <typename Handler>
    class server
    {
        typedef typename rpc::string_type string_type;
        typedef typename rpc::id_type     id_type;
        typedef shared_ptr<basic_response> (Handler::*method_handler) (request const & requ);
        typedef void (Handler::*notification_handler) (notification const & notifi);

        typedef typename AssociativeContainer<method_type, method_handler>::type method_repository;
        typedef typename AssociativeContainer<method_type, notification_handler>::type notification_repository;

    public:
        server (Handler & handler)
            : _ph(& handler)
        {}

        void register_method (method_type const & name, method_handler mh)
        {
            insert(_method_repo, name, mh);
        }

        void register_notification (method_type const & name, notification_handler nh)
        {
            insert(_notification_repo, name, nh);
        }

//         void exec (json_type const & request, json_type & response) pfs_noexcept;
//
//         protected:
//         typedef typename Traits::template associative_container<string_type
//                 , method_handler>::type       method_map_type;
//
//         typedef typename Traits::template associative_container<string_type
//                 , notification_handler>::type notification_map_type;

    private:
        Handler *               _ph;
        method_repository       _method_repo;
        notification_repository _notification_repo;
    };

    template <typename Handler>
    struct client
    {
        typedef typename rpc::string_type string_type;
        typedef typename rpc::id_type     id_type;
        typedef void (Handler::*success_handler) (success const & response);
        typedef void (Handler::*error_handler) (error const & response);

        client (Handler & handler)
             : _ph(& handler)
//             , _default_error_handler(0) {}
        {}
//         void register_result_handler (id_type id, result_handler_type h)
//         {
//             _result_handlers.insert(id, h);
//         }
//
//         void register_error_handler (int code, error_handler_type h)
//         {
//             _error_handlers.insert(code, h);
//         }
//
//         void set_default_error_handler (error_handler_type h)
//         {
//             _default_error_handler = h;
//         }
//
//         bool handle (json_type const & response);

    protected:
//         typedef typename Traits::template associative_container<id_type
//                 , result_handler_type>::type result_map_type;
//
//         typedef typename Traits::template associative_container<int
//                 , error_handler_type>::type  error_map_type;
//
//         error_handler_type _default_error_handler;
//
        Handler *       _ph;
//         result_map_type _result_handlers;
//         error_map_type  _error_handlers;
    };
};

} // pfs

