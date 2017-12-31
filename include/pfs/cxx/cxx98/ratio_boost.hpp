#ifndef __PFS_CXX_CXX98_RATIO_BOOST_HPP__
#define __PFS_CXX_CXX98_RATIO_BOOST_HPP__

#include <boost/ratio.hpp>

namespace pfs {

template <intmax_t Num, intmax_t Denom = 1>
class ratio : public ::boost::ratio<Num, Denom>
{};

template <typename R1, typename R2>
struct ratio_equal : public ::boost::ratio_equal<R1, R2>
{};

template <typename R1, typename R2>
struct ratio_not_equal : public ::boost::ratio_not_equal<R1, R2>
{};

template <typename R1, typename R2>
struct ratio_less : public ::boost::ratio_less<R1, R2>
{};

template <typename R1, typename R2>
struct ratio_less_equal : public ::boost::ratio_less_equal<R1, R2>
{};

template <typename R1, typename R2>
struct ratio_greater : public ::boost::ratio_greater<R1, R2>
{};

template <typename R1, typename R2>
struct ratio_greater_equal : public ::boost::ratio_greater_equal<R1, R2>
{};

template <typename R1, typename R2>
struct ratio_add : public ::boost::ratio_add<R1,R2>
{};

template <typename R1, typename R2>
struct ratio_subtract : public ::boost::ratio_subtract<R1,R2>
{};

template <typename R1, typename R2>
struct ratio_multiply : public ::boost::ratio_multiply<R1,R2>
{};

template <typename R1, typename R2>
struct ratio_divide : public ::boost::ratio_divide<R1,R2>
{};


typedef ratio<1,       1000000000000000000> atto;
typedef ratio<1,          1000000000000000> femto;
typedef ratio<1,             1000000000000> pico;
typedef ratio<1,                1000000000> nano;
typedef ratio<1,                   1000000> micro;
typedef ratio<1,                      1000> milli;
typedef ratio<1,                       100> centi;
typedef ratio<1,                        10> deci;
typedef ratio<                       10, 1> deca;
typedef ratio<                      100, 1> hecto;
typedef ratio<                     1000, 1> kilo;
typedef ratio<                  1000000, 1> mega;
typedef ratio<               1000000000, 1> giga;
typedef ratio<            1000000000000, 1> tera;
typedef ratio<         1000000000000000, 1> peta;
typedef ratio<      1000000000000000000, 1> exa;

} // pfs

#endif /* __PFS_CXX_CXX98_RATIO_BOOST_HPP__ */

