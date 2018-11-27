#include <sstream>
#include <pfs/test.hpp>
#include <pfs/string.hpp>
#include <pfs/date.hpp>
#include "compare.hpp"

typedef pfs::string string_t;
typedef std::stringstream stringstream_type;

void test_min_max (void)
{
    TEST_OK(pfs::date::MIN_JULIAN_DAY
            == pfs::date::julian_day(pfs::numeric_limits<int>::min(), 1, 1));

    TEST_OK(pfs::date::MAX_JULIAN_DAY
            == pfs::date::julian_day(pfs::numeric_limits<int>::max(), 12, 31));

    TEST_OK(pfs::date::EPOCH_JULIAN_DAY == pfs::date::julian_day(1970, 1, 1));
}

void test_jd (void)
{
    int y, m, d;

    TEST_OK(!pfs::date(0, 0, 0).valid());
    TEST_OK(pfs::date::julian_day(1, 1, 1) == 1721424);

    pfs::date::from_julian_day(1721424, & y, & m, & d);
    TEST_OK2(y == 1 && m == 1 && d == 1, "JD(1721424) is 01.01.0001");

    // Changes from Julian to Gregorian between October 4/15, 1582.
    TEST_OK(pfs::date::julian_day(1582, 10, 4) == 2299160);
    TEST_OK(pfs::date::julian_day(1582, 10, 5) == pfs::date::NULL_JULIAN_DAY); // invalid
    TEST_OK(pfs::date::julian_day(1582, 10, 6) == pfs::date::NULL_JULIAN_DAY); // invalid
    TEST_OK(pfs::date::julian_day(1582, 10, 7) == pfs::date::NULL_JULIAN_DAY); // invalid
    TEST_OK(pfs::date::julian_day(1582, 10, 8) == pfs::date::NULL_JULIAN_DAY); // invalid
    TEST_OK(pfs::date::julian_day(1582, 10, 9) == pfs::date::NULL_JULIAN_DAY); // invalid
    TEST_OK(pfs::date::julian_day(1582, 10, 10) == pfs::date::NULL_JULIAN_DAY); // invalid
    TEST_OK(pfs::date::julian_day(1582, 10, 11) == pfs::date::NULL_JULIAN_DAY); // invalid
    TEST_OK(pfs::date::julian_day(1582, 10, 12) == pfs::date::NULL_JULIAN_DAY); // invalid
    TEST_OK(pfs::date::julian_day(1582, 10, 13) == pfs::date::NULL_JULIAN_DAY); // invalid
    TEST_OK(pfs::date::julian_day(1582, 10, 14) == pfs::date::NULL_JULIAN_DAY); // invalid
    TEST_OK(pfs::date::julian_day(1582, 10, 15) == 2299161);

    TEST_OK(pfs::date::julian_day(1582, 10, 3) == 2299159);
    TEST_OK(pfs::date::julian_day(1582, 10, 16) == 2299162);

    TEST_OK(pfs::date::julian_day(1600, 1, 1) == 2305448);

    TEST_OK(pfs::date::julian_day(2013, 9, 13) == 2456549);
    TEST_OK(pfs::date::julian_day(-2013, 9, 13) == 986431);

    TEST_OK(pfs::date(2013, 9, 13).days_to(pfs::date(2013, 9, 13)) == 0);
    TEST_OK(pfs::date(2013, 9, 13).days_to(pfs::date(2013, 9, 12)) == -1);
    TEST_OK(pfs::date(2013, 9, 13).days_to(pfs::date(2013, 6, 5)) == -100);
    TEST_OK(pfs::date(2013, 9, 13).days_to(pfs::date(2010, 12, 18)) == -1000);
    TEST_OK(pfs::date(2013, 9, 13).days_to(pfs::date(1986, 4, 28)) == -10000);
    TEST_OK(pfs::date(2013, 9, 13).days_to(pfs::date(1739, 11, 29)) == -100000);
    TEST_OK(pfs::date(2013, 9, 13).days_to(pfs::date(-726, 10, 25)) == -1000000);
    TEST_OK(pfs::date(2013, 9, 13).days_to(pfs::date(-25366, 2, 27)) == -10000000);

    pfs::date d1(2013, 9, 13);
    d1 = d1.add_days(-10000000);
    //pfs::date::fromJulianDay(d1.julianDay(), & y, & m, & d);
    //std::cout << "13.09.2013 - 10000000 days is " << d << "." << m << "." << y << std::endl;
    //long_t jd = pfs::date::julianDay(y, m, d);
    //TEST_OK(jd == d1.julianDay());

    pfs::date d2(-25366, 2, 27);
    TEST_OK2(d1.valid(), "13.9.2013 - 10000000 is a valid date and it is 27.2.25366 BC");
    TEST_OK2(d2.valid(), "27.2.25366 BC is valid date");
    TEST_OK2(!pfs::date(-25365, 2, 30).valid(), "30.2.25365 BC is invalid date");

    // 4.10.1582 + 1 day = 15.10.1582
    TEST_OK(pfs::date(1582, 10, 4).add_days(1) == pfs::date(1582, 10, 15));
    TEST_OK(pfs::date(1582, 10, 15).add_days(-1) == pfs::date(1582, 10, 4));
    //std::cout << d1.julianDay() << " == " << d2.julianDay()  << std::endl;
}

// From Qt's QDate class reference

void test_valid (void)
{
    TEST_OK2(pfs::date::valid(2002, 5, 17), "17.05.2002 is a valid date"); // true
    TEST_OK2(!pfs::date::valid(2002, 2, 30), "30.02.2002 is not a valid date: Feb 30 does not exist");
    TEST_OK2(pfs::date::valid(2004, 2, 29), "29.02.2004 is a valid date: 2004 is a leap year");
    TEST_OK2(pfs::date::valid(2000, 2, 29), "29.02.2004 is a valid date: 2000 is a leap year");
    TEST_OK2(!pfs::date::valid(2006, 2, 29), "29.02.2006 is not a valid date: 2006 is not a leap year");
    TEST_OK2(!pfs::date::valid(2100, 2, 29), "29.02.2100 is not a valid date: 2100 is not a leap year");
    TEST_OK2(pfs::date::valid(1202, 6, 6), "06.06.1202 is a valid date: even though 1202 is pre-Gregorian");

    int year;
    int month;
    int day;
    intmax_t jd = intmax_t(2456575);
    pfs::date::from_julian_day(jd, & year, & month, & day);

    stringstream_type ss;
    ss << "9.10.2013 == " << jd;
    TEST_OK2(pfs::date::julian_day(2013, 10, 9) == jd, ss.str().c_str());

    ss.clear();
    ss << jd << " (9.10.2013) == " << day << '.' << month << '.' << year;
    TEST_OK2(jd == pfs::date::julian_day(year, month, day), ss.str().c_str());
}

void test_periods (void)
{
    pfs::date d1(1995, 5, 17); // May 17, 1995
    pfs::date d2(1995, 5, 20); // May 20, 1995
    TEST_OK2(d1.days_to(d2) == 3, "Days bitween 17.05.1995 and 20.05.1995 is 3");
    TEST_OK2(d2.days_to(d1) == -3, "Days bitween 20.05.1995 and 17.05.1995 is -3");
}

void test_stringify ()
{
    TEST_OK(pfs::to_string(pfs::date(2013, 11, 28), string_t("%Y-%m-%d")) == string_t("2013-11-28"));
    TEST_OK(pfs::to_string(pfs::date(1, 2, 3), string_t("%Y-%m-%d")) == string_t("0001-02-03"));
    TEST_OK(pfs::to_string(pfs::date(2013, 11, 28)) == string_t("2013-11-28"));
    TEST_OK(pfs::to_string(pfs::date(1, 2, 3)) == string_t("0001-02-03"));
}

int main ()
{
    BEGIN_TESTS(54);

    test_min_max();
    test_jd();
    test_valid();
    test_periods();
    test_stringify();

    TEST_OK(!pfs::date::is_leap_year(2015));
    TEST_OK(pfs::date::is_leap_year(2016));
    TEST_OK(!pfs::date::is_leap_year(2017));

    test_compare();

    return END_TESTS;
}
