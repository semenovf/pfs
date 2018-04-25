#include <iostream>
#include <pfs/test.hpp>
#include <pfs/rpc.hpp>
#include <pfs/system_error.hpp>
#include <pfs/byte_string.hpp>
#include <pfs/binary_istream.hpp>
#include <pfs/binary_ostream.hpp>
#include <pfs/io/buffer.hpp>

typedef pfs::rpc<> rpc_ns;

class Protocol : public rpc_ns::protocol
{
    typedef rpc_ns::protocol base_class;
    typedef pfs::byte_string::size_type size_type;

    // Minimum size for packet (empty)
    static size_type min_packet_size ()
    {
        return 4 // flags
            + sizeof(size_type);
    }

    static const char flag_prefix () { return '\x10'; }
    static const char flag_begin () { return '\x01'; }
    static const char flag_end () { return '\x02'; }

public:
    Protocol () : base_class() {}

    virtual bool begin_transfer () pfs_override
    {
        _output_buffer.clear();
        _os << flag_prefix() << flag_begin()
            << pfs::byte_string::size_type(0); // reserve for length
        return true;
    }

    virtual bool commit_transfer () pfs_override
    {
        // Set size of packet (excluding leading and trailing falgs - 4 bytes)
        pfs::byte_string sz;
        pfs::binary_ostream<pfs::byte_string> o(sz);
        o << _output_buffer.size() - 4;
        PFS_ASSERT(sz.size() == sizeof(pfs::byte_string::size_type));
        _output_buffer.replace(2, sizeof(pfs::byte_string::size_type), sz);
        _os << flag_prefix() << flag_end();
        return true;
    }


    virtual bool begin_payload () pfs_override
    {
        while (_is.available() >= min_packet_size()) {
            char prfx, b;
            size_type len;

            _is >> prfx;

            if (prfx != flag_prefix())
                continue;

            _is >> b;

            if (b != flag_begin())
                continue;

            _is >> len;

            // Incomplete Packet
            if (_is.available() >= len + 2) // + 2 for `flag_prefix` and `flag_end` flags
                return true;
        }

        return false;
    }

    virtual bool commit_payload () pfs_override
    {
        if (_is.available() < 2)
            return false;

        char prfx, e;

        _is >> prfx;

        if (prfx != flag_prefix())
            return false;

        _is >> e;

        if (e != flag_end())
            return false;

        _input_buffer.erase(_input_buffer.cbegin(), _is.cbegin());
        _is.reset(_input_buffer);

        return true;
    }

    template <typename T>
    inline Protocol & operator << (T const & x)
    {
        _os << x;
        return *this;
    }

    inline Protocol & operator << (pfs::string const & s)
    {
        _os << s.size();
        pfs::string::const_iterator first = s.cbegin();
        pfs::string::const_iterator last = s.cend();

        while (first != last) {
            _os << *first++;
        }
    }

    template <typename T>
    inline protocol & operator >> (T & x)
    {
        _is >> x;
        return *this;
    }

    inline Protocol & operator >> (pfs::string & s)
    {
        pfs::string::size_type n = 0;
        _is >> n;

        while (n--) {
            pfs::string::value_type c;
            _is >> c;
            s.push_back(c);
        }
        return *this;
    }
};

class ServerTransport : public rpc_ns::transport
{
public:
    ServerTransport () : rpc_ns::transport() {}

    virtual ssize_t send (pfs::byte_string const &, pfs::error_code & ec) pfs_override
    {
        return -1;
    }

    virtual ssize_t recv (pfs::byte_string &, pfs::error_code & ec) pfs_override
    {
        return -1;
    }
};

class ClientTransport : public rpc_ns::transport
{
public:
    ClientTransport () : rpc_ns::transport() {}

    virtual ssize_t send (pfs::byte_string const &, pfs::error_code & ec) pfs_override
    {
        return -1;
    }

    virtual ssize_t recv (pfs::byte_string &, pfs::error_code & ec) pfs_override
    {
        return -1;
    }
};

class Session : public rpc_ns::session
{
public:
    Session () : rpc_ns::session() {}
};

int method1 ()
{
    std::cout << "method1()" << std::endl;
    return 123;
}

int method2 (int n)
{
    std::cout << "method2(" << n << ')' << std::endl;
    return n;
}

void notify1 ()
{
    std::cout << "notify1()" << std::endl;
}

void notify2 (int n)
{
    std::cout << "notify2(" << n << ')' << std::endl;
}

int main ()
{
    BEGIN_TESTS(0);

    rpc_ns::id_type idc = 0;
    rpc_ns::server<Protocol, Session, ServerTransport> server;
    rpc_ns::client<Protocol, Session, ClientTransport> client;

//     server.bind("method1", & method1);
//     server.bind("method2", & method2);
//     server.bind("notify1", & notify1);
//     server.bind("notify2", & notify2);

//     TEST_OK(client.call("method1").get<int>() == 123);
//     TEST_OK(client.call("method2")(425).get<int>() == 425);

    client.notify("notify1").send();
    client.notify("notify2")(123).send();

    return END_TESTS;
}
