#pragma once
#include <pfs/types.hpp>
#include <pfs/compare.hpp>
#include <pfs/string.hpp>

namespace pfs {

class date : public compare_op<date>
{
public:
    typedef intmax_t value_type;

    static const value_type NULL_JULIAN_DAY;
    static const value_type MIN_JULIAN_DAY   = -784366681008L; // Date::julianDay(PFS_INT_MIN, 1, 1)
    static const value_type MAX_JULIAN_DAY   = 784354017364L;  // Date::julianDay(PFS_INT_MAX, 12, 31)
    static const value_type EPOCH_JULIAN_DAY = 2440588L;       // Date::julianDay(1970, 1, 1)

private:
    value_type _jd; // Julian Day;

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
    value_type days_to (date const & d) const
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
    value_type julian_day() const
    {
        return _jd;
    }

    /**
     *
     * @param other
     * @return
     */
    bool operator == (date const & other) const
    {
        return _jd == other._jd;
    }

    bool operator  < (date const & other) const
    {
        return _jd <  other._jd;
    }

    /**
     *
     * @param year
     * @param month
     * @param day
     * @return
     */
    static value_type julian_day (int year, int month, int day);

    /**
     *
     * @param julianDay
     * @param yearPtr
     * @param monthPtr
     * @param dayPtr
     */
    static void from_julian_day (value_type julianDay, int * yearPtr, int * monthPtr, int * dayPtr);

    /**
     *
     * @param julianDay
     * @return
     */
    static date from_julian_day (value_type julianDay)
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
     *      is_leap_year
     * else if year is divisible by 100 then
     *      not_leap_year
     * else if year is divisible by 4 then
     *      is_leap_year
     * else
     *      not_leap_year
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

    string to_string (string const & format) const;

   /**
    * @brief Locale specific month abbreviation.
    * @param index Value in range from 1 to 12 (e.g. returned by month())
    */
    static string month_abbrev (int index);

private:

    static inline void append_prefixed2 (string & s, string::value_type fill_char, int i2)
    {
        if (i2 >= 0 && i2 < 10) s.push_back(fill_char);
        s.append(pfs::to_string(i2));
    }

    static inline void append_prefixed3 (string & s, string::value_type fill_char, int i3)
    {
        if (i3 >= 0) {
            if (i3 < 100) s.push_back(fill_char);
            if (i3 < 10) s.push_back(fill_char);
        }
        s.append(pfs::to_string(i3));
    }

    static inline void append_prefixed4 (string & s, string::value_type fill_char, int i4)
    {
        if (i4 >= 0) {
            if (i4 < 1000) s.push_back(fill_char);
            if (i4 < 100) s.push_back(fill_char);
            if (i4 < 10) s.push_back(fill_char);
        }
        s.append(pfs::to_string(i4));
    }
};

/**
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
inline string to_string (date const & d, string const & format)
{
    return d.to_string(format);
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
inline string to_string (date const & d)
{
    return d.to_string(string("%F")); // equivalent to %H:%M:%S
}

pfs::date current_date ();

} // pfs
