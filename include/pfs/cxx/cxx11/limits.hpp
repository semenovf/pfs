/* 
 * File:   limits.hpp
 * Author: wladt
 *
 * Created on February 18, 2017, 4:17 PM
 */

#ifndef __PFS_CXX11_LIMITS_HPP__
#define __PFS_CXX11_LIMITS_HPP__

#include <limits>

namespace pfs {

template <typename T>
using numeric_limits = std::numeric_limits<T>;

} // pfs

#endif /* __PFS_CXX11_LIMITS_HPP__ */

