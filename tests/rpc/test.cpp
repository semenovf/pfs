#include <iostream>
#include <pfs/test.hpp>
#include <pfs/rpc.hpp>
#include <pfs/system_error.hpp>
#include <pfs/byte_string.hpp>
#include <pfs/binary_istream.hpp>
#include <pfs/binary_ostream.hpp>
#include <pfs/io/buffer.hpp>

typedef pfs::rpc<1, 0> rpc_ns;

class Protocol : public rpc_ns::protocol<Protocol>
{
    typedef rpc_ns::protocol<Protocol> base_class;
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

    virtual manip major_version (uint8_t value) pfs_override
    {
        *this << value;
        return manip();
    }

    virtual manip minor_version (uint8_t value) pfs_override
    {
        *this << value;
        return manip();
    }

    virtual manip rpc_entity (uint8_t value)
    {
        *this << value;
        return manip();
    }

    virtual manip method_name (string_type const & value) pfs_override
    {
        *this << value;
        return manip();
    }

    virtual manip id (id_type const & value) pfs_override
    {
        *this << value;
        return manip();
    }

    virtual manip get_major_version (uint8_t & value) pfs_override
    {
        *this >> value;
        return manip();
    }

    virtual manip get_minor_version (uint8_t & value) pfs_override
    {
        //return *this >> value;
        return manip();
    }

    virtual manip get_rpc_entity (uint8_t & value) pfs_override
    {
        //return *this >> value;
        return manip();
    }

    virtual manip get_method_name (string_type & value) pfs_override
    {
        //return *this >> value;
        return manip();
    }

    virtual manip get_id (id_type & value) pfs_override
    {
        //return *this >> value;
        return manip();
    }

    template <typename T>
    manip param (T const & x)
    {
        return manip();
    }

    template <typename T>
    manip get_param (T & x)
    {
        return manip();
    }

    template <typename T>
    manip param (string_type const & param_name, T const & x)
    {
        return manip();
    }

    template <typename T>
    manip get_param (string_type & param_name, T & x)
    {
        return manip();
    }

    virtual bool begin_tx () pfs_override
    {
        _output_buffer.clear();
        _os << flag_prefix() << flag_begin()
            << pfs::byte_string::size_type(0); // reserve for length
        return true;
    }

    virtual bool commit_tx () pfs_override
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

    virtual bool begin_rx () pfs_override
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

    virtual bool commit_rx () pfs_override
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

//     inline Protocol & operator << (manip m)
//     {
//         return *this;
//     }
//
//     inline Protocol & operator >> (manip m)
//     {
//         return *this;
//     }
//
//     template <typename T>
//     inline Protocol & operator << (T const & x)
//     {
//         _os << x;
//         return *this;
//     }
//
//     inline Protocol & operator << (pfs::string const & s)
//     {
//         _os << s.size();
//         pfs::string::const_iterator first = s.cbegin();
//         pfs::string::const_iterator last = s.cend();
//
//         while (first != last) {
//             _os << *first++;
//         }
//     }
//
//     template <typename T>
//     inline Protocol & operator >> (T & x)
//     {
//         _is >> x;
//         return *this;
//     }
//
//     Protocol & operator >> (pfs::string & s)
//     {
//         pfs::string::size_type n = 0;
//         _is >> n;
//
//         while (n--) {
//             pfs::string::value_type c;
//             _is >> c;
//             s.push_back(c);
//         }
//         return *this;
//     }
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

int integer ()
{
    std::cout << "integer()" << std::endl;
    return 123;
}

int echo (int n)
{
    std::cout << "echo(" << n << ')' << std::endl;
    return n;
}

int sum (int a, int b)
{
    std::cout << "sum(" << a << ',' << b << ')' << std::endl;
    return a + b;
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
    rpc_ns::server<Protocol, ServerTransport> server;
    rpc_ns::client<Protocol, ClientTransport> client;

//     server.bind("method1", & method1);
//     server.bind("method2", & method2);
//     server.bind("notify1", & notify1);
//     server.bind("notify2", & notify2);

    TEST_OK(client.call("integer").result<int>() == 123);
    TEST_OK(client.call("echo")(425).result<int>() == 425);
    TEST_OK(client.call("sum")(1, 2).result<int>() == 3);

//     client.notify("notify1").send();
//     client.notify("notify2")(123).send();

    return END_TESTS;
}
