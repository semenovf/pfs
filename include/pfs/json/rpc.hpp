#ifndef __PFS_JSON_RPC_HPP__
#define __PFS_JSON_RPC_HPP__

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

template <typename JsonType>
inline bool is_request (JsonType const & d)
{
    return d.contains("method") && d.contains("id");
}

template <typename JsonType, typename IdType>
JsonType make_request (IdType const & id, char const * method)
{
    JsonType r = JsonType::template make_object();
    r["jsonrpc"] = jsonrpc();
    r["id"] = id;
    r["method"] = method;
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

template <typename JsonType>
inline bool is_notification (JsonType const & d)
{
    return d.contains("method") && !d.contains("id");
}

template <typename JsonType>
JsonType make_notification (char const * method)
{
    JsonType r = JsonType::template make_object();
    r["jsonrpc"] = jsonrpc();
    r["method"] = method;
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

template <typename JsonType>
inline bool is_response (JsonType const & d)
{
    return d.contains("id") && (d.contains("result") || d.contains("error"));
}

template <typename JsonType>
inline bool is_success (JsonType const & d)
{
    return d.contains("id") && d.contains("result");
}

template <typename JsonType, typename IdType>
JsonType make_success (IdType const & id, JsonType const & result)
{
    JsonType r = JsonType::template make_object();
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

template <typename JsonType>
inline bool is_error (JsonType const & d)
{
    return d.contains("id") && d.contains("error");
}

template <typename JsonType, typename IdType>
JsonType make_error (IdType const & id, int code
        , typename JsonType::string_type const & message = JsonType::string_type()
        , JsonType data = JsonType())
{
    JsonType r = JsonType::template make_object();
    r["jsonrpc"] = jsonrpc();
    r["id"] = id;
    r["code"] = code;

    if (!message.empty())
        r["message"] = message;

    if (!data.is_null())
        r["data"] = data;

    return r;
}


// TODO Implement
template <typename JsonType>
struct server
{
    server () {}

    void register_method (char const * name, void (*) (JsonType const & request));
    void register_notification (char const * name, void (*) (JsonType const & notification));
};

// TODO Implement
template <typename JsonType>
struct client
{
    client () {}

    void register_result_handler (void (*) (JsonType const & result));
    void register_error_handler (int code, void (*) (JsonType const & error));
};

// TODO Implement (if need)
struct packet
{

};

#if __FIXME__


template <typename JsonType>
struct basic_entity
{
    typedef typename JsonType::string_type string_type;

    static string_type const & jsonrpc () { return "2.0"; }

    template <typename T>
    T id () const
    {
        return _d["id"].get<T>();
    }

    void set_id (string_type const & v)
    {
        _d["id"] = v;
    }

    void set_id (int v)
    {
        _d["id"] = v;
    }

    void set_id (long int v)
    {
        _d["id"] = v;
    }

protected:
    basic_entity ()
    {
        _d["jsonrpc"] = jsonrpc();
    }

protected:
    JsonType _d;
};

template <typename JsonType>
struct basic_request : basic_entity<JsonType>
{
    typedef basic_entity<JsonType>       base_class;
    typedef typename JsonType::size_type size_type;
    typedef typename JsonType::key_type  key_type;

    string_type method () const
    {
        return _d["method"].get<string_type>();
    }

    void set_method (string_type const & m)
    {
        _d["method"] = m;
    }

    template <typename T>
    void push_parameter (T const & v)
    {
        _d["params"].push_back(v);
    }

    template <typename T>
    void insert_parameter (string_type const & name, T const & v)
    {
        _d["params"][name] = v;
    }

    bool has_params () const
    {
        return _d.contains("params");
    }

    JsonType const & params () const
    {
        return _d["params"];
    }

protected:
    basic_request () : base_class() {}
};

template <typename JsonType>
struct request : basic_request<JsonType>
{
    typedef basic_request<JsonType> base_class;

    request () : base_class() {}
};

template <typename JsonType>
struct notification : basic_request<JsonType>
{
    typedef basic_request<JsonType> base_class;

    notification ()
        : base_class()
    {}
};



template <typename JsonType>
struct basic_response : basic_entity<JsonType>
{
    typedef basic_entity<JsonType> base_class;

    basic_response () : base_class() {}

    bool is_error () const
    {
        return _d.contains("error");
    }

    JsonType const & result () const
    {
        return _d["result"];
    }

    JsonType & result ()
    {
        return _d["result"];
    }

    JsonType const & error () const
    {
        return _d["error"];
    }

    JsonType & error ()
    {
        return _d["error"];
    }
};

template <typename JsonType>
struct response : basic_entity<JsonType>
{
    typedef basic_entity<JsonType> base_class;

    response () : base_class() {}

    bool is_error () const
    {
        return _d.contains("error");
    }

    JsonType const & result () const
    {
        return _d["result"];
    }

    JsonType & result ()
    {
        return _d["result"];
    }

    JsonType const & error () const
    {
        return _d["error"];
    }

    JsonType & error ()
    {
        return _d["error"];
    }
};


template <typename JsonType>
struct error
{
    typedef typename JsonType::string_type string_type;


    error (int c)
    {
        _d["code"] = c;
    }

    int code () const
    {
        return reference_wrapper(_d)["code"].get<int>(NO_ERROR);
    }

    string_type message () const
    {
        return reference_wrapper(_d)["message"].get<string_type>();
    }

    void set_message (string_type const & m)
    {
        _d["message"] = m;
    }

    bool has_data () const
    {
        return _d.contains("data");
    }

    JsonType const & data () const
    {
        return _d["data"];
    }

    void set_data (JsonType const & v)
    {
        _d["data"] = v;
    }

private:
    JsonType _d;
};
#endif

}}} // pfs::json::rpc

#endif /* __PFS_JSON_RPC_HPP__ */

