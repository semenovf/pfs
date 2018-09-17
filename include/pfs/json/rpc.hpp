#pragma once
#include <pfs/endian.hpp>
#include <pfs/system_error.hpp>
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

struct id_generator
{
    typedef int32_t type;

    id_generator () : id(0) {}

    type next ()
    {
        return ++id;
    }

private:
    type id;
};

template <typename Json, int Optimization = UBJSON_FULL_OPTIMIZED>
struct ubjson_serializer
{
    typedef Json json_type;

    pfs::byte_string operator () (json_type const & value) const
    {
        pfs::byte_string result = to_ubjson(value, Optimization);
        return result;
    }

    pfs::error_code operator () (pfs::byte_string const & bytes, json_type & result) const
    {
        pfs::error_code ec;
        result = pfs::json::from_ubjson<json_type>(bytes, ec);
        return ec;
    }

    pfs::error_code operator () (pfs::byte_string::const_iterator first
            , pfs::byte_string::const_iterator last
            , json_type & result) const
    {
        pfs::error_code ec;
        result = pfs::json::from_ubjson<json_type>(first, last, ec);
        return ec;
    }
};

template <typename Json
        , typename Method = typename Json::string_type
        , typename IdGenerator = id_generator
        , typename Serializer = ubjson_serializer<Json>
        , int Major = 2
        , int Minor = 0>
struct rpc
{
    typedef Json                                json_type;
    typedef Method                              method_type;
    typedef IdGenerator                         id_generator_type;
    typedef Serializer                          serializer_type;
    typedef typename IdGenerator::type          id_type;
    typedef typename json_type::difference_type difference_type;
    typedef typename json_type::size_type       size_type;
    typedef typename json_type::string_type     string_type;
    typedef typename json_type::boolean_type    boolean_type;
    typedef typename json_type::integer_type    integer_type;
    typedef typename json_type::real_type       real_type;
    typedef typename json_type::uinteger_type   uinteger_type;

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

    static bool parse_version (string_type const & version_str
            , int & major
            , int & minor)
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
        } catch (bad_lexical_cast const &) {
            ;
        }

        return false;
    }

    class entity
    {
    protected:
        json_type _j;

        struct init_tag {};

    protected:
        entity () {}

        entity (init_tag)
        {
            _j["jsonrpc"] = pfs::to_string(Major) + '.' + pfs::to_string(Minor);
        }

    public:
        bool version_match (int major_sample, int minor_sample) const
        {
            int major = !major_sample, minor = !minor_sample;
            string_type version_str = pfs::json::cref(this->_j)["jsonrpc"].get_string();

            if (parse_version(version_str, major, minor)
                    && major == major_sample
                    && minor == minor_sample)
                return true;

            return false;
        }

        bool empty () const
        {
            return _j.empty();
        }

        void clear ()
        {
            return _j.clear();
        }

#if PFS_TEST
    public: json_type const & internal_data () const { return _j; }
#endif
    };

    class request : public entity
    {
    protected:
        request (method_type const & name)
            : entity(typename entity::init_tag())
        {
            this->_j["method"] = name;
        }

    public:
        request () : entity() {}

        method_type name () const
        {
            method_type result = pfs::json::cref(this->_j)["method"].template get<method_type>();
            return result;
        }

        method_type method () const
        {
            return this->name();
        }

        id_type id () const
        {
            return this->_j["id"].template get<id_type>();
        }

        bool has_params () const
        {
            return this->_j.contains("params");
        }

        /**
         * @throw json_exception if parameter by @a index not found.
         */
        template <typename T>
        T get (int index) const
        {
            return this->_j["params"][index].template get<T>();
        }

        /**
         * @throw json_exception if parameter by @a name not found.
         */
        template <typename T>
        T get (string_type const & name) const
        {
            return this->_j["params"][name].template get<T>();
        }

        template <typename T>
        T get (int index, T const & default_value) const pfs_noexcept
        {
            return pfs::json::cref(this->_j)["params"][index].template get<T>(default_value);
        }

        template <typename T>
        T get (string_type const & name, T const & default_value) const pfs_noexcept
        {
            return pfs::json::cref(this->_j)["params"][name].template get<T>(default_value);
        }

        bool is_method () const
        {
            return this->_j.contains("id");
        }

        bool is_notification () const
        {
            return !is_method();
        }

        template <typename T>
        request & arg (T const & value)
        {
            this->_j["params"].push_back(value);
            return *this;
        }

        template <typename T>
        request & arg (string_type const & name, T const & value)
        {
            this->_j["params"][name] = value;
            return *this;
        }

        template <typename T>
        request & operator () (T const & value)
        {
            this->_j["params"].push_back(value);
            return *this;
        }

        template <typename T>
        request & operator () (string_type const & name, T const & value)
        {
            this->_j["params"][name] = value;
            return *this;
        }

    public: // static
        static request make_notification (method_type const & name)
        {
            request r(name);
            return r;
        }

        static request make_method (IdGenerator & idgen, method_type const & name)
        {
            request r(name);
            r._j["id"] = idgen.next();
            return r;
        }

        static void make_notification (request & r, method_type const & name)
        {
            if (r._j.contains("id"))
                r._j.erase("id");
            r._j["method"] = name;
            return r;
        }

        static void make_method (request & r, IdGenerator & idgen, method_type const & name)
        {
            if (!r._j.contains("id"))
                r._j["id"] = idgen.next();
            r._j["method"] = name;
        }

        static pfs::byte_string serialize (request const & rq)
        {
            serializer_type ar;
            pfs::byte_string r = ar(rq._j);
            return r;
        }

        static pfs::error_code deserialize (pfs::byte_string const & bytes
                , request & rq)
        {
            serializer_type ar;
            pfs::error_code ec = ar(bytes, rq._j);
            return ec;
        }

        static pfs::error_code deserialize (pfs::byte_string::const_iterator first
                , pfs::byte_string::const_iterator last
                , request & rq)
        {
            serializer_type ar;
            pfs::error_code ec = ar(first, last, rq._j);
            return ec;
        }
    };

    class response : public entity
    {
    protected:
        response (id_type const & id)
            : entity(typename entity::init_tag())
        {
            this->_j["id"] = id;
        }

    public:
        response () : entity() {}

        id_type id () const
        {
            return this->_j["id"].template get<id_type>();
        }

        bool is_success () const
        {
            return this->_j.contains("result");
        }

        bool is_failure () const
        {
            return ! is_success();
        }

        ///////////////////////////////////////////////////////////////////////
        // Methods applicable to success                                     //
        ///////////////////////////////////////////////////////////////////////

        // Applicable to success
        bool has_result () const
        {
            return this->_j.contains("result");
        }

        json_type const & result () const
        {
            return this->_j["result"];
        }

        /**
         * @return Result value.
         * @throw json_exception if result value not found.
         */
        template <typename T>
        T get () const
        {
            return this->_j["result"].template get<T>();
        }

        ///////////////////////////////////////////////////////////////////////
        // Methods applicable to failure                                     //
        ///////////////////////////////////////////////////////////////////////

        /**
         * @return Integer number that indicates the error type that occurred.
         * @throw json_exception if code value not found.
         */
        int code () const
        {
            return this->_j["error"]["code"].template get<int>();
        }

        /**
         * @return String providing a short description of the error.
         * @note The message should be limited to a concise single sentence.
         */
        string_type message () const pfs_noexcept
        {
            return pfs::json::cref(this->_j)["error"]["message"].get_string();
        }

        bool has_error_data () const
        {
            return this->_j.contains("error")
                    && this->_j["error"].contains("data");
        }

        /**
         * @return Primitive or structured value that contains
         *         additional information (e.g. detailed error
         *         information, nested errors etc.) about the error.
         */
        template <typename T>
        T error_data () const
        {
            return this->_j["error"]["data"].template get<T>();
        }

    public: // static
        template <typename T>
        static response make_success (request const & rq, T const & result)
        {
            response r(rq.id());
            r._j["result"] = result;
            return r;
        }

        static response make_failure (int code, request const & rq)
        {
            response r(rq.id());
            r._j["error"]["code"] = code;
            return r;
        }

        static response make_failure (int code
                , string_type const & message
                , request const & rq)
        {
            response r(rq.id());
            r._j["error"]["code"] = code;
            r._j["error"]["message"] = message;
            return r;
        }

        template <typename T>
        static response make_failure (int code
                , string_type const & msg
                , T const & data
                , request const & rq)
        {
            response r(rq.id());
            r._j["error"]["code"] = code;
            r._j["error"]["message"] = msg;
            r._j["error"]["data"] = data;
            return r;
        }

        static pfs::byte_string serialize (response const & rp)
        {
            serializer_type ar;
            pfs::byte_string r = ar(rp._j);
            return r;
        }

        static pfs::error_code deserialize (pfs::byte_string const & bytes
                , response & rp)
        {
            serializer_type ar;
            pfs::error_code ec = ar(bytes, rp._j);
            return ec;
        }

        static pfs::error_code deserialize (pfs::byte_string::const_iterator first
                , pfs::byte_string::const_iterator last
                , response & rp)
        {
            serializer_type ar;
            pfs::error_code ec = ar(first, last, rp._j);
            return ec;
        }
    };
};

}} // namespace pfs::json
