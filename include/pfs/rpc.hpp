#pragma once
#include <string>
#include <pfs/endian.hpp>
#include <pfs/memory.hpp>
#include <pfs/types.hpp>
// #include <pfs/byte_string_ostream.hpp>
// #include <pfs/byte_string_istream.hpp>
#include <pfs/byte_string.hpp>
#include <pfs/string.hpp>
#include <pfs/list.hpp>
#include <pfs/map.hpp>
#include <pfs/system_error.hpp>

namespace pfs {

enum rpc_entity {
      RPC_METHOD       = 1
    , RPC_NOTIFICATION = 2
    , RPC_SUCCESS      = 3
    , RPC_ERROR        = 4
};

///////////////////////////////////////////////////////////////////////////
// ID Generator                                                          //
///////////////////////////////////////////////////////////////////////////

template <typename Id>
struct id_generator
{
    typedef Id type;
    type next_id () const;
};

///////////////////////////////////////////////////////////////////////////
// Serializer                                                            //
///////////////////////////////////////////////////////////////////////////
//
// struct serializer
// {
//      serializer & version (uint8_t major, uint8_t minor);
//      serializer & entity (rpc_entity ent);
//      serializer & method (string_type const & name);
// };
//
///////////////////////////////////////////////////////////////////////////
// Protocol                                                              //
///////////////////////////////////////////////////////////////////////////
//
// struct protocol
// {
// };

///////////////////////////////////////////////////////////////////////////
// Transport                                                             //
///////////////////////////////////////////////////////////////////////////
//
// template <typename Protocol>
// struct transport
// {
//     ssize_t send (pfs::byte_string const &, pfs::error_code & ec);
// //    ssize_t recv (pfs::byte_string &, pfs::error_code & ec);
// };
//

template <uint8_t MajorVersion, uint8_t MinorVersion
        , typename IdGenerator
        , typename Serializer
        , typename Protocol
        , typename StringT = pfs::string
        , template <typename, typename> class AssociativeContainer = pfs::map
        , template <typename> class SequenceContainer = pfs::list>
struct rpc
{
    typedef StringT                     string_type;
    typedef IdGenerator                 id_generator;
    typedef typename id_generator::type id_type;
    typedef Serializer                  serializer_type;
    typedef Protocol                    protocol_type;

    ///////////////////////////////////////////////////////////////////////////
    // Client                                                                //
    ///////////////////////////////////////////////////////////////////////////

    template <template <typename> class Transport>
    class client
    {
        friend struct session;
        typedef Transport<Protocol> transport_type;

        struct session
        {
            id_type         _id;
            client &        _owner;
            error_code      _ec;
            serializer_type _serializer;

            session (id_type const & id, client & owner)
                : _id(id)
                , _owner(owner)
            {}

            session & call (string_type const & name)
            {
                _serializer.set_version(MajorVersion, MinorVersion)
                    .set_rpc_entity(RPC_METHOD)
                    .set_id(_id)
                    .set_method(name);
                return *this;
            }

            template <typename T>
            inline session & operator () (T const & value)
            {
                _serializer.add_param(value);
                return *this;
            }

            template <typename T>
            inline session & operator () (string_type const & name, T const & value)
            {
                _serializer.add_param(name, value);
                return *this;
            }

#if __cplusplus >= 201103L
            template <typename T, typename ...Args>
            inline session & operator () (T const & value, Args const &... args)
            {
                _serializer.add_param(value);
                return operator () (args...);
            }
#endif

            template <typename T>
            error_code result (T & value)
            {
                //
                // Send data
                //
                ssize_t n = _owner._transport.send(_serializer.pack(), _ec);

                if (n < 0 || _ec)
                    return _ec;

                //
                // Receive data
                //

                byte_string buffer;
                n = _transport.recv(buffer, _ec);

                if (n < 0 || _ec)
                    return _ec;

                _ec = _serializer.unpack(buffer);

                if (_ec)
                    return _ec;

                value = _serializer.value();

                return error_code();
            }

            error_code const & errorcode () const
            {
                return _ec;
            }

        private:
            bool begin_result ()
            {
//                 _ec.clear();
//                 _proto.commit_tx();
//
//                 // Send data via transport
//                 ssize_t n = _transport.send(_proto.data(), _ec);
//
//                 if (n < 0 || _ec)
//                     return false;
//
//                 // Receive data from server
//                 byte_string buffer;
//                 n = _transport.recv(buffer, _ec);
//
//                 if (n > 0 && !_ec)
//                     _proto.buffer().append(buffer);
//
//                 if (!_proto.begin_rx())
//                     return false;
//
//                 uint8_t major
//                         , minor
//                         , status;
//                 id_type id;
//
//                 _proto >> _proto.get_major_version(major)
//                         >> _proto.get_minor_version(minor)
//                         >> _proto.get_rpc_entity(status)
//                         >> _proto.get_id(id);
//
//                 if (major != MajorVersion || minor != MinorVersion) {
//                     // TODO set error code
//
//                     return false;
//                 }
//
//                 if (status != RPC_SUCCESS()) {
//                     // TODO set error code
//
//                     return false;
//                 }
//
//                 if (id != _id) {
//                     // TODO set error code
//
//                     return false;
//                 }
//
                return true;
            }
        };


    public:
        client (transport_type & transport)
            : _transport(transport)
        {}

        session & call (string const & method_name)
        {
            id_type id = _id_generator().next_id();
            std::pair<typename session_registry::iterator,bool> r
                    = _sessions.insert(id, session(id, *this));
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
        typedef AssociativeContainer<id_type, session> session_registry;

        id_generator     _id_generator;
        transport_type & _transport;
        session_registry _sessions;

    };








    //typedef int32_t error_code_type;


    //
    // Error codes
    //
//     static error_code_type const NO_ERROR () { return 0; }
//
//     static error_code_type const BAD_VERSION () { return 1; }
//
//     // Invalid JSON was received by the server.
//     // An error occurred on the server while parsing
//     // the JSON text.
//     static error_code_type const PARSE_ERROR () { return -32700; }
//
//     // The JSON sent is not a valid Request object.
//     static error_code_type const INVALID_REQUEST () { return -32600; }
//
//     // The method does not exist / is not available.
//     static error_code_type const METHOD_NOT_FOUND () { return -32601; }
//
//     // Invalid method parameter(s).
//     static error_code_type const INVALID_PARAMS () { return -32602; }
//
//     // Internal JSON-RPC error.
//     static error_code_type const INTERNAL_ERROR () { return -32603; }
//
//     // -32000 to -32099 Reserved for implementation-defined server-errors.
//     static error_code_type const SERVER_ERROR () { return -32000; }

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
    // Server                                                                //
    ///////////////////////////////////////////////////////////////////////////

    //template <typename Protocol, typename Transport>
    class server
    {
    public:
      //  typedef Transport transport_type;

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

//     struct default_id_generator
//     {
//         id_type next_id () const
//         {
//             static id_type id = 0;
//             return ++id;
//         }
//     };
};

} // pfs

