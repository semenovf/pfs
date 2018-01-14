#include "pfs/datetime.hpp"

namespace pfs {

void datetime::set_date (date const & d)
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

	intmax_t r = 10000000000L * Y + 100000000L * M + 1000000L * D
			+ 10000L * h + 100L * m + s;

	return r;
}

} // pfs


