/*
 * time.hpp
 *
 *  Created on: Sep 13, 2013
 *      Author: wladt
 */

#ifndef __PFS_TIME_HPP__
#define __PFS_TIME_HPP__

#include <pfs/types.hpp>
#include <pfs/string.hpp>
#include <pfs/lexical_cast.hpp>

namespace pfs {

class time
{
	int32_t _millis; // milliseconds from midnight

public:
	static const int NULL_TIME          = -1;
	static const int SECONDS_PER_MINUTE = 60;
	static const int SECONDS_PER_HOUR   = 3600;
	static const int SECONDS_PER_DAY    = 86400;
    static const int MILLIS_PER_MINUTE  = 60000;
    static const int MILLIS_PER_HOUR    = 3600000;
    static const int MILLIS_PER_DAY     = 86400000;

public:
	time ()
		: _millis(NULL_TIME)
	{}

	time (int hour, int min, int sec = 0, int millis = 0)
	{
		set_time(hour, min, sec, millis);
	}

	int32_t millis_from_midnight () const
	{
		return _millis;
	}

	time add_millis  (int millis) const;
	time add_seconds (int secs) const { return add_millis(secs * 1000); }

	int	hour () const
	{
		return valid() ? _millis / MILLIS_PER_HOUR : NULL_TIME;
	}

	int	minute () const
	{
		return valid() ? (_millis % MILLIS_PER_HOUR) / MILLIS_PER_MINUTE : NULL_TIME;
	}

	int	second () const
	{
		return valid() ? (_millis / 1000) % SECONDS_PER_MINUTE : NULL_TIME;
	}

	int	millis () const
	{
		return valid() ? _millis % 1000 : NULL_TIME;
	}

	bool valid () const
	{
		return _millis > NULL_TIME && _millis < MILLIS_PER_DAY;
	}

	int	millis_to (const time & t) const
	{
		return valid() && t.valid()
			? t._millis - _millis
			: 0;
	}

	int	seconds_to (const time & t) const
	{
	    return valid() &&  t.valid()
			? int(t._millis / 1000) - int(_millis / 1000)
			: 0;
	}

	bool set_time (int hour, int min, int sec, int millis = 0);

	bool operator == (const time & t) const
	{
		return _millis == t._millis;
	}

	bool operator != (const time & t) const
	{
		return _millis != t._millis;
	}

	bool operator  < (const time & t) const
	{
		return _millis  < t._millis;
	}

	bool operator <= (const time & t) const
	{
		return _millis <= t._millis;
	}

	bool operator  > (const time & t) const
	{
		return _millis  > t._millis;
	}

	bool operator >= (const time & t) const
	{
		return _millis >= t._millis;
	}

	static bool valid (int hour, int min, int sec, int millis = 0)
	{
		return static_cast<unsigned int>(hour) < 24
				&& static_cast<unsigned int>(min) < 60
				&& static_cast<unsigned int>(sec) < 60
				&& static_cast<unsigned int>(millis) < 1000;
	}

	static int millis (int hour, int min, int sec, int millis = 0)
	{
		return MILLIS_PER_HOUR * hour + MILLIS_PER_MINUTE * min + 1000 * sec + millis;
	}
};


namespace details {
namespace time {

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

}}
/**
 *
 * @param format
 * @return
 *
 * %n     A newline character.
 * %t     A tab character.
 * %H     The hour as a decimal number using a 24-hour clock (range 00 to 23).
 * %I     The hour as a decimal number using a 12-hour clock (range 01 to 12).
 * %k     The hour (24-hour clock) as a decimal number (range 0 to 23); single digits are preceded by a blank.
 * %l     The hour (12-hour clock) as a decimal number (range 1 to 12); single digits are preceded by a blank.
 * %M     The minute as a decimal number (range 00 to 59).
 * %q     The milliseconds (range 000 to 999).
 * %Q     The milliseconds (range 0 to 999).
 * %S     The second as a decimal number (range 00 to 60).
 * %R     The time in 24-hour notation (%H:%M).
 * %T     The time in 24-hour notation (%H:%M:%S).
 * %J     The time in 24-hour notation with milliseconds part (%H:%M:%S.%Q).
 * %p     Either "AM" or "PM" according to the given time value, or the corresponding strings for the current locale.  Noon is treated as "PM" and midnight as "AM".
 */
template <typename StringT>
StringT to_string (time const & t, StringT const & format)
{
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
                    
				case 'H':
					details::time::append_prefixed2(r, '0', t.hour());
					break;
                    
				case 'I':
					details::time::append_prefixed2(r, '0', t.hour() % 12);
					break;
                    
				case 'k':
					details::time::append_prefixed2(r, ' ', t.hour());
					break;
                    
				case 'l':
					details::time::append_prefixed2(r, ' ', t.hour() % 12);
					break;
                    
				case 'M':
					details::time::append_prefixed2(r, '0', t.minute());
					break;
                    
				case 'q':
					r.append(to_string<StringT>(t.millis()));
					break;
                    
				case 'Q':
					details::time::append_prefixed3(r, '0', t.millis());
					break;
                    
				case 'S':
					details::time::append_prefixed2(r, '0', t.second());
					break;
                    
				case 'R':
					details::time::append_prefixed2(r, '0', t.hour());
					r.push_back(':');
					details::time::append_prefixed2(r, '0', t.minute());
					break;
                    
				case 'T':
					details::time::append_prefixed2(r, '0', t.hour());
					r.push_back(':');
					details::time::append_prefixed2(r, '0', t.minute());
					r.push_back(':');
					details::time::append_prefixed2(r, '0', t.second());
					break;
                    
				case 'J':
					details::time::append_prefixed2(r, '0', t.hour());
					r.push_back(':');
					details::time::append_prefixed2(r, '0', t.minute());
					r.push_back(':');
					details::time::append_prefixed2(r, '0', t.second());
					r.push_back('.');
					details::time::append_prefixed3(r, '0', t.millis());
					break;
                    
				case 'p':
					r.append(t.hour() < 12 ? StringT("AM") : StringT("PM"));
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
 *
 * @param t
 * @return
 */
template <typename StringT>
inline StringT to_string (time const & t)
{
	return to_string<StringT>(t, StringT("%T"));
}

pfs::time current_time ();

} // pfs

#endif /* __PFS_TIME_HPP__ */
