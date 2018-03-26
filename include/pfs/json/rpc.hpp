#ifndef __PFS_JSON_RPC_HPP__
#define __PFS_JSON_RPC_HPP__

#include <pfs/traits/associative_container.hpp>

// [JSON-RPC 2.0 Specification](http://www.jsonrpc.org/specification)

namespace pfs {
namespace json {
namespace rpc {

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

template <typename JsonType
    , typename StringType
    , typename IdType
    , template <typename> class AssociativeContainerImplType>
struct traits
{
    typedef JsonType   json_type;
    typedef StringType string_type;
    typedef IdType     id_type;

    template <typename Key, typename Value>
    struct associative_container
    {
        typedef pfs::traits::associative_container<
              pfs::traits::kv<Key, Value>
            , AssociativeContainerImplType> type;

        typedef Key key_type;
        typedef Value value_type;
    };
};

inline char const * jsonrpc () { return "2.0"; }

//
// Request object
// -----------------------------------------------------------------------------
// A rpc call is represented by sending a Request object to a Server.
// The Request object has the following members:
//
// jsonrpc [REQUIRED]
//      A String specifying the version of the JSON-RPC protocol.
//      MUST be exactly "2.0".
//
// method [REQUIRED]
//      A String containing the name of the method to be invoked.
//      Method names that begin with the word rpc followed by a period
//      character (U+002E or ASCII 46) are reserved for rpc-internal methods
//      and extensions and MUST NOT be used for anything else.
//
// params [OPTIONAL]
//      A Structured value that holds the parameter values to be used during
//      the invocation of the method. This member MAY be omitted.
//
// id [REQUIRED]
//      An identifier established by the Client that MUST contain a String,
//      Number, or NULL value if included. If it is not included it is assumed
//      to be a notification. The value SHOULD normally not be Null and
//      Numbers SHOULD NOT contain fractional parts.
//

template <typename Traits>
inline typename Traits::id_type id (typename Traits::json_type const & d)
{
    return d["id"].template get<typename Traits::id_type>();
}

template <typename Traits>
inline bool is_request (typename Traits::json_type const & d)
{
    return d.contains("method") && d.contains("id");
}

template <typename Traits>
typename Traits::json_type make_request (typename Traits::id_type const & id
        , char const * method
        , typename Traits::json_type const & params = typename Traits::json_type())
{
    typedef typename Traits::json_type json_type;

    json_type r = json_type::make_object();
    r["jsonrpc"] = jsonrpc();
    r["id"] = id;
    r["method"] = method;

    if (!params.is_null())
        r["params"] = params;

    return r;
}

//
// Notification
//------------------------------------------------------------------------------
// A Notification is a Request object without an "id" member. A Request object
// that is a Notification signifies the Client's lack of interest in the
// corresponding Response object, and as such no Response object needs to be
// returned to the client. The Server MUST NOT reply to a Notification,
// including those that are within a batch request.
//

template <typename Traits>
inline bool is_notification (typename Traits::json_type const & d)
{
    return d.contains("method") && !d.contains("id");
}

template <typename Traits>
typename Traits::json_type make_notification (char const * method
        , typename Traits::json_type const & params = typename Traits::json_type())
{
    typedef typename Traits::json_type json_type;

    json_type r = json_type::make_object();
    r["jsonrpc"] = jsonrpc();
    r["method"] = method;

    if (!params.is_null())
        r["params"] = params;

    return r;
}

//
// Response object
// -----------------------------------------------------------------------------
// When a rpc call is made, the Server MUST reply with a Response, except for
// in the case of Notifications. The Response is expressed as a single
// JSON Object, with the following members:
//
// jsonrpc [REQUIRED]
//      A String specifying the version of the JSON-RPC protocol.
//      MUST be exactly "2.0".
//
// result [REQUIRED on success]
//      This member is REQUIRED on success.
//      This member MUST NOT exist if there was an error invoking the method.
//      The value of this member is determined by the method invoked on the Server.
//
// error [REQUIRED on error]
//    This member is REQUIRED on error.
//    This member MUST NOT exist if there was no error triggered during invocation.
//    The value for this member MUST be an Object as defined below.
//
// id [REQUIRED]
//      This member is REQUIRED.
//      It MUST be the same as the value of the id member in the Request Object.
//      If there was an error in detecting the id in the Request object
//      (e.g. Parse error/Invalid Request), it MUST be Null.
//

template <typename Traits>
inline bool is_response (typename Traits::json_type const & d)
{
    return d.contains("id") && (d.contains("result") || d.contains("error"));
}

template <typename Traits>
inline bool is_success (typename Traits::json_type const & d)
{
    return d.contains("id") && d.contains("result");
}

template <typename Traits>
typename Traits::json_type make_success (typename Traits::id_type const & id
        , typename Traits::json_type const & result = typename Traits::json_type())
{
    typedef typename Traits::json_type json_type;

    json_type r = json_type::make_object();
    r["jsonrpc"] = jsonrpc();
    r["id"] = id;
    r["result"] = result;
    return r;
}


//
// Error object
//------------------------------------------------------------------------------
// When a rpc call encounters an error, the Response Object MUST contain the
// error member with a value that is a Object with the following members:
//
// code [REQUIRED]
//      A Number that indicates the error type that occurred.
//      This MUST be an integer.
//
// message [OPTIONAL]
//      A String providing a short description of the error.
//      The message SHOULD be limited to a concise single sentence.
//
// data [OPTIONAL]
//      A Primitive or Structured value that contains additional information
//      about the error.
//      This may be omitted.
//      The value of this member is defined by the Server (e.g. detailed error
//      information, nested errors etc.).
//

template <typename Traits>
inline bool is_error (typename Traits::json_type const & d)
{
    return /*d.contains("id") && */d.contains("error");
}

//
// Used if e.g. Parse error/Invalid Request
//
template <typename Traits>
typename Traits::json_type make_error (int code
        , typename Traits::string_type const & message = typename Traits::string_type()
        , typename Traits::json_type data = typename Traits::json_type())
{
    typedef typename Traits::json_type json_type;

    json_type r = json_type::make_object();
    r["jsonrpc"] = jsonrpc();
    r["error"]["code"] = code;

    if (!message.empty())
        r["error"]["message"] = message;

    if (!data.is_null())
        r["error"]["data"] = data;

    return r;
}

template <typename Traits>
typename Traits::json_type make_error (typename Traits::id_type const & id
        , int code
        , typename Traits::string_type const & message = typename Traits::string_type()
        , typename Traits::json_type data = typename Traits::json_type())
{
    typedef typename Traits::json_type json_type;

    json_type r = make_error<Traits>(code, message, data);
    r["id"] = id;
    return r;
}

template <typename Traits>
inline int code (typename Traits::json_type const & error)
{
    return error["error"]["code"].template get<int>();
}

template <typename Traits, typename Handler>
struct server
{
    typedef typename Traits::json_type   json_type;
    typedef typename Traits::string_type string_type;
    typedef typename Traits::id_type     id_type;
    typedef void (Handler::*method_handler_type) (json_type const & request, json_type & response);
    typedef void (Handler::*notification_handler_type) (json_type const & notification);

    server (Handler & handler)
        : _handler(handler)
    {}

    void register_method (char const * name, method_handler_type mh)
    {
        _methods.insert(string_type(name), mh);
    }

    void register_notification (char const * name, notification_handler_type nh)
    {
        _notifications.insert(string_type(name), nh);
    }

    void exec (json_type const & request, json_type & response) pfs_noexcept;

protected:
    typedef typename Traits::template associative_container<string_type
            , method_handler_type>::type       method_map_type;

    typedef typename Traits::template associative_container<string_type
            , notification_handler_type>::type notification_map_type;

    Handler &             _handler;
    method_map_type       _methods;
    notification_map_type _notifications;
};

template <typename Traits, typename Handler>
void server<Traits, Handler>::exec (json_type const & request, json_type & response) pfs_noexcept
{
    response.clear();

    if (is_request<Traits>(request)) {
        typename method_map_type::const_iterator it = _methods.find(request["method"].get_string());

        if (it != _methods.cend()) {
            method_handler_type method = method_map_type::mapped_reference(it);
            (_handler.*method)(request, response);
        } else {
            response = make_error<Traits>(request["id"].template get<id_type>(), METHOD_NOT_FOUND);
        }
    } else if (is_notification<Traits>(request)) {
        typename notification_map_type::const_iterator it = _notifications.find(request["method"].get_string());

        if (it != _notifications.cend()) {
            notification_handler_type method = notification_map_type::mapped_reference(it);
            (_handler.*method)(request);
        } else {
            response = make_error<Traits>(METHOD_NOT_FOUND);
        }
    } else {
        response = make_error<Traits>(INVALID_REQUEST);
    }
}

template <typename Traits, typename Handler>
struct client
{
    typedef typename Traits::json_type json_type;
    typedef typename Traits::string_type string_type;
    typedef typename Traits::id_type id_type;
    typedef void (Handler::*result_handler_type) (json_type const & request);
    typedef void (Handler::*error_handler_type) (json_type const & error);

    client (Handler & handler)
        : _handler(handler)
        , _default_error_handler(0) {}

    void register_result_handler (id_type id, result_handler_type h)
    {
        _result_handlers.insert(id, h);
    }

    void register_error_handler (int code, error_handler_type h)
    {
        _error_handlers.insert(code, h);
    }

    void set_default_error_handler (error_handler_type h)
    {
        _default_error_handler = h;
    }

    bool handle (json_type const & response);

protected:
    typedef typename Traits::template associative_container<id_type
            , result_handler_type>::type result_map_type;

    typedef typename Traits::template associative_container<int
            , error_handler_type>::type  error_map_type;

    error_handler_type _default_error_handler;

    Handler &       _handler;
    result_map_type _result_handlers;
    error_map_type  _error_handlers;
};

template <typename Traits, typename Handler>
bool client<Traits, Handler>::handle (json_type const & response)
{
    if (is_success<Traits>(response)) {
        typename result_map_type::const_iterator it
                = _result_handlers.find(response["id"].template get<id_type>());

        if (it != _result_handlers.cend()) {
            (_handler.*result_map_type::mapped_reference(it))(response);
            return true;
        }
    } else if (is_error<Traits>(response)) {
        typename error_map_type::const_iterator it
                = _error_handlers.find(code<Traits>(response));

        if (it != _error_handlers.cend()) {
            (_handler.*error_map_type::mapped_reference(it))(response);
        } else {
            (_handler.*_default_error_handler)(response);
        }

        return true;
    }

    return false;
}

}}} // pfs::json::rpc

#endif /* __PFS_JSON_RPC_HPP__ */

