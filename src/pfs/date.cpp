#include "pfs/date.hpp"
#include "pfs/math.hpp"
#include "pfs/utility.hpp"
#include "pfs/algorithm.hpp"

// Sources:
// 		http://en.wikipedia.org/wiki/Julian_day ,
// 		http://www.tondering.dk/claus/cal/julperiod.php ,
//      http://www.hermetic.ch/cal_stud/jdn.htm ,
// 		and source files from Qt library.
// Useful links:
//		Julian Day Calculator: http://www.csgnetwork.com/juliandaydate.html


namespace pfs {

const intmax_t date::NULL_JULIAN_DAY  = numeric_limits<intmax_t>::max();

static const byte_t __daysInMonth[] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

static inline date __valid_date(int y, int m, int d)
{
	// first day of the month
    date r(y, m, 1);

    // set date according to the number of days in the specified month.
    r.set_date(y, m, min(d, r.days_in_month()));
    return r;
}

/**
 * @brief Calculates Julian Day (JD) from Gregorian calendar date
 *
 * @param year  Year.
 * @param month Month.
 * @param day   Day.
 * @return JD value.
 */
intmax_t date::julian_day (int year, int month, int day) // static
{
	if (year < 0) // there is no 0 year
		++year;

	int      a = math::floor_div(14 - month, 12);
	intmax_t y = intmax_t(year) + 4800 - a;
	int      m = month + 12 * a - 3;

	// Gregorian calendar: >= 15.10.1582
	if (year > 1582 || (year == 1582 && (month > 10 || (month == 10 && day >= 15)))) {
		return day + math::floor_div(153 * m + 2, 5)
			+ 365 * y
			+ math::floor_div(y, 4)
			- math::floor_div(y, 100)
			+ math::floor_div(y, 400)
			- 32045;
	}
	// Julian calendar: <= 4.10.1582
	else if (year < 1582 || (year == 1582 && (month < 10 || (month == 10 && day <= 4)))) {
		return day + math::floor_div(153 * m + 2, 5)
			+ 365 * y
			+ math::floor_div(y, 4)
			- 32083;
	}

	return NULL_JULIAN_DAY;
}

/**
 * @brief Calculates Gregorian calendar date from Julian Day (JD)
 *
 * @param julianDay JD value.
 * @param yearPtr   Year result.
 * @param monthPtr  Month result.
 * @param dayPtr    Day result.
 *
 * @note see http://www.tondering.dk/claus/cal/julperiod.php.
 */
void date::from_julian_day (intmax_t julianDay, int * yearPtr, int * monthPtr, int * dayPtr) // static
{
	intmax_t b = 0;
	intmax_t c = 0;

	// Gregorian calendar
	//
	if (julianDay >= 2299161) {
		intmax_t a = julianDay + 32044;
		b = math::floor_div(4 * a + 3, 146097);
		c = a - math::floor_div(146097 * b, 4);
	} else {
		b = 0;
		c = julianDay + 32082;
	}

    intmax_t    d = math::floor_div(4 * c + 3, 1461);
    intmax_t    e = c - math::floor_div(1461 * d, 4);
    intmax_t    m = math::floor_div(5 * e + 2, 153);

    intmax_t    day = e - math::floor_div(153 * m + 2, 5) + 1;
    intmax_t    month = m + 3 - 12 * math::floor_div(m, 10);
    intmax_t    year = 100 * b + d - 4800 + math::floor_div(m, 10);

    if (year <= 0)
        --year;

    if (yearPtr)
        *yearPtr = int(year);

    if (monthPtr)
        *monthPtr = int(month);

    if (dayPtr)
        *dayPtr = int(day);
}


/**
 * @brief Checks if the specified date is valid
 *
 * @param year  Year value.
 * @param month Month value.
 * @param day   Day value.
 * @return @c true if the specified date is valid; otherwise returns @c false.
 */
bool date::valid (int year, int month, int day) // static
{
    return year == 0
        ? false
    	: (day > 0 && month > 0 && month <= 12) &&
           (day <= __daysInMonth[month] || (day == 29 && month == 2 && is_leap_year(year)));
}



// 31.01.2013 + 1 mon = 29.02.2013
// 31.01.2013 - 2 mon = 30.11.2012
date date::add_months (int nmonths) const
{
	// Note: algorithm adopted from QDate::addMonths

    if (!valid())
        return date();

    if (!nmonths)
    	return *this;

    int y, m, d;
    from_julian_day(_jd, & y, & m, & d);

    int start_year = y;

    while (nmonths != 0) {
    	if (nmonths < 0 && nmonths + 12 <= 0) {
    		--y;
    		nmonths += 12;
    	} else if (nmonths < 0) {
    		m += nmonths;
    		nmonths = 0;

    		if (m <= 0) {
    			--y;
    			m += 12;
    		}
    	} else if (nmonths - 12 >= 0) {
    		++y;
    		nmonths -= 12;
    	} else if (m == 12) {
    		++y;
    		m = 0;
    	} else {
    		m += nmonths;
    		nmonths = 0;

    		if (m > 12) {
    			++y;
    			m -= 12;
    		}
    	}
    }

    // special cases: transition the year through 0
    if (start_year > 0 && y <= 0)      // decreasing months
    	--y;
    else if (start_year < 0 && y >= 0) // increasing months
    	++y;

    return __valid_date(y, m, d);
}

date date::add_years (int nyears) const
{
	// Note: algorithm adopted from QDate::addYears

    if (!valid())
        return date();

    int y, m, d;
    from_julian_day(_jd, & y, & m, & d);

    int start_year = y;
    y += nyears;

    // special cases: transition the year through 0
    if (start_year > 0 && y <= 0)      // decreasing months
    	--y;
    else if (start_year < 0 && y >= 0) // increasing months
    	++y;

    return __valid_date(y, m, d);
}


/**
 * @brief Returns the weekday (1 = Monday to 7 = Sunday) for this date.
 *
 * @return The weekday (1 = Monday to 7 = Sunday) for this date
 *         or 0 if the date is invalid.
 */
int date::day_of_week() const
{
    return !valid()
        ? 0
        : (_jd >= 0)
          	  ? (_jd % 7) + 1
    		  : ((_jd + 1) % 7) + 7;
}

/**
 * @brief Returns the day of the year (1 to 365 or 366 on leap years) for
          this date.
 *
 * @return The day of the year (1 to 365 or 366 on leap years) for
    this date, or 0 if the date is invalid.
 */
int date::day_of_year() const
{
    return !valid()
        ? 0
        : int(_jd - julian_day(year(), 1, 1) + 1);
}

/**
 * @brief Calculates the number of days in the month (28 to 31) for this date.
 *
 * @return The number of days in the month (28 to 31) for this date
 *         or 0 if the date is invalid.
 */
int date::days_in_month() const
{
    if (! valid())
        return 0;

    int y, m;

    from_julian_day(_jd, & y, & m, 0);

    return  (m == 2 && is_leap_year(y))
    		? 29
    		: __daysInMonth[m];
}

/**
 * @brief Calculates the number of days in the year (365 or 366) for this date.
 *
 * @return The number of days in the year (365 or 366) for this date
 *         or 0 if the date is invalid.
 */
int date::days_in_year () const
{
    if (! valid())
        return 0;

    int y;
    from_julian_day(_jd, & y, 0, 0);
    return is_leap_year(y) ? 366 : 365;
}

/**
 * @brief Extracts the date's year, month, and day.
 *
 * @param year
 * @param month
 * @param day
 */
void date::split(int * year, int * month, int * day)
{
    if (valid()) {
        from_julian_day(_jd, year, month, day);
    } else {
        if (year)
            *year = 0;
        if (month)
            *month = 0;
        if (day)
            *day = 0;
    }
}

bool date::set_date (int year, int month, int day)
{
    if (valid(year, month, day))
        _jd = julian_day(year, month, day);
    else
        _jd = NULL_JULIAN_DAY;

    return valid();
}

int date::year () const
{
    if (!valid())
        return 0;

    int y;
    from_julian_day(_jd, & y, 0, 0);
    return y;
}

int date::month () const
{
    if (!valid())
        return 0;

    int m;
    from_julian_day(_jd, 0, & m, 0);
    return m;
}

int date::day () const
{
    if (!valid())
        return 0;

    int d;
    from_julian_day(_jd, 0, 0, & d);
    return d;
}

} // pfs
