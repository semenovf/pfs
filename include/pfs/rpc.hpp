#pragma once
#include <string>
#include <pfs/endian.hpp>
#include <pfs/memory.hpp>
#include <pfs/types.hpp>
#include <pfs/optional.hpp>
#include <pfs/v2/binary_ostream.hpp>
#include <pfs/v2/binary_istream.hpp>
#include <pfs/v2/byte_string.hpp>
#include <pfs/v2/safeformat.hpp>
#include <pfs/v2/string.hpp>
#include <pfs/v2/stdcxx/list.hpp>
#include <pfs/v2/stdcxx/map.hpp>

namespace pfs {

// struct protocol
// {
//     bool pack (byte_string & data);
//     bool unpack (byte_string & data);
// }
//
// struct transport
// {
//     ssize_t read (byte_string & data);
//     ssize_t write (byte_string const & data);
// }

template <uint8_t MajorVersion, uint8_t MinorVersion
        , typename Protocol
        , typename String                  = pfs::string
        , typename MethodName              = String
        , typename Id                      = int32_t
        , typename AssociativeContainerTag = stdcxx::map
        , typename SequenceContainerTag    = stdcxx::list
        , int Order                        = endian::network_endian>
struct rpc
{
    typedef uint16_t   version_type;
    typedef Protocol   protocol_type;
    typedef String     string_type;
    typedef MethodName method_name_type;
    typedef Id         id_type;
    typedef int32_t    error_code_type;

    typedef safeformat<string_type> fmt;

    static uint8_t const MAJOR_VERSION = MajorVersion;
    static uint8_t const MINOR_VERSION = MinorVersion;

    static uint8_t const RPC_METHOD       = 1;
    static uint8_t const RPC_NOTIFICATION = 2;
    static uint8_t const RPC_SUCCESS      = 3;
    static uint8_t const RPC_ERROR        = 4;

    //
    // Error codes
    //
    static error_code_type const NO_ERROR = 0;

    static error_code_type const BAD_VERSION = 1;

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

public:
#if __COMMENT__
    typedef shared_ptr<archive_type> shared_archive;

    static shared_archive make_none ()
    {
        return shared_response();
    }

    static shared_archive make_success (id_type const & id)
    {
        shared_archive r = pfs::make_shared<archive_type>();
        binary_ostream(*r, Order) bos;
        bos << MAJOR_VERSION << MINOR_VERSION << RPC_SUCCESS << id;
        return r;
    }

    static shared_archive make_error (id_type const & id, error_code_type ec)
    {
        shared_archive r = pfs::make_shared<archive_type>();
        binary_ostream(*r, Order) bos;
        bos << MAJOR_VERSION << MINOR_VERSION << RPC_ERROR << id << ec;
        return r;
    }

    static shared_archive make_error (error_code_type ec)
    {
        shared_archive r = pfs::make_shared<archive_type>();
        binary_ostream(*r, Order) bos;
        bos << MAJOR_VERSION << MINOR_VERSION << RPC_ERROR << ec;
        return r;
    }
#endif

#if __COMMENT__
protected:
    struct basic_binder
    {
        //virtual shared_response call (byte_istream &) = 0;
        virtual shared_response call (id_type, byte_istream &) = 0;
    };

    template <typename F>
    struct function_binder : basic_binder
    {
        F _f;
        function_binder (F f) : _f(f) {}

//         virtual shared_response call (byte_istream & params) pfs_override
//         {
//             return (*_f)(params);
//         }

        virtual shared_archive call (id_type id, byte_istream & params) pfs_override
        {
            return (*_f)(id, params);
        }
    };

    template <typename F, typename C>
    struct method_binder : basic_binder
    {
        F _f;
        C & _c;
        method_binder (F f, C & c) : _f(f), _c(c) {}

//         virtual shared_response call (byte_istream & params) pfs_override
//         {
//             return (_c.*_f)(params);
//         }

        virtual shared_archive call (id_type id, byte_istream & params) pfs_override
        {
            return (_c.*_f)(id, params);
        }
    };

    typedef shared_ptr<basic_binder> shared_binder;
#endif

public:

    ///////////////////////////////////////////////////////////////////////////
    // Server                                                                //
    ///////////////////////////////////////////////////////////////////////////

    template <typename Transport>
    class server
    {
    public:
        typedef Transport transport_type;

    private:
        typedef typename rpc::string_type string_type;
        typedef typename rpc::id_type     id_type;

//         struct repository_traits
//         {
//             typedef typename associative_container::type_traits<
//                       method_name_type
//                     , shared_binder
//                     , AssociativeContainerTag>::type type;
//
//             typedef associative_container::iterators<
//                       method_name_type
//                     , shared_binder
//                     , type> iterators;
//
//             typedef associative_container::inserter<
//                       method_name_type
//                     , shared_binder
//                     , type> inserter;
//
//             typedef associative_container::finder<
//                       method_name_type
//                     , shared_binder
//                     , type> finder;
//
//             typedef typename iterators::iterator iterator;
//             typedef typename iterators::const_iterator const_iterator;
//         };

    public:
        server (protocol_type & protocol, transport_type & transport)
            : _protocol(protocol)
            , _transport(transport)
        {}

#if __COMMENT__
        template <typename F>
        inline void bind (method_name_type const & method_name, F f)
        {
              typename repository_traits::inserter(_method_repo).insert(method_name
                      , pfs::static_pointer_cast<basic_binder>(pfs::make_shared<function_binder<F>, F>(f)));
        }

        template <typename F, typename C>
        inline void bind (method_name_type const & method_name, F f, C & c)
        {
            typename repository_traits::inserter(_method_repo).insert(method_name
                    , pfs::static_pointer_cast<basic_binder>(pfs::make_shared<method_binder<F, C>, F, C>(f, c)));
        }

        shared_response exec (request & rq)
        {
            typename repository_traits::finder finder(_method_repo);
            typename repository_traits::iterators iterators(_method_repo);

            uint8_t major_version;
            uint8_t minor_version;
            uint8_t rpc_type;
            id_type id;
            method_name_type method_name;

            rq >> major_version >> minor_version >> rpc_type;

            if (!(major_version == MAJOR_VERSION && minor_version == MINOR_VERSION))
                return make_error(BAD_VERSION);

            if (rpc_type == RPC_METHOD)
                rq >> id;
            else if (rpc_type == RPC_NOTIFICATION) {
                ;
            } else {
                return make_error();
            }

            rq >> method_name;

            typename repository_traits::const_iterator it = finder.find(method_name);

            if (it == iterators.cend()) {
                if (rpc_type == RPC_METHOD)
                    return make_error(id, METHOD_NOT_FOUND);
                return make_error(METHOD_NOT_FOUND);
            }

            shared_binder const & m = iterators.value(it);
            return m->call(rq);
        }
#endif

    private:
        protocol_type  & _protocol;
        transport_type & _transport;
        //typename repository_traits::type _method_repo;
    };

    ///////////////////////////////////////////////////////////////////////////
    // Client                                                                //
    ///////////////////////////////////////////////////////////////////////////

    template <typename Transport>
    class client
    {
        typedef typename rpc::string_type string_type;
        typedef typename rpc::id_type     id_type;

#if __COMMENT__
        struct calls_traits
        {
             typedef typename sequence_container::type_traits<
                       shared_binder
                     , SequenceContainerTag>::type type;

             typedef sequence_container::iterators<
                       shared_binder
                     , type> iterators;

             typedef typename iterators::iterator iterator;
             typedef typename iterators::const_iterator const_iterator;
        };

        struct calls_mapper_traits
        {
             typedef typename associative_container::type_traits<
                       id_type
                     , typename calls_traits::iterator
                     , AssociativeContainerTag>::type type;

            typedef associative_container::iterators<
                      id_type
                    , typename calls_traits::iterator
                    , type> iterators;

            typedef associative_container::inserter<
                      id_type
                    , typename calls_traits::iterator
                    , type> inserter;

            typedef associative_container::finder<
                      id_type
                    , typename calls_traits::iterator
                    , type> finder;

            typedef typename iterators::iterator       iterator;
            typedef typename iterators::const_iterator const_iterator;
        };
#endif

    public:
        typedef Transport transport_type;
        typedef typename Protocol::response_type response_type;

    public:
        client (protocol_type & protocol, transport_type & transport)
            : _protocol(protocol)
            , _transport(transport)
        {}

        client & call (method_name_type const & method_name)
        {
            _protocol.begin();
            _protocol << MAJOR_VERSION << MINOR_VERSION << RPC_SUCCESS;// << id;
            _protocol <<
            _protocol << method_name;
            return *this;
        }

        void result (response_type & response)
        {
            _protocol.commit();

            // TODO

            // Send data via transport

            // Wait for response
        }

        void notify ()
        {
            _protocol.commit();

            // TODO

            // Send data via transport
        }

        template <typename T>
        inline client & operator () (T const & x)
        {
            _protocol << x;
        }

#if __COMMENT__
        shared_request make_request (id_type const & id, method_name_type const & method_name)
        {
            shared_request r = pfs::make_shared<byte_ostream>();
            *r << MAJOR_VERSION << MINOR_VERSION << RPC_METHOD << id << method_name;
            return r;
        }

        shared_request make_notification (method_name_type const & method_name)
        {
            shared_request r = pfs::make_shared<byte_ostream>();
            *r << MAJOR_VERSION << MINOR_VERSION << RPC_NOTIFICATION << method_name;
            return r;
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
  //      bool process (response const & rp);

        void call ();
#endif
    protected:
//         typedef typename Traits::template associative_container<id_type
//                 , result_handler_type>::type result_map_type;
//
//         typedef typename Traits::template associative_container<int
//                 , error_handler_type>::type  error_map_type;
//
//         error_handler_type _default_error_handler;
//
#if __COMMENT__
         typename calls_traits::type        _calls;
         typename calls_mapper_traits::type _cache_mapper;
#endif
// //         result_map_type _result_handlers;
// //         error_map_type  _error_handlers;
    private:
        protocol_type  & _protocol;
        transport_type & _transport;
    };
};

} // pfs

