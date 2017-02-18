/* 
 * File:   limits.hpp
 * Author: wladt
 *
 * Created on February 18, 2017, 4:10 PM
 */

#ifndef __PFS_CXX_CXX98_LIMITS_HPP__
#define __PFS_CXX_CXX98_LIMITS_HPP__

#include <limits>

namespace pfs {

template <typename T>
struct numeric_limits : public std::numeric_limits<T>
{};

}

#endif /* __PFS_CXX_CXX98_LIMITS_HPP__ */

