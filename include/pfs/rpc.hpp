#pragma once

#include <string>
#include <iostream>
#include <map>
#include <pfs/memory.hpp>
#include <pfs/types.hpp>
#include <pfs/safeformat.hpp>
#include <pfs/optional.hpp>
#include <pfs/v2/stdcxx/list.hpp>
#include <pfs/v2/stdcxx/map.hpp>

namespace pfs {

//
// Based on [JSON-RPC 2.0 Specification](http://www.jsonrpc.org/specification)
//

// AssociativeContainer requirements:
//     insert(KEY, VALUE)
//     find(KEY)

template <int MajorVersion, int MinorVersion
        , typename String        = std::string
        , typename MethodType    = String
        , typename IdType        = int32_t
        , typename AssociativeContainerTag = stdcxx::map
        , typename SequenceContainerTag = stdcxx::list
        , typename OStreamType   = std::ostream
        , typename IStreamType   = std::istream>
struct rpc
{
    typedef uint16_t      version_type;
    typedef String        string_type;
    typedef MethodType    method_type;
    typedef IdType        id_type;
    typedef int32_t       error_code_type;
    typedef OStreamType   ostream_type;
    typedef IStreamType   istream_type;

    typedef safeformat<string_type> fmt;

    //
    // Error codes
    //
    static error_code_type const NO_ERROR = 0;

    // Invalid JSON was received by the server.
    // An error occurred on the server while parsing
    // the JSON text.
    static error_code_type const PARSE_ERROR = -32700;

    // The JSON sent is not a valid Request object.
    static error_code_type const INVALID_REQUEST = -32600;

    // The method does not exist / is not available.
    static error_code_type const METHOD_NOT_FOUND = -32601;

    // Invalid method parameter(s).
    static error_code_type const INVALID_PARAMS = -32602;

    // Internal JSON-RPC error.
    static error_code_type const INTERNAL_ERROR = -32603;

    // -32000 to -32099 Reserved for implementation-defined server-errors.
    static error_code_type const SERVER_ERROR = -32000;

    static error_code_type const MAJOR_VERSION = MajorVersion;
    static error_code_type const MINOR_VERSION = MinorVersion;

    static inline string_type version_s ()
    {
        return fmt("%d.%d")
                % MAJOR_VERSION
                % MINOR_VERSION;
    }

    static inline version_type version ()
    {
        return MAJOR_VERSION * 100 + MINOR_VERSION;
    }

//     struct basic_params {};
//     struct basic_result {};
//     struct basic_error_data {};

private:
    struct entity
    {
        version_type      _version;
        optional<id_type> _id;

        entity () : _version(version()) {}
        entity (id_type const & id) : _version(version()), _id(id) {}

        inline id_type const & id () const
        {
            return *_id;
        }
    };

public:
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
    class request : public entity
    {
    protected:
        method_type _method;
//        optional<basic_params> _params;

    public:
        // Construct notification
        request (method_type const & method)
            : entity()
            , _method(method)
        {}

        // Construct request
        request (id_type const & id, method_type const & method)
            : entity(id)
            , _method(method)
        {}

        inline bool is_request () const pfs_noexcept
        {
            return this->_id.has_value();
        }

        inline bool is_notification () const pfs_noexcept
        {
            return !this->_id.has_value();
        }

        inline method_type const & method () const pfs_noexcept
        {
            return _method;
        }
    };

    //
    // Response object
    // -----------------------------------------------------------------------------
    // When a rpc call is made, the Server MUST reply with a Response, except for
    // in the case of Notifications. The Response is expressed as a single
    // Object, with the following members:
    //
    // version [REQUIRED]
    //      A String specifying the version of the RPC protocol.
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
    class response : public entity
    {
        error_code_type       _ec;
        optional<string_type> _msg;
        // optional<error_data>  _data;

    public:
        response () : entity(), _ec(NO_ERROR) {}
        response (id_type const & id, error_code_type ec = NO_ERROR)
            : entity(id)
            , _ec(ec)
        {}

        bool is_success () const pfs_noexcept
        {
            return _ec == NO_ERROR;
        }

        bool is_error () const pfs_noexcept
        {
            return _ec != NO_ERROR;
        }

        error_code_type code () const pfs_noexcept
        {
            return _ec;
        }

        void set_code (error_code_type ec)
        {
            _ec = ec;
        }

        void set_message (string_type const & m)
        {
            _msg = m;
        }
    };

    typedef shared_ptr<request>  shared_request;
    typedef shared_ptr<response> shared_response;

    static shared_ptr<response> null_response ()
    {
        return shared_response();
    }

    static shared_response make_success (id_type const & id)
    {
        return pfs::make_shared<response>(id);
    }

    static shared_response make_error (error_code_type code)
    {
        shared_response r = pfs::make_shared<response>();
        r->set_code(code);
        return r;
    }

    static shared_response make_error (id_type const & id, error_code_type code)
    {
        return pfs::make_shared<response>(id, code);
    }

    template <typename HandlerPool>
    class server
    {
        typedef typename rpc::string_type string_type;
        typedef typename rpc::id_type     id_type;
        typedef shared_response (HandlerPool::*method_handler) (request const & requ);

        struct method_repository : public associative_container::type_traits<
                                          method_type
                                        , method_handler
                                        , AssociativeContainerTag>::type
        {
            typedef associative_container::iterators<
                          method_type
                        , method_handler
                        , AssociativeContainerTag> iterators;
            typedef associative_container::inserter<
                          method_type
                        , method_handler
                        , AssociativeContainerTag> inserter;
            typedef associative_container::finder<
                          method_type
                        , method_handler
                        , AssociativeContainerTag> finder;

            typedef typename iterators::iterator       iterator;
            typedef typename iterators::const_iterator const_iterator;
        };

    public:
        server (HandlerPool & hpool)
            : _ph(& hpool)
        {}

        inline void register_method (method_type const & name, method_handler mh)
        {
            typename method_repository::inserter ins(_method_repo);
            ins.insert(name, mh);
        }

        inline shared_response exec (shared_request const & requ)
        {
            return this->exec(*requ);
        }

        shared_response exec (request const & requ)
        {
            typename method_repository::finder finder(_method_repo);
            typename method_repository::iterators iterators(_method_repo);
            typename method_repository::const_iterator it = finder.find(requ.method());

            if (it == iterators.cend()) {
                if (requ.is_request())
                    return make_error(requ.id(), METHOD_NOT_FOUND);
                return make_error(METHOD_NOT_FOUND);
            }

            method_handler const & method = iterators.value(it);
            return (_ph->*method)(requ);
        }

    private:
        HandlerPool *     _ph;
        method_repository _method_repo;
    };

    template <typename HandlerPool>
    class client
    {
        typedef typename rpc::string_type string_type;
        typedef typename rpc::id_type     id_type;
        typedef void (HandlerPool::*handler) (response const & rp);

        struct method_cache : public sequence_container::type_traits<
                                  handler
                                , SequenceContainerTag>::type
        {
            typedef sequence_container::iterators<
                          handler
                        , SequenceContainerTag> iterators;
//             typedef sequence_container::inserter<
//                           handler
//                         , SequenceContainerTag> inserter;
//             typedef sequence_container::finder<
//                           handler
//                         , SequenceContainerTag> finder;

            typedef typename iterators::iterator       iterator;
            typedef typename iterators::const_iterator const_iterator;
        };

        struct method_cache_mapper : public associative_container::type_traits<
                                          id_type
                                        , typename method_cache::iterator
                                        , AssociativeContainerTag>::type
        {
            typedef associative_container::iterators<
                          id_type
                        , typename method_cache::iterator
                        , AssociativeContainerTag> iterators;
            typedef associative_container::inserter<
                          id_type
                        , typename method_cache::iterator
                        , AssociativeContainerTag> inserter;
            typedef associative_container::finder<
                          id_type
                        , typename method_cache::iterator
                        , AssociativeContainerTag> finder;

            typedef typename iterators::iterator       iterator;
            typedef typename iterators::const_iterator const_iterator;
        };

    public:
        client (HandlerPool & hpool)
             : _ph(& hpool)
        {}

        shared_request make_request (id_type const & id, method_type const & method)
        {
            shared_request rq = pfs::make_shared<request>(id, method);

            //_cache.insert

            return rq;
        }

        static shared_request make_notification (method_type const & method)
        {
            return pfs::make_shared<request>(method);
        }

//         void register_success_handler (id_type const & id, success_handler sh)
//         {
//             _result_handlers.insert(id, h);
//         }
//
//         void register_error_handler (error_code_type ec, error_handler eh)
//         {
//             _error_handlers.insert(code, h);
//         }

//         void set_default_error_handler (error_handler_type h)
//         {
//             _default_error_handler = h;
//         }
//
        bool process (response const & rp);

    protected:
//         typedef typename Traits::template associative_container<id_type
//                 , result_handler_type>::type result_map_type;
//
//         typedef typename Traits::template associative_container<int
//                 , error_handler_type>::type  error_map_type;
//
//         error_handler_type _default_error_handler;
//
        HandlerPool *       _ph;
        method_cache        _cache;
        method_cache_mapper _cache_mapper;
//         result_map_type _result_handlers;
//         error_map_type  _error_handlers;
    };
};

} // pfs

