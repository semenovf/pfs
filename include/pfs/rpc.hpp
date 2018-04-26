#pragma once
#include <string>
#include <pfs/endian.hpp>
#include <pfs/memory.hpp>
#include <pfs/types.hpp>
#include <pfs/byte_string_ostream.hpp>
#include <pfs/byte_string_istream.hpp>
#include <pfs/byte_string.hpp>
#include <pfs/string.hpp>
#include <pfs/list.hpp>
#include <pfs/map.hpp>
#include <pfs/system_error.hpp>

namespace pfs {

template <uint8_t MajorVersion, uint8_t MinorVersion
        , int Order = endian::network_endian
        , typename Id = int32_t
        , typename StringT = pfs::string
        , template <typename, typename> class AssociativeContainer = pfs::map
        , template <typename> class SequenceContainer = pfs::list>
struct rpc
{
    //typedef uint16_t    version_type;
    typedef StringT string_type;
    typedef Id      id_type;
    typedef int32_t error_code_type;

    static uint8_t const RPC_METHOD ()       { return 1; }
    static uint8_t const RPC_NOTIFICATION () { return 2; }
    static uint8_t const RPC_SUCCESS ()      { return 3; }
    static uint8_t const RPC_ERROR ()        { return 4; }

    //
    // Error codes
    //
    static error_code_type const NO_ERROR () { return 0; }

    static error_code_type const BAD_VERSION () { return 1; }

    // Invalid JSON was received by the server.
    // An error occurred on the server while parsing
    // the JSON text.
    static error_code_type const PARSE_ERROR () { return -32700; }

    // The JSON sent is not a valid Request object.
    static error_code_type const INVALID_REQUEST () { return -32600; }

    // The method does not exist / is not available.
    static error_code_type const METHOD_NOT_FOUND () { return -32601; }

    // Invalid method parameter(s).
    static error_code_type const INVALID_PARAMS () { return -32602; }

    // Internal JSON-RPC error.
    static error_code_type const INTERNAL_ERROR () { return -32603; }

    // -32000 to -32099 Reserved for implementation-defined server-errors.
    static error_code_type const SERVER_ERROR () { return -32000; }

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

protected:
//
//     template <typename F, typename C>
//     struct method_binder : basic_binder
//     {
//         F _f;
//         C & _c;
//         method_binder (F f, C & c) : _f(f), _c(c) {}
//
// //         virtual shared_response call (byte_istream & params) pfs_override
// //         {
// //             return (_c.*_f)(params);
// //         }
//
//         virtual shared_archive call (id_type id, byte_istream & params) pfs_override
//         {
//             return (_c.*_f)(id, params);
//         }
//     };
//
//     typedef shared_ptr<basic_binder> shared_binder;

public:
    ///////////////////////////////////////////////////////////////////////////
    // Protocol                                                              //
    ///////////////////////////////////////////////////////////////////////////

    template <typename Derived>
    struct protocol
    {
//         typedef StringT string_type;
//         typedef Id      id_type;
//
//         protocol ()
//             : _is(_input_buffer)
//             , _os(_output_buffer)
//         {}
//
//         struct manip {};
//
//         // Output manipulators
//         virtual manip major_version (uint8_t value) = 0;
//         virtual manip minor_version (uint8_t value) = 0;
//         virtual manip rpc_entity (uint8_t value) = 0;
//         virtual manip method_name (string_type const & value) = 0;
//         virtual manip id (id_type const & value) = 0;
//
//         // Input manipulators
//         virtual manip get_major_version (uint8_t & value) = 0;
//         virtual manip get_minor_version (uint8_t & value) = 0;
//         virtual manip get_rpc_entity (uint8_t & value) = 0;
//         virtual manip get_method_name (string_type & value) = 0;
//         virtual manip get_id (id_type & value) = 0;
//
//         template <typename T>
//         manip param (T const & x);
//
//         template <typename T>
//         manip get_param (T & x);
//
//         template <typename T>
//         manip param (string_type const & param_name, T const & x);
//
//         template <typename T>
//         manip get_param (string_type & param_name, T & x);
//
//         virtual bool begin_tx () = 0;
//         virtual bool commit_tx () = 0;
//         virtual bool begin_rx () = 0;
//         virtual bool commit_rx () = 0;
//
//         pfs::byte_string const & data () const
//         {
//             return _output_buffer;
//         }
//
//         pfs::byte_string & buffer ()
//         {
//             return _input_buffer;
//         }
//
//     protected:
//         pfs::byte_string _input_buffer;
//         pfs::byte_string _output_buffer;
//         pfs::byte_string_istream _is;
//         pfs::byte_string_ostream _os;
    };

    ///////////////////////////////////////////////////////////////////////////
    // Transport                                                             //
    ///////////////////////////////////////////////////////////////////////////

    struct transport
    {
        transport () {}
        virtual ssize_t send (pfs::byte_string const &, pfs::error_code & ec) = 0;
        virtual ssize_t recv (pfs::byte_string &, pfs::error_code & ec) = 0;
    };

    ///////////////////////////////////////////////////////////////////////////
    // Server                                                                //
    ///////////////////////////////////////////////////////////////////////////

    template <typename Protocol, typename Transport>
    class server
    {
    public:
        typedef Transport transport_type;

    private:
//         struct basic_binder
//         {
//             virtual bool call (session &) = 0;
//         };

        typedef typename rpc::string_type string_type;
        typedef typename rpc::id_type     id_type;
//        typedef AssociativeContainer<string_type, pfs::shared_ptr<basic_binder> > repository;


//         template <typename F>
//         struct binder : basic_binder
//         {
//             F _f;
//             function_binder (F f) : _f(f) {}
//
//             virtual bool call (session & sess) pfs_override
//             {
//
//             }

    // //         virtual shared_response call (byte_istream & params) pfs_override
    // //         {
    // //             return (*_f)(params);
    // //         }
    //
    //         virtual shared_archive call (id_type id, byte_istream & params) pfs_override
    //         {
    //             return (*_f)(id, params);
    //         }
//        };


    public:
        server () {}

        void exec () {}

//         template <typename F>
//         inline void bind (string_type const & method_name, F f)
//         {
//               typename _repo(method_name
//                       , pfs::static_pointer_cast<basic_binder>(pfs::make_shared<binder<F>, F>(f)));
//         }

//          template <typename F, typename A1>
//          inline void bind (string_type const & method_name, F f, C & c)
//          {
// //             typename repository_traits::inserter(_method_repo).insert(method_name
// //                     , pfs::static_pointer_cast<basic_binder>(pfs::make_shared<method_binder<F, C>, F, C>(f, c)));
//          }
#if __COMMENT__
//         shared_response exec (request & rq)
//         {
//             typename repository_traits::finder finder(_method_repo);
//             typename repository_traits::iterators iterators(_method_repo);
//
//             uint8_t major_version;
//             uint8_t minor_version;
//             uint8_t rpc_type;
//             id_type id;
//             method_name_type method_name;
//
//             rq >> major_version >> minor_version >> rpc_type;
//
//             if (!(major_version == MAJOR_VERSION && minor_version == MINOR_VERSION))
//                 return make_error(BAD_VERSION);
//
//             if (rpc_type == RPC_METHOD)
//                 rq >> id;
//             else if (rpc_type == RPC_NOTIFICATION) {
//                 ;
//             } else {
//                 return make_error();
//             }
//
//             rq >> method_name;
//
//             typename repository_traits::const_iterator it = finder.find(method_name);
//
//             if (it == iterators.cend()) {
//                 if (rpc_type == RPC_METHOD)
//                     return make_error(id, METHOD_NOT_FOUND);
//                 return make_error(METHOD_NOT_FOUND);
//             }
//
//             shared_binder const & m = iterators.value(it);
//             return m->call(rq);
//         }
#endif

    private:
        //repository _repo;
//         protocol_type  _protocol;
//         transport_type _transport;
        //typename repository_traits::type _method_repo;
    };

    struct default_id_generator
    {
        id_type next_id () const
        {
            static id_type id = 0;
            return ++id;
        }
    };

    ///////////////////////////////////////////////////////////////////////////
    // Client                                                                //
    ///////////////////////////////////////////////////////////////////////////

    template <typename Protocol
            , typename Transport
            , typename IdGenerator = default_id_generator>
    class client
    {
        typedef typename rpc::string_type    string_type;
        typedef typename rpc::id_type        id_type;
        typedef Protocol                     protocol_type;
        typedef Transport                    transport_type;

        struct session
        {
            session (id_type i) : _id(i) {}

            error_code const & errorcode () const
            {
                return _ec;
            }

            session & call (string_type const method_name)
            {
                _proto.begin_tx();
                _proto << _proto.major_version(MajorVersion)
                        << _proto.minor_version(MinorVersion)
                        << _proto.rpc_entity(RPC_METHOD())
                        << _proto.id(_id)
                        << _proto.method_name(method_name);
                return *this;
            }

            template <typename T>
            inline session & operator () (T const & x)
            {
                _proto << _proto.param(x);
                return *this;
            }

            template <typename T>
            inline session & operator () (string_type const & param_name, T const & x)
            {
                _proto << _proto.param(param_name, x);
                return *this;
            }

#if __cplusplus >= 201103L
            template <typename T, typename ...Args>
            inline session & operator () (T const & x, Args const &... args)
            {
                _proto << x;
                return operator () (args...);
            }
#endif

            template <typename T>
            T result ()
            {
                T x;

                if (begin_result()) {
                    _proto >> x;
                }

                if (commit_result())
                    ;

                return x;
            }

        private:
            bool begin_result ()
            {
                _ec.clear();
                _proto.commit_tx();

                // Send data via transport
                ssize_t n = _transport.send(_proto.data(), _ec);

                if (n < 0 || _ec)
                    return false;

                // Receive data from server
                byte_string buffer;
                n = _transport.recv(buffer, _ec);

                if (n > 0 && !_ec)
                    _proto.buffer().append(buffer);

                if (!_proto.begin_rx())
                    return false;

                uint8_t major
                        , minor
                        , status;
                id_type id;

                _proto >> _proto.get_major_version(major)
                        >> _proto.get_minor_version(minor)
                        >> _proto.get_rpc_entity(status)
                        >> _proto.get_id(id);

                if (major != MajorVersion || minor != MinorVersion) {
                    // TODO set error code

                    return false;
                }

                if (status != RPC_SUCCESS()) {
                    // TODO set error code

                    return false;
                }

                if (id != _id) {
                    // TODO set error code

                    return false;
                }

                return true;
            }

            bool commit_result ()
            {
                return _proto.commit_rx();
            }

        protected:
            id_type    _id;
            Protocol   _proto;
            Transport  _transport;
            error_code _ec;
        };

        typedef AssociativeContainer<id_type, session> session_registry;

    public:
        client () {}

        session & call (string const & method_name)
        {
            id_type id = IdGenerator().next_id();
            std::pair<typename session_registry::iterator,bool> r = _sessions.insert(id, session(id));
            PFS_ASSERT(r.second);

            session & sess = session_registry::mapped_reference(r.first);
            return sess.call(method_name);
        }

//         client & notify (string const & method_name)
//         {
//             _protocol.begin_tx();
//             _protocol << RPC_NOTIFICATION();// << id;
//             _protocol << method_name;
//             return *this;
//         }
//
//         // for notify
//         bool send ()
//         {
//             _ec.clear();
//             _protocol.commit_tx();
//             ssize_t n = _transport.send(_protocol.data(), _ec);
//
//             if (n > 0 && !_ec)
//                 return false;
//             return true;
//         }

    private:
        IdGenerator      _id_generator;
        session_registry _sessions;
    };
};

} // pfs

