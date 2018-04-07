#include <pfs/test/test.hpp>
#include <pfs/rpc.hpp>

typedef pfs::rpc<1, 0> rpc_ns;

struct server_handler
{};

struct client_handler
{};

int main ()
{
    BEGIN_TESTS(0);

    server_handler shandler;
    client_handler chandler;
    rpc_ns::server<server_handler> server(shandler);
    rpc_ns::client<client_handler> client(chandler);

    return END_TESTS;
}
