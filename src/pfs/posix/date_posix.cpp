#include <ctime>
#include <locale>
#include <langinfo.h>
#include "pfs/date.hpp"

//#if ! PFS_HAVE_LOCALTIME_R
//#    include <pfs/mutex.hpp>
//#endif

namespace pfs {

date current_date ()
{
    time_t t;
    ::time(& t);

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
    return date(tmPtr->tm_year + 1900, tmPtr->tm_mon + 1, tmPtr->tm_mday);
}

string date::month_abbrev (int index)
{
    PFS_ASSERT(index > 0 || index <= 12)

    // Query current locale fot date and time values
    char * lc_time_locale = setlocale(LC_TIME, "");
    //locale_t loc = newlocale(LC_TIME, lc_time_locale, 0);

    if (lc_time_locale) {
        static int const abmon_index [] = {
              ABMON_1, ABMON_2, ABMON_3, ABMON_4,  ABMON_5, ABMON_6
            , ABMON_7, ABMON_8, ABMON_9, ABMON_10, ABMON_11, ABMON_12
        };
        return string(nl_langinfo(abmon_index[index - 1]));
    } else {
        static char const * abmon[] = {
                    "jan", "feb", "mar", "apr", "may", "jun"
                , "jul", "aug", "sep", "oct", "nov", "dec"
        };
        return string(abmon[index - 1]);
    }
}

} // pfs
