#pragma once
#include <string>
#include <pfs/endian.hpp>
#include <pfs/memory.hpp>
#include <pfs/types.hpp>
#include <pfs/functional.hpp>
#include <pfs/byte_string.hpp>
#include <pfs/string.hpp>
#include <pfs/list.hpp>
#include <pfs/map.hpp>
#include <pfs/system_error.hpp>

namespace pfs {

///////////////////////////////////////////////////////////////////////////
// RPC error                                                             //
///////////////////////////////////////////////////////////////////////////

#if __cplusplus >= 201103L
enum class rpc_errc
{
#else
struct rpc_errc
{
    enum value_enum {
#endif
          no_error = 0              //!< No error.
        , bad_version = 1           //!< Version does not match.
        , id_not_match = 2          //!< Response ID does not match requested.
        , parse_error = -32700      //!< An error occurred on the server while parsing.
        , invalid_request = -32600  //!< Is not a valid request.
        , method_not_found = -32601 //!< The method does not exist / is not available.
        , invalid_params = -32602   //!< Invalid method parameter(s).
        , internal_error = -32603   //!< Intrenal error.
        , invalid_response = -32604 //!< Is not a valid response.
        //
        // -32000 to -32099 Reserved for implementation-defined server-errors.
        //
#if __cplusplus < 201103L
    };

    value_enum v;

    rpc_errc (value_enum x)
        : v(x)
    {}

    rpc_errc & operator = (value_enum x)
    {
        v = x;
        return *this;
    }

    operator int () const
    {
        return static_cast<int>(v);
    }
#endif
};

namespace details {
class rpc_error_category : public pfs::error_category
{
public:
    virtual char const * name () const pfs_noexcept pfs_override;
    virtual std::string message (int ev) const pfs_override;
};
} // details

pfs::error_category const & rpc_error_category ();

inline pfs::error_code make_error_code (rpc_errc e)
{
    return pfs::error_code(static_cast<int>(e), rpc_error_category());
}

class rpc_exception : public error_code_exception
{
public:
    rpc_exception (error_code const & ec) : error_code_exception(ec) {}
    explicit rpc_exception (error_code const & ec, char const * s) : error_code_exception(ec, s) {}
    explicit rpc_exception (error_code const & ec, std::string const & s) : error_code_exception(ec, s) {}
};

} // pfs

namespace pfs {

enum rpc_entity {
      RPC_METHOD       = 1
    , RPC_NOTIFICATION = 2
    , RPC_SUCCESS      = 3
    , RPC_ERROR        = 4
    , rpc_method       = RPC_METHOD
    , rpc_notification = RPC_NOTIFICATION
    , rpc_success      = RPC_SUCCESS
    , rpc_error        = RPC_ERROR
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
//     pfs::byte_string envelope (pfs::byte_string const & payload) const?
//     pfs::byte_string unenvelope (pfs::byte_string const & data) const?
// };
//
//////////////////////////////////////////////////////////////////////////
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

    template <typename Transport>
    class client
    {
        friend struct session;
        typedef Transport transport_type;
        typedef typename Transport::istream_type istream_type;
        typedef typename Transport::ostream_type ostream_type;

        struct session
        {
            id_type         _id;
            client &        _owner;
            serializer_type _serializer;

            session (id_type const & id, client & owner)
                : _id(id)
                , _owner(owner)
            {}

            session & call (string_type const & name)
            {
                _serializer.set_version(MajorVersion, MinorVersion)
                    .set_entity(RPC_METHOD)
                    .set_id(_id)
                    .set_method(name);
                return *this;
            }

//             session & notify (string_type const & name)
//             {
//                 _serializer.set_version(MajorVersion, MinorVersion)
//                     .set_entity(RPC_NOTIFICATION)
//                     .set_method(name);
//                 return *this;
//             }

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
            inline T result ()
            {
                T value;
                error_code ec = this->result(value);
                if (ec)
                    PFS_THROW(rpc_exception(ec));
                return value;
            }

            template <typename T>
            error_code result (T & value)
            {
                error_code ec;
                Protocol proto;

                //
                // Send data
                //
                ostream_type & writer = _owner._transport.writer();

                if (! proto.send(writer, _serializer.pack()))
                    return make_error_code(rpc_errc::internal_error); // TODO May be need special error for this situation

                //
                // Receive data
                //
                byte_string payload;
                istream_type & reader = _owner._transport.reader();

                if (! proto.recv(reader, payload))
                    return make_error_code(rpc_errc::internal_error); // TODO May be need special error for this situation

                if (! _serializer.unpack(payload, ec))
                    return ec;

                uint8_t major, minor;
                int entity;
                id_type id;

                if (!_serializer.get_version(major, minor))
                    return make_error_code(rpc_errc::parse_error);

                if (major != MajorVersion || minor != MinorVersion)
                    return make_error_code(rpc_errc::bad_version);

                if (!_serializer.get_entity(entity))
                    return make_error_code(rpc_errc::parse_error);

                if (entity != RPC_SUCCESS)
                    return make_error_code(rpc_errc::invalid_response);

                if (!_serializer.get_id(id))
                    return make_error_code(rpc_errc::parse_error);

                if (id != _id)
                    return make_error_code(rpc_errc::id_not_match);

                if (_serializer.get_value(value))
                    return make_error_code(rpc_errc::parse_error);

                return error_code();
            }
        }; // session

    public:
        client (transport_type & transport)
            : _transport(transport)
        {}

        session & call (string const & method_name)
        {
            id_type id = _id_generator.next_id();
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

    ///////////////////////////////////////////////////////////////////////////
    // Server                                                                //
    ///////////////////////////////////////////////////////////////////////////

    template <typename Transport>
    class server
    {
        friend struct session;
        typedef Transport transport_type;

        struct basic_binder
        {
            virtual ~basic_binder () {}
            virtual error_code operator () (serializer_type & out, serializer_type const & in) = 0;
        };

        template <typename F>
        struct binder : basic_binder
        {
            pfs::function<F> _f;
            binder (pfs::function<F> f) : _f(f) {}
            virtual error_code operator () (serializer_type & out, serializer_type const & in) pfs_override
            {
                return error_code();
            }
        };

        typedef AssociativeContainer<string_type, basic_binder *> repository_type;

//         template <typename F>
//         struct binder : basic_binder
//         {
//             boost::function<F> _f;
//
//             binder (F f) : _f(boost::bind<F>(f)) {}
//
//             virtual error_code operator () (serializer_type & out, serializer_type const & in) pfs_override
//             {
//                 if (in.has_params())
//                     return make_error_code(rpc_errc::invalid_params);
//                 out.set_result(_f());
//                 return error_code();
//             }
//         };

//         template <typename F
//                 , typename A1 = void
//                 , typename A2 = void
//                 , typename A3 = void
//                 , typename A4 = void
//                 , typename A5 = void
//                 , typename A6 = void
//                 , typename A7 = void
//                 , typename A8 = void
//                 , typename A9 = void>
//         struct binder
//         {
//             //typedef R result_type;
//             pfs::function<void()> _f;
//             pfs::tuple<A1, A2, A3, A4, A5, A6, A7, A8, A9> _args;
//
//             binder (F f) : _f(f) {}
//             binder (F f, A1 a1)
//                 : _f(f)
//                 , _args(a1) {}
//             binder (F f, A1 a1, A2 a2)
//                 : _f(f)
//                 , _args(a1, a2) {}
//             binder (F f, A1 a1, A2 a2, A3 a3)
//                 : _f(f)
//                 , _args(a1, a2, a3) {}
//             binder (F f, A1 a1, A2 a2, A3 a3, A4 a4)
//                 : _f(f)
//                 , _args(a1, a2, a3, a4) {}
//             binder (F f, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5)
//                 : _f(f)
//                 , _args(a1, a2, a3, a4, a5) {}
//             binder (F f, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6)
//                 : _f(f)
//                 , _args(a1, a2, a3, a4, a5, a6) {}
//             binder (F f, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7)
//                 : _f(f)
//                 , _args(a1, a2, a3, a4, a5, a6, a7) {}
//             binder (F f, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8)
//                 : _f(f)
//                 , _args(a1, a2, a3, a4, a5, a6, a7, a8) {}
//             binder (F f, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9)
//                 : _f(f)
//                 , _args(a1, a2, a3, a4, a5, a6, a7, a8, a9) {}
//
//             error_code operator () (serializer_type & out, serializer_type const & in) pfs_override
//             {
//                 if ()
//
//                 if (in.has_params())
//                     return make_error_code(rpc_errc::invalid_params);
//
//                 //result = _f();
//                 return error_code();
//             }
//
//         private:
//         };

//         template <typename R, typename F
//                 , typename A1>
//         struct binder1 : binder<R>
//         {
//             F _f;
//             binder1 (F f) : _f(f) {}
//
//             virtual error_code operator () (R & result, serializer_type & serializer) pfs_override
//             {
//                 A1 a1;
//
//                 if (!serializer.get_param(a1))
//                     return make_error_code(rpc_errc::invalid_params);
//
//                 result = _f(a1);
//                 return error_code();
//             }
//         };
//
//         template <typename R, typename F
//                 , typename A1
//                 , typename A2>
//         struct binder2 : binder<R>
//         {
//             F _f;
//             binder2 (F f) : _f(f) {}
//
//             virtual error_code operator () (R & result, serializer_type & serializer) pfs_override
//             {
//                 A1 a1;
//                 A2 a2;
//
//                 if (!serializer.get_param(a1)
//                         || !serializer.get_param(a2))
//                     return make_error_code(rpc_errc::invalid_params);
//
//                 result = _f(a1, a2);
//                 return error_code();
//             }
//         };
//
//         template <typename R, typename F
//                 , typename A1
//                 , typename A2
//                 , typename A3>
//         struct binder3 : binder<R>
//         {
//             F _f;
//             binder3 (F f) : _f(f) {}
//
//             virtual error_code operator () (R & result, serializer_type & serializer) pfs_override
//             {
//                 A1 a1;
//                 A2 a2;
//                 A2 a3;
//
//                 if (!serializer.get_param(a1)
//                         || !serializer.get_param(a2)
//                         || !serializer.get_param(a3))
//                     return make_error_code(rpc_errc::invalid_params);
//
//                 result = _f(a1, a2, a3);
//                 return error_code();
//             }
//         };

        //
        // TODO call_binder4, call_binder5, call_binder6, call_binder7, call_binder8
        //

        struct session
        {
        };

    public:
        server (transport_type & transport)
            : _transport(transport)
        {}

        template <typename F>
        void bind (string_type const & name, F & f)
        {
            _repo.insert(name, new binder<F>(f));
        }

        error_code dispatch ()
        {
            error_code ec;

            //
            // Receive data
            //

            byte_string buffer;
            ssize_t n = _transport.recv(buffer, ec);

            // No data
            if (n == 0)
                return error_code();

            if (n < 0 || ec)
                return ec;

            serializer_type in;

            byte_string payload = Protocol().unenvelope(buffer);

            if (! in.unpack(payload, ec))
                return ec;

            uint8_t major, minor;
            int entity;
            id_type id;

            if (!in.get_version(major, minor))
                return make_error_code(rpc_errc::parse_error);

            if (major != MajorVersion || minor != MinorVersion)
                return make_error_code(rpc_errc::bad_version);

            if (!in.get_entity(entity))
                return make_error_code(rpc_errc::parse_error);

            if (entity == RPC_METHOD) {
                if (!in.get_id(id))
                    return make_error_code(rpc_errc::parse_error);
                // TODO Handle method
            } else if (entity == RPC_NOTIFICATION) {
                // TODO Handle notification
            } else {
                return make_error_code(rpc_errc::invalid_request);
            }

            string_type method_name = in.get_method();

            if (method_name.empty())
                return make_error_code(rpc_errc::parse_error);

            typename repository_type::const_iterator it = _repo.find(method_name);

            if (it == _repo.cend())
                return make_error_code(rpc_errc::method_not_found);

            serializer_type out;

            basic_binder * binder = repository_type::mapped_reference(it);
            ec = (*binder)(out, in);

            if (ec)
                return ec;

            //
            // Send data
            //
            n = _transport.send(out.pack(), ec);

            if (n < 0 || ec)
                return ec;

            return error_code();
        }

    private:
        transport_type & _transport;
        repository_type  _repo;
    };
};

} // pfs

