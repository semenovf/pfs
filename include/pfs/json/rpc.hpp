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
namespace rpc {

template <typename Id>
struct id_generator;

template <>
struct id_generator<int32_t> : pfs::id_generator<int32_t>
{
    id_generator () {}

    type next_id () const
    {
        static type id = 0;
        return ++id;
    }
};

template <typename JsonT
        , typename Id
        , int Optimization = UBJSON_FULL_OPTIMIZED
        , int Order = endian::network_endian>
struct ubjson_serializer
{
    typedef JsonT json_type;
    typedef Id id_type;
    typedef typename json_type::string_type string_type;

    ubjson_serializer () {}

    ubjson_serializer & set_version (uint8_t major, uint8_t minor)
    {
        _j["jsonrpc"] = pfs::to_string(major) + '.' + pfs::to_string(minor);
        return *this;
    }

    ubjson_serializer & set_entity (rpc_entity value)
    {
        // Nothing to do.
        // Entity recognized by `method`, `id`, `result` and `error` fields existance.
        // See protocol desription above.
        return *this;
    }

    ubjson_serializer & set_method (string_type const & value)
    {
        _j["method"] = value;
        return *this;
    }

    ubjson_serializer & set_id (id_type const & value)
    {
        _j["id"] = value;
        return *this;
    }

    template <typename T>
    ubjson_serializer & add_param (T & value)
    {
        _j["params"].push_back(value);
        return *this;
    }

    template <typename T>
    ubjson_serializer & add_param (string_type const & name, T & value)
    {
        _j["params"][name] = value;
        return *this;
    }

    byte_string pack () const
    {
        return to_ubjson(_j, Optimization);
    }

    bool unpack (byte_string const & data, error_code & ec)
    {
        _j = from_ubjson<json_type>(data, ec);
        return ec ? false : true;
    }

    bool get_version (uint8_t & major, uint8_t & minor) const
    {
        pfs::mpl::stringlist<string_type> slist;
        slist.split(pfs::json::cref(_j)["jsonrpc"].get_string(), ".");

        if (slist.size() != 2)
            return false;

        try {
            typename pfs::mpl::stringlist<string_type>::const_iterator first = slist.cbegin();
            major = pfs::lexical_cast<uint8_t>(*first++);
            minor = pfs::lexical_cast<uint8_t>(*first);
        } catch (...) {
            return false;
        }

        return true;
    }

    bool get_entity (int & value) const
    {
        if (_j.contains("method")) {
            if (_j.contains("id"))
                value = RPC_METHOD;
            else
                value = RPC_NOTIFICATION;
        } else if (_j.contains("result")) {
            value = RPC_SUCCESS;
        } else if (_j.contains("error")) {
            value = RPC_ERROR;
        } else {
            return false;
        }

        return true;
    }

    bool get_id (id_type & value) const
    {
        value = pfs::json::cref(_j)["id"].template get<id_type>();
        return value == id_type() ? false : true;
    }

    template <typename T>
    bool get_value (T & value) const
    {
        if (_j.contains("result")) {
            value = _j["result"].template get<T>();
            return true;
        }

        return false;
    }

    bool get_value (json_type & value) const
    {
        if (_j.contains("result")) {
            value = _j["result"];
            return true;
        }

        return false;
    }

    template <typename T>
    bool get_param (T & value) const
    {
        if (_j.contains("params")) {
            value = _j["params"].template get<T>();
            return true;
        }

        return false;
    }

    bool get_param (json_type & value) const
    {
        if (_j.contains("params")) {
            value = _j["params"];
            return true;
        }

        return false;
    }

    bool has_params () const
    {
        return _j.contains("params");
    }

    template <typename T>
    void set_result (T const & result)
    {
        _j["result"] = result;
    }

    void set_result (void)
    {}

    string_type get_method () const
    {
        return _j["method"].get_string();
    }

private:
    json_type _j;
};

}}}
