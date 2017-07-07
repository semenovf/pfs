/*
 * datetime.cpp
 *
 *  Created on: Sep 13, 2013
 *      Author: wladt
 */

#include "pfs/datetime.hpp"

namespace pfs {

void datetime::set_date (const date & d)
{
    _date = d;

    if (_date.valid() && !_time.valid())
        _time.set_time(0, 0, 0);
}

datetime datetime::add_millis (intmax_t millis) const
{
	datetime r;

    intmax_t dd = _date.julian_day();
    intmax_t tt = time(0, 0, 0).millis_to(_time);
    int sign = 1;

    if (millis < 0) {
    	millis = -millis;
        sign = -1;
    }

    if (millis >= intmax_t(time::MILLIS_PER_DAY)) {
        dd += sign * (millis / time::MILLIS_PER_DAY);
        millis %= time::MILLIS_PER_DAY;
    }

    tt += sign * millis;

    if (tt < 0) {
        tt = time::MILLIS_PER_DAY - tt - 1;
        dd -= tt / time::MILLIS_PER_DAY;
        tt = tt % time::MILLIS_PER_DAY;
        tt = time::MILLIS_PER_DAY - tt - 1;
    } else if (tt >= time::MILLIS_PER_DAY) {
        dd += tt / time::MILLIS_PER_DAY;
        tt = tt % time::MILLIS_PER_DAY;
    }

    PFS_ASSERT(tt >= numeric_limits<int>::min() 
            && tt <= numeric_limits<int>::max());
    
    r._date.from_julian_day(dd);
    r._time = time(0, 0, 0).add_millis(int(tt));

    return r;
}

void datetime::set_millis_since_epoch (intmax_t millis)
{
    intmax_t days = millis / time::MILLIS_PER_DAY;
    millis %= time::MILLIS_PER_DAY;

    if (millis < 0) {
        --days;
        millis += time::MILLIS_PER_DAY;
    }

    PFS_ASSERT(days >= numeric_limits<int>::min() 
            && days <= numeric_limits<int>::max());
    
    PFS_ASSERT(millis >= numeric_limits<int>::min() 
            && millis <= numeric_limits<int>::max());
    
    _date = date(1970, 1, 1).add_days(int(days));
    _time = time(0, 0, 0).add_millis(int(millis));
}

intmax_t datetime::to_intmax () const
{
	date d = this->get_date();
	int Y = d.year();
	int M = d.month();
	int D = d.day();

	time t = this->get_time();
	int	h = t.hour();
	int	m = t.minute();
	int	s = t.second();

	intmax_t r = intmax_t(10000000000) * Y + intmax_t(100000000) * M + intmax_t(1000000) * D
			+ intmax_t(10000) * h + intmax_t(100) * m + s;

	return r;
}

system_string timezone::offset_to_string (long int off)
{
    int sign = 1;

    if (off < 0) {
        off *= -1;
        sign = -1;
    }

    int h = off / 3600;
    int m = (off - h * 3600) / 60;

    system_string sign_str = sign < 0
            ? system_string(1, '-')
            : system_string(1, '+');

    return sign_str
            + to_string<system_string>(h, 2, 10, true, system_string::value_type('0'))
            + to_string<system_string>(m, 2, 10, true, system_string::value_type('0')); //"-%02u%02u"
}

} // pfs


