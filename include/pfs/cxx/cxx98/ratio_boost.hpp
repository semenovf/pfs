
/* 
 * File:   ratio_boost.hpp
 * Author: wladt
 *
 * Created on September 13, 2017, 11:00 AM
 */

#ifndef __PFS_CXX_CXX98_RATIO_BOOST_HPP__
#define __PFS_CXX_CXX98_RATIO_BOOST_HPP__

#include <boost/ratio.hpp>

namespace pfs {

template <intmax_t Num, intmax_t Denom = 1>
class ratio : public ::boost::ratio<Num, Denom> 
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

