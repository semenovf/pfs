#include "pfs/time.hpp"

namespace pfs {

bool time::set_time (int hour, int min, int sec, int millis)
{
    if (valid(hour, min, sec, millis)) {
        _millis = (hour * SECONDS_PER_HOUR + min * SECONDS_PER_MINUTE + sec) * 1000 + millis;
        return true;
    }

    _millis = NULL_TIME;
    return false;
}

time time::add_millis (int millis) const
{
    time r;
    if (valid()) {
        if (millis >= 0) {
        	r._millis = (_millis + millis) % MILLIS_PER_DAY;
        } else {
            int ndays = (MILLIS_PER_DAY - millis) / MILLIS_PER_DAY;
            r._millis = (_millis + millis + ndays * MILLIS_PER_DAY) % MILLIS_PER_DAY;
        }
    }
    return r;
}

} // pfs



