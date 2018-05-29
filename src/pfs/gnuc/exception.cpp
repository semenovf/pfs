#if HAVE_BACKWARD_CPP
#   include "3rdparty/backward-cpp/backward.hpp"
#else
#   include <execinfo.h>
#endif

#include "pfs/cxx/cxx98/exception.hpp"

namespace pfs {

void exception::append_backtrace (std::string & buffer)
{
#if HAVE_BACKWARD_CPP

#else // ! HAVE_BACKWARD_CPP

    void * bt_buffer[PFS_GNUC_BACKTRACE_MAXSIZE];
    int bt_size = ::backtrace(bt_buffer, PFS_GNUC_BACKTRACE_MAXSIZE);
    char ** bt_symbols = ::backtrace_symbols(bt_buffer, bt_size);

    if (bt_symbols) {
        for (int i = 0; i < bt_size; i++) {
            buffer.append("\n");
            buffer.append(bt_symbols[i]);
        }
        free(bt_symbols);
    }
#endif // HAVE_BACKWARD_CPP
}

} // namespace pfs
