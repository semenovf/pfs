#ifndef __PFS_CXX11_RATIO_HPP__
#define __PFS_CXX11_RATIO_HPP__

#include <chrono>

namespace pfs {

template <intmax_t Num, intmax_t Denom = 1>
using ratio = std::ratio<Num, Denom>;

template <typename R1, typename R2>
using ratio_equal  = std::ratio_equal<R1, R2>;

template <typename R1, typename R2>
using ratio_not_equal  = std::ratio_not_equal<R1, R2>;

template <typename R1, typename R2>
using ratio_less  = std::ratio_less<R1, R2>;

template <typename R1, typename R2>
using ratio_less_equal  = std::ratio_less_equal<R1, R2>;

template <typename R1, typename R2>
using ratio_greater  = std::ratio_greater<R1, R2>;

template <typename R1, typename R2>
using ratio_greater_equal  = std::ratio_greater_equal<R1, R2>;

template <typename R1, typename R2>
using ratio_add = std::ratio_add<R1, R2>;

template <typename R1, typename R2>
using ratio_subtract = std::ratio_subtract<R1, R2>;

template <typename R1, typename R2>
using ratio_multiply = std::ratio_multiply<R1, R2>;

template <typename R1, typename R2>
using ratio_divide = std::ratio_divide<R1, R2>;

} // pfs

#endif /* __PFS_CXX11_RATIO_HPP__ */

