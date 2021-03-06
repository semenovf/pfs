#include <signal.h>
#include <fcntl.h>
#include <unistd.h>
#include "pfs/system_error.hpp"
#include "pfs/safeformat.hpp"
#include "pfs/modulus.hpp"
#include "pfs/assert.hpp"

namespace pfs {

static basic_dispatcher * __active_dispatcher = 0;

static int const signal_list[] = {
      SIGHUP
    , SIGINT
    , SIGQUIT
    , SIGILL
    , SIGABRT
    , SIGFPE
};

static void __signal_handler (int signum)
{
    if (__active_dispatcher) {
        switch (signum)  {
        case SIGHUP:
        case SIGINT:
        case SIGQUIT:
        case SIGABRT:
        case SIGILL:
        case SIGFPE:
            __active_dispatcher->quit();
            break;
        }
    }
}

static bool __set_posix_signal_handling (basic_dispatcher * disp
    , void (* handler)(int))
{
    struct sigaction act;
    act.sa_handler = handler;
    act.sa_flags = SA_RESTART;

    // Block every signal during the handler
    sigfillset(& act.sa_mask);

    int count = sizeof(signal_list)/sizeof(signal_list[0]);

    for (int i = 0; i < count; ++i) {
        PFS_ASSERT(sigaction(signal_list[i], & act, NULL) >= 0);
//        if (sigaction(signal_list[i], & act, NULL) < 0) {
//            disp->print_error(0, current_datetime(), dispatcher::fmt("sigaction(%d): %s")
//                    (signal_list[i])
//                    (to_string<dispatcher::string_type>(pfs::get_last_system_error())));
//
//            return false;
//        }
    }

    return true;
}

// activate of handling some posix signals
//
bool basic_dispatcher::activate_posix_signal_handling ()
{
    if (! __set_posix_signal_handling(this, & __signal_handler))
        return false;

    __active_dispatcher = this;

    return true;
}

void basic_dispatcher::deactivate_posix_signal_handling ()
{
    __set_posix_signal_handling(this, SIG_DFL);
    __active_dispatcher = 0;
}

} // pfs

