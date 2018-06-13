#pragma once
#include <pfs/endian.hpp>
#include <pfs/stringlist.hpp>
#include <pfs/byte_string_ostream.hpp>
#include <pfs/byte_string_istream.hpp>
#include <pfs/rpc.hpp>
#include <pfs/json/json.hpp>
#include "pfs/json/ubjson_ostream.hpp"
#include "pfs/json/ubjson_istream.hpp"

//
// [JSON-RPC 2.0 Specification](http://www.jsonrpc.org/specification)
//
///////////////////////////////////////////////////////////////////////////////
// Request object                                                            //
///////////////////////////////////////////////////////////////////////////////
//
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
///////////////////////////////////////////////////////////////////////////////
// Notification                                                              //
///////////////////////////////////////////////////////////////////////////////
//
// A Notification is a Request object without an "id" member. A Request object
// that is a Notification signifies the Client's lack of interest in the
// corresponding Response object, and as such no Response object needs to be
// returned to the client. The Server MUST NOT reply to a Notification,
// including those that are within a batch request.
//
///////////////////////////////////////////////////////////////////////////////
// Response object                                                           //
///////////////////////////////////////////////////////////////////////////////
//
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
///////////////////////////////////////////////////////////////////////////////
// Error object                                                              //
///////////////////////////////////////////////////////////////////////////////
//
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

namespace pfs {
namespace json {

template <typename Json
        , typename Id
        , int Major = 2
        , int Minor = 0>
struct rpc 
{
    typedef Json json_type;
    typedef Id id_type;
    typedef typename json_type::string_type string_type;

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

//     template <typename Id>
//     struct id_generator;

// template <>
// struct id_generator<int32_t> : pfs::id_generator<int32_t>
// {
//     typedef int32_t type;
//     id_generator () {}
//
//     type next_id () const
//     {
//         static type id = 0;
//         return ++id;
//     }
// };
    
    bool parse_version (string_type const & version_str
            , int & major
            , int & minor) const
    {
        stringlist<string_type> slist;
        slist.split(version_str, ".");

        if (slist.size() != 2)
            return false;

        try {
            typename stringlist<string_type>::const_iterator first = slist.cbegin();
            major = pfs::lexical_cast<int>(*first++);
            minor = pfs::lexical_cast<int>(*first);

            return true;
        } catch (...) {
            ;
        }

        return false;
    }

    class entity
    {
    protected:
        json_type & _j;
        
        struct init_tag {};

    protected:
        entity (json_type & j) : _j(j)  {}
        
        entity (json_type & j, init_tag) : _j(j) 
        {
            _j["jsonrpc"] = pfs::to_string(Major) + '.' + pfs::to_string(Minor);
        }
    };

    class request : public entity
    {
    protected:
        request (json_type & j, string_type const & name) 
            : entity(j, typename entity::init_tag()) 
        {
            this->_j["method"] = name;
        }

    public:
        request (json_type & j) : entity(j) {}
        
        string_type method () const
        {
            string_type result = pfs::json::cref(this->_j)["method"].get_string();
            return result;
        }

//         template <typename T>
//         void add_param (T & value)
//         {
//             _j["params"].push_back(value);
//         }
// 
//         void add_param (string_type const & name, T & value)
//         {
//             _j["params"][name] = value;
//         }
// 
        bool has_params () const
        {
            return this->_j.contains("params");
        }

//         template <typename T>
//         T param (int index, T const & default_value) const
//         {
//             return pfs::json::cref(_j)[index].template get<T>(default_value);
//         }
// 
//         template <typename T>
//         T param (string_type const & name, T const & default_value) const
//         {
//             return pfs::json::cref(_j)[name].template get<T>(default_value);
//         }

        bool is_method () const
        {
            return this->_j.contains("id");
        }

        bool is_notification () const
        {
            return !is_method();
        }
    };

    class notification : public request
    {
    protected:
        notification (json_type & j, string_type const & name)
            : request(j, name)
        {}
    };

    class method : public request
    {
    public:
        method (json_type & j, id_type id, string_type const & name)
            : request(j, name)
        {
            this->_j["id"] = id;
        }
        
        id_type id () const
        {
            return this->_j["id"].template get<id_type>();
        }
    };

    class response : public entity
    {
    protected:
        response (json_type & j) : entity(j) {}
        
        response (json_type & j, id_type const & id)
            : entity(j) 
        {
            this->_j["id"] = id;
        }

    public:
        bool is_success () const
        {
            return this->_j.contains("result");
        }

        bool is_error () const
        {
            return ! is_success();
        }
    };

    class success : public response
    {
    protected:
        success (json_type & j, method const & m) 
            : response(j, m.id())
        {}

//     public:
//         template <typename T>
//         void set_result (T const & result)
//         {
//             _j["result"] = result;
//         }
// 
//         bool has_result () const
//         {
//             return _j.contains("result");
//         }
// 
//         // TODO Replace result type with optional
//         // T must have default constructor
//         template <typename T>
//         T result () const
//         {
//             return _j["result"].template get<T>();
//         }
    };

    class failure : public response
    {
    public:
        failure (json_type & j, int code) 
            : response(j) 
        {
            
        }
        
        failure (json_type & j, int code, method const & m)
            : response(j, m.id())
        {
            this->_j["error"]["code"] = code;
        }

    public:
//         int code () const
//         {
//             return _j["error"]["code"];
//         }
// 
//         void set_message (string_type const & text)
//         {
//             _j["error"]["message"] = text;
//         }
// 
//         string_type message () const
//         {
//             return pfs::json::cref(_j)["error"]["message"].get_string();
//         }
// 
//         template <typename T>
//         void set_data (T const & data)
//         {
//             _j["error"]["data"] = data;
//         }
// 
//         template <typename T>
//         T data () const
//         {
//             return _j["error"]["data"].template get<T>();
//         }
// 
//         bool has_data () const
//         {
//             return _j.contains("error") && _j["error"].contains("data").
//         }
    };

//template <typename Json
//         , typename Id
//        , int Optimization = UBJSON_FULL_OPTIMIZED
//         , int Order = endian::network_endian>
// class ubjson_serializer
// {
//     typedef Json json_type;
//     typedef Id id_type;
//     typedef typename json_type::string_type string_type;
//
//    json_type _j;
//
// private:
//     ubjson_serializer & set_version (uint8_t major, uint8_t minor)
//     {
//         _j["jsonrpc"] = pfs::to_string(major) + '.' + pfs::to_string(minor);
//         return *this;
//     }
//
// public:
//     ubjson_serializer ()
//     {
//         set_version(2,0);
//     }
//
//     ubjson_serializer & set_entity (rpc_entity value)
//     {
//         // Nothing to do.
//         // Entity recognized by `method`, `id`, `result` and `error` fields existance.
//         // See protocol desription above.
//         return *this;
//     }
//
//     ubjson_serializer & set_method (string_type const & value)
//     {
//         _j["method"] = value;
//         return *this;
//     }
//
//     ubjson_serializer & set_id (id_type const & value)
//     {
//         _j["id"] = value;
//         return *this;
//     }
//
//     template <typename T>
//     ubjson_serializer & add_param (T & value)
//     {
//         _j["params"].push_back(value);
//         return *this;
//     }
//
//     template <typename T>
//     ubjson_serializer & add_param (string_type const & name, T & value)
//     {
//         _j["params"][name] = value;
//         return *this;
//     }
//
//     byte_string pack () const
//     {
//         return to_ubjson(_j, Optimization);
//     }
//
//     template <typename T>
//     void set_result (T const & result)
//     {
//         _j["result"] = result;
//     }
//};

// template <typename Json
//         , typename Id
//         , int Major = 2
//         , int Minor = 0
//         , int Optimization = UBJSON_FULL_OPTIMIZED
//         , int Order = endian::network_endian>
// class ubjson_deserializer
// {
// private:
//     typedef Json json_type;
//     typedef Id id_type;
//     typedef typename json_type::string_type string_type;
//
//     json_type _j;
//
// public:
//     ubjson_deserializer () {}
//
//     bool unpack (byte_string const & data, error_code & ec)
//     {
//         _j = from_ubjson<json_type>(data, ec);
//         return ec ? false : true;
//     }
//
//     bool unpack (byte_string::const_iterator first
//             , byte_string::const_iterator last
//             , error_code & ec)
//     {
//         _j = from_ubjson<json_type>(first, last, ec);
//         return ec ? false : true;
//     }
//
//     bool get_version (uint8_t & major, uint8_t & minor) const
//     {
//         pfs::mpl::stringlist<string_type> slist;
//         slist.split(pfs::json::cref(_j)["jsonrpc"].get_string(), ".");
//
//         if (slist.size() != 2)
//             return false;
//
//         try {
//             typename pfs::mpl::stringlist<string_type>::const_iterator first = slist.cbegin();
//             major = pfs::lexical_cast<uint8_t>(*first++);
//             minor = pfs::lexical_cast<uint8_t>(*first);
//
//             if (major != Major)
//                 return false;
//
//             if (minor != Minor)
//                 return false;
//         } catch (...) {
//             return false;
//         }
//
//         return true;
//     }
//
//     bool get_entity (rpc_entity & value) const
//     {
//         if (_j.contains("method")) {
//             if (_j.contains("id"))
//                 value = RPC_METHOD;
//             else
//                 value = RPC_NOTIFICATION;
//         } else if (_j.contains("result")) {
//             value = RPC_SUCCESS;
//         } else if (_j.contains("error")) {
//             value = RPC_ERROR;
//         } else {
//             value = RPC_UNKNOWN;
//             return false;
//         }
//
//         return true;
//     }
//
//     bool get_id (id_type & value) const
//     {
//         value = pfs::json::cref(_j)["id"].template get<id_type>();
//         return value == id_type() ? false : true;
//     }
//
//     template <typename T>
//     bool get_value (T & value) const
//     {
//         if (_j.contains("result")) {
//             value = _j["result"].template get<T>();
//             return true;
//         }
//
//         return false;
//     }
//
//     bool get_value (json_type & value) const
//     {
//         if (_j.contains("result")) {
//             value = _j["result"];
//             return true;
//         }
//
//         return false;
//     }
//
//     template <typename T>
//     bool get_param (T & value) const
//     {
//         if (_j.contains("params")) {
//             value = _j["params"].template get<T>();
//             return true;
//         }
//
//         return false;
//     }
//
//     bool get_param (json_type & value) const
//     {
//         if (_j.contains("params")) {
//             value = _j["params"];
//             return true;
//         }
//
//         return false;
//     }
//
//     bool has_params () const
//     {
//         return _j.contains("params");
//     }
//
//     void set_result (void)
//     {}
//
//     string_type get_method () const
//     {
//         return _j["method"].get_string();
//     }
// };
};
    
}} // namespace pfs::json
