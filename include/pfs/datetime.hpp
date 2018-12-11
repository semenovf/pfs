#pragma once
#include <pfs/types.hpp>
#include <pfs/compare.hpp>
#include <pfs/date.hpp>
#include <pfs/time.hpp>

namespace pfs {

class timezone
{
private:
    string   _tzname;
    long int _offset;

public:
    timezone ()
        : _tzname("XXXX")
        , _offset(0)
    {}

    timezone (string const & tzname, long int offset)
        : _tzname(tzname)
        , _offset(offset)
    {}

    timezone (timezone const & other)
        : _tzname(other._tzname)
        , _offset(other._offset)
    {}

    timezone & operator = (timezone const & other)
    {
        _tzname = other._tzname;
        _offset = other._offset;
        return *this;
    }

    string const & tzname () const
    {
        return _tzname;
    }

    long int offset () const
    {
        return _offset;
    }

    /**
     * @brief Converts UTC offset to string.
     * @param off UTC offset value.
     *
     * @return String in format '+hhmm' or '-hhmm' (that is, the hour and minute offset from UTC).
     */
    static string offset_to_string (long int off);

    string offset_to_string () const
    {
        return offset_to_string(_offset);
    }
};

class datetime : public compare_op<datetime>
{
    date _date;
    time _time;

public:
    datetime ()
        : _date()
        , _time()
    {}

    datetime (date const & d)
        : _date(d)
        , _time()
    {}

    datetime (date const & d, time const & t)
        : _date(d)
        , _time(t)
    {}

    datetime add_days (int ndays) const
    {
        return datetime(_date.add_days(ndays), _time);
    }

    datetime add_months (int nmonths) const
    {
        return datetime(_date.add_months(nmonths), _time);
    }

    datetime add_years (int nyears) const
    {
        return datetime(_date.add_years(nyears), _time);
    }

    datetime add_millis (intmax_t millis) const;

    datetime add_seconds (intmax_t secs) const
    {
        return add_millis(secs * 1000);
    }

    intmax_t days_to (datetime const & other) const
    {
        return _date.days_to(other._date);
    }

    intmax_t seconds_to (datetime const & other) const
    {
        return valid() && other.valid()
                ? _date.days_to(other._date) * time::SECONDS_PER_DAY + intmax_t(_time.seconds_to(other._time))
                : 0;
    }

    intmax_t millis_to (datetime const & other) const
    {

        return valid() && other.valid()
                ? _date.days_to(other._date) * time::MILLIS_PER_DAY
                        + intmax_t(_time.millis_to(other._time))
                : 0;
    }

    intmax_t millis_since_epoch () const
    {
        intmax_t jd = _date.julian_day() - date::EPOCH_JULIAN_DAY;
        return (jd * time::MILLIS_PER_DAY) + time(0, 0, 0).millis_to(_time);
    }

    date get_date () const
    {
        return _date;
    }

    time get_time () const
    {
        return _time;
    }

    void set_date (date const & d);

    void set_time (time const & t)
    {
        _time = t;
    }

    void set_millis_since_epoch (intmax_t millis);

    intmax_t to_intmax () const;

    bool valid () const
    {
        return _date.valid() && _time.valid();
    }

    bool operator == (datetime const & other) const
    {
        return _time == other._time
                && _date == other._date;
    }

    bool operator < (datetime const & other) const
    {
        return _date != other._date
                ? _date < other._date
                : _time < other._time;
    }

    static datetime from_millis_since_epoch (intmax_t millis)
    {
        datetime d;
        d.set_millis_since_epoch(millis);
        return d;
    }

    static datetime from_seconds_since_epoch (unsigned long int seconds)
    {
        datetime d;
        d.set_millis_since_epoch(seconds * 1000);
        return d;
    }

    string to_string (timezone const & tz, string const & format) const;
};

inline string to_string (datetime const & dt, timezone const & tz, string const & format)
{
    return dt.to_string(tz, format);
}

/**
 * @brief Converts date and time to string according to @a format.
 *
 * @param dt Date and time value.
 * @param format Format to convert @a dt.
 * @return String representation of @a dt.
 */
inline string to_string (datetime const & dt, string const & format)
{
    return dt.to_string(timezone(), format);
}

/**
 * @brief Converts date and time to string.
 *
 * @details The string format corresponds to the ISO 8601 specification,
 *          taking the form YYYY-MM-DDTHH:mm:ss, where YYYY is the year,
 *          MM is the month of the year (between 01 and 12),
 *          and DD is the day of the month between 01 and 31.
 *
 * @return The date and time as string.
 */
inline string to_string (datetime const & dt)
{
    string r = to_string(dt.get_date());
    r.push_back('T');
    r.append(to_string(dt.get_time()));
    return r;
}

datetime current_datetime ();
long int offset_utc ();
string timezone_name ();

inline timezone current_timezone ()
{
    return timezone(timezone_name(), offset_utc());
}

} // pfs
