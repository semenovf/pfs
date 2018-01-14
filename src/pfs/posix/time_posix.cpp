#include <ctime>
#include <sys/time.h> // gettimeofday
#include "pfs/assert.hpp"
#include "pfs/time.hpp"

//#if ! PFS_HAVE_LOCALTIME_R
//#	include "pfs/mutex.hpp"
//#endif

namespace pfs {

pfs::time current_time ()
{
    struct timeval tv;
    gettimeofday(& tv, 0);
    time_t t = tv.tv_sec;

    struct tm * tmPtr = 0;

//#if PFS_HAVE_LOCALTIME_R
    tzset();
    struct tm res;
    tmPtr = localtime_r(& t, & res);
//#else
//    static pfs::mutex mtx;
//    pfs::lock_guard<pfs::mutex> locker(mtx);
//    tmPtr = localtime(& t);
//#endif

    PFS_ASSERT(tmPtr);

    return pfs::time(tmPtr->tm_hour, tmPtr->tm_min, tmPtr->tm_sec, tv.tv_usec/1000);
}

} // pfs::platform

