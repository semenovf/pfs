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

string time::to_string (string const & format) const
{
    string r;

    string::const_iterator p = format.cbegin();
    string::const_iterator end = format.cend();

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
                    append_prefixed2(r, '0', hour());
                    break;

                case 'I':
                    append_prefixed2(r, '0', hour() % 12);
                    break;

                case 'k':
                    append_prefixed2(r, ' ', hour());
                    break;

                case 'l':
                    append_prefixed2(r, ' ', hour() % 12);
                    break;

                case 'M':
                    append_prefixed2(r, '0', minute());
                    break;

                case 'q':
                    r.append(pfs::to_string(millis()));
                    break;

                case 'Q':
                    append_prefixed3(r, '0', millis());
                    break;

                case 'S':
                    append_prefixed2(r, '0', second());
                    break;

                case 'R':
                    append_prefixed2(r, '0', hour());
                    r.push_back(':');
                    append_prefixed2(r, '0', minute());
                    break;

                case 'T':
                    append_prefixed2(r, '0', hour());
                    r.push_back(':');
                    append_prefixed2(r, '0', minute());
                    r.push_back(':');
                    append_prefixed2(r, '0', second());
                    break;

                case 'J':
                    append_prefixed2(r, '0', hour());
                    r.push_back(':');
                    append_prefixed2(r, '0', minute());
                    r.push_back(':');
                    append_prefixed2(r, '0', second());
                    r.push_back('.');
                    append_prefixed3(r, '0', millis());
                    break;

                case 'p':
                    r.append(hour() < 12 ? string("AM") : string("PM"));
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


} // pfs



