#include <pfs/test.hpp>
#include "pfs/io/device_notifier_pool.hpp"

typedef pfs::io::device_notifier_pool<> device_notifier_pool;

int main ()
{
    BEGIN_TESTS(0);

    device_notifier_pool dnp;
    dnp.insert(0, pfs::io::notify_read);
    dnp.insert(1, pfs::io::notify_write);
    dnp.insert(2, pfs::io::notify_all);

    return END_TESTS;
}
