#include "pfs/config.h"

#if HAVE_BACKWARD_CPP
#   include "3rdparty/backward-cpp/backward.hpp"
#   include <sstream>
#else
#   include <execinfo.h>
#endif

#include "pfs/cxx/cxx98/exception.hpp"

namespace pfs {

void exception::append_backtrace (std::string & buffer)
{
    buffer.append("\n\n---BACKTRACE BEGIN---");

#if HAVE_BACKWARD_CPP
    backward::StackTrace st;
    st.load_here(PFS_GNUC_BACKTRACE_MAXSIZE /*32*/);

    backward::TraceResolver tr;
    tr.load_stacktrace(st);

    for (size_t i = 0; i < st.size(); ++i) {
        backward::ResolvedTrace trace = tr.resolve(st[i]);

        std::stringstream out;
        out << "\n" << "#" << i
            << " " << trace.object_filename
            << " in " << trace.object_function;
//             << " SOURCE_FUNCTION:" << trace.source.function
//             << " SOURCE_FILENAME:" << trace.source.filename
//             << " SOURCE_LINE:" << trace.source.line;
            //<< " [" << trace.addr << "]";

        buffer.append(out.str());
    }

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
    buffer.append("\n---BACKTRACE END---");
}

} // namespace pfs
