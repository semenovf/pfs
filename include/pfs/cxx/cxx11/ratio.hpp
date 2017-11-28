#ifndef __PFS_CXX11_RATIO_HPP__
#define __PFS_CXX11_RATIO_HPP__

#include <chrono>

namespace pfs {

template <intmax_t Num, intmax_t Denom = 1>
using ratio = std::ratio<Num, Denom>;

} // pfs

#endif /* __PFS_CXX11_RATIO_HPP__ */

