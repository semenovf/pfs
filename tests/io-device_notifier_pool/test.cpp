#include <pfs/test.hpp>
#include "pfs/io/device_notifier_pool.hpp"

typedef pfs::io::device_notifier_pool<> device_notifier_pool;
using pfs::io::device_ptr;
using pfs::io::server_ptr;

#include "test_standard_streams.hpp"
#include "test_client_server.hpp"

int main ()
{
    BEGIN_TESTS(0);

//    test_standard_streams::run();
    test_client_server::run();

    return END_TESTS;
}
