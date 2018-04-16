#pragma once
#include <pfs/types.hpp>
#include <pfs/v2/string.hpp>
#include <pfs/lexical_cast.hpp>

namespace pfs {

class date
{
	intmax_t _jd; // Julian Day;

public:
	static const intmax_t NULL_JULIAN_DAY;
	static const intmax_t MIN_JULIAN_DAY   = -784366681008L; // Date::julianDay(PFS_INT_MIN, 1, 1)
	static const intmax_t MAX_JULIAN_DAY   = 784354017364L;  // Date::julianDay(PFS_INT_MAX, 12, 31)
    static const intmax_t EPOCH_JULIAN_DAY = 2440588L;       // Date::julianDay(1970, 1, 1)

public:
	date ()
		: _jd (NULL_JULIAN_DAY)
	{}

	date (int year, int month, int day)
	{
		set_date(year, month, day);
	}

	bool valid () const
	{
		return _jd >= MIN_JULIAN_DAY && _jd <= MAX_JULIAN_DAY;
	}

	date add_days (int ndays) const
	{
		return valid() ? from_julian_day(_jd + ndays) : date();
	}

	/**
	 *
	 * @param nmonths
	 * @return
	 */
	date add_months (int nmonths) const;

	/**
	 *
	 * @param nyears
	 * @return
	 */
	date add_years (int nyears) const;

	/**
	 *
	 * @return
	 */
	int day_of_week () const;

	/**
	 *
	 * @return
	 */
	int day_of_year () const;

	/**
	 *
	 * @return
	 */
	int days_in_month () const;

	/**
	 *
	 * @return
	 */
	int days_in_year () const;

	/**
	 *
	 * @param d
	 * @return
	 */
	intmax_t days_to (const date & d) const
	{
		return valid() && d.valid()
				? d._jd - _jd
				: 0;
	}

	/**
	 *
	 * @param year
	 * @param month
	 * @param day
	 * @return
	 */
	bool set_date (int year, int month, int day);

	/**
	 *
	 * @param year
	 * @param month
	 * @param day
	 */
	void split (int * year, int * month, int * day);

	/**
	 *
	 * @return
	 */
	int year () const;

	/**
	 *
	 * @return
	 */
	int month () const;

	/**
	 *
	 * @return
	 */
	int day () const;

	/**
	 *
	 * @return
	 */
	intmax_t julian_day() const
	{
		return _jd;
	}

	/**
	 *
	 * @param other
	 * @return
	 */
	bool operator == ( const date & other ) const
	{
		return _jd == other._jd;
	}

	bool operator != ( const date & other ) const
	{
		return _jd != other._jd;
	}

	bool operator  < ( const date & other ) const
	{
		return _jd <  other._jd;
	}

	bool operator <= ( const date & other ) const
	{
		return _jd <= other._jd;
	}

	bool operator > (const date & other) const
	{
		return _jd >  other._jd;
	}

	bool operator >= (const date & other) const
	{
		return _jd >= other._jd;
	}

	/**
	 *
	 * @param year
	 * @param month
	 * @param day
	 * @return
	 */
	static intmax_t julian_day (int year, int month, int day);

	/**
	 *
	 * @param julianDay
	 * @param yearPtr
	 * @param monthPtr
	 * @param dayPtr
	 */
	static void from_julian_day (intmax_t julianDay, int * yearPtr, int * monthPtr, int * dayPtr);

	/**
	 *
	 * @param julianDay
	 * @return
	 */
	static date from_julian_day (intmax_t julianDay)
	{
		date d;
		if (julianDay >= MIN_JULIAN_DAY && julianDay <= MAX_JULIAN_DAY)
			d._jd = julianDay;
		return d;
	}

	/*
	 * Algorithm (http://en.wikipedia.org/wiki/Leap_year):
	 *
	 * if year is divisible by 400 then
	 * 		is_leap_year
	 * else if year is divisible by 100 then
	 * 		not_leap_year
	 * else if year is divisible by 4 then
	 * 		is_leap_year
	 * else
	 * 		not_leap_year
	 */
	static bool is_leap_year (int year)
	{
	    if (year < 1) // There is no year 0 in Gregorian calendar
	        ++year;
	    return year % 400 == 0 || (year % 4 == 0 && year % 100 != 0);
	}

	/**
	 *
	 * @param year
	 * @param month
	 * @param day
	 * @return
	 */
	static bool valid (int year, int month, int day);
};


namespace details {
namespace date {

template <typename StringT>
inline void append_prefixed2 (StringT & s, typename StringT::value_type fill_char, int i2)
{
	if (i2 >= 0 && i2 < 10) s.push_back(fill_char);
	s.append(to_string<StringT>(i2));
}

template <typename StringT>
inline void append_prefixed3 (StringT & s, typename StringT::value_type fill_char, int i3)
{
	if (i3 >= 0) {
		if (i3 < 100) s.push_back(fill_char);
		if (i3 < 10) s.push_back(fill_char);
	}
	s.append(to_string<StringT>(i3));
}

template <typename StringT>
inline void append_prefixed4 (StringT & s, typename StringT::value_type fill_char, int i4)
{
	if (i4 >= 0) {
		if (i4 < 1000) s.push_back(fill_char);
		if (i4 < 100) s.push_back(fill_char);
		if (i4 < 10) s.push_back(fill_char);
	}
	s.append(to_string<StringT>(i4));
}

}} // details::date

/**
 *
 * @param format
 * @return
 *
 * %n     A newline character.
 * %t     A tab character.
 * %C     The century number (year/100) as a 2-digit integer.
 * %d     The day of the month as a decimal number (range 01 to 31).
 * %e     Like %d, the day of the month as a decimal number, but a leading zero is replaced by a space.
 * %F     Equivalent to %Y-%m-%d (the ISO 8601 date format).
 * %j     The day of the year as a decimal number (range 001 to 366).
 * %m     The month as a decimal number (range 01 to 12).
 * %u     The day of the week as a decimal, range 1 to 7, Monday being 1.
 * %y     The year as a decimal number without a century (range 00 to 99).
 * %Y     The year as a decimal number including the century.
 *
 * TODO need to support
 * %U     The week number of the current year as a decimal number, range 00 to 53,
 *        starting with the first Sunday as the first day of week 01.
 * %V     The ISO 8601 week number (see NOTES) of the current year as a decimal number,
 *        range 01 to 53, where week 1 is the first week that has at least 4 days in the new year.
 *        See also  %U  and %W.
 */
template <typename StringT>
StringT to_string (date const & d, StringT const & format)
{
	if (d.year() < 0 || d.year() > 9999)
		return StringT();

	// std::basic_stringstream<typename string::value_type> ss;
	StringT r;

	typename StringT::const_iterator p = format.cbegin();
	typename StringT::const_iterator end = format.cend();

	bool need_spec = false; // true if conversion specifier character expected

	while (p < end) {
		if (*p == '%') {
			if (need_spec) {
				r.push_back('%');
				need_spec = false;
			} else {
				need_spec = true;
			}
		} else {
			if (!need_spec) {
				r.push_back(*p);
			} else {
				switch (to_ascii(*p)) {
				case 'n':
					r.push_back('\n');
					break;
				case 't':
					r.push_back('\t');
					break;
				case 'C':
					details::date::append_prefixed2(r, '0', d.year()/100);
					break;
				case 'd':
					details::date::append_prefixed2(r, '0', d.day());
					break;
				case 'e':
					details::date::append_prefixed2(r, ' ', d.day());
					break;
				case 'F':
					details::date::append_prefixed4(r, '0', d.year());
					r.push_back('-');
					details::date::append_prefixed2(r, '0', d.month());
					r.push_back('-');
					details::date::append_prefixed2(r, '0', d.day());
					break;
				case 'j':
					details::date::append_prefixed3(r, '0', d.day_of_year());
					break;
				case 'm':
					details::date::append_prefixed2(r, '0', d.month());
					break;
				case 'u':
					r.append(to_string<StringT>(d.day_of_week()));
					break;
				case 'y':
					details::date::append_prefixed2(r, '0', d.year() % 100);
					break;
				case 'Y':
					details::date::append_prefixed4(r, '0', d.year());
					break;
				default:
					r.push_back('%');
					r.push_back(*p);
					break;
				}

				need_spec = false;
			}
		}
		++p;
	}

	return r;    
}

/**
 * @brief Converts date to string.
 *
 * @details The string format corresponds to the ISO 8601 specification,
 *          taking the form YYYY-MM-DD, where YYYY is the year,
 *          MM is the month of the year (between 01 and 12),
 *          and DD is the day of the month between 01 and 31.
 *
 * @return The date as string.
 */
template <typename StringT>
inline StringT to_string (date const & d)
{
	return to_string<StringT>(d, StringT("%F")); // equivalent to %H:%M:%S
}

pfs::date current_date ();

} // pfs
