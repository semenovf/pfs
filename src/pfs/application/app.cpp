#include "pfs/application/app.hpp"

namespace pfs {
namespace application {

int app::exec (dispatcher & d)
{
    int r = static_cast<int> (exit_status::failure);

    d.connect_all();

    if (d.start()) {
        r = d.exec();
    }

    d.finalize();

    return r;
}

}} // pfs
