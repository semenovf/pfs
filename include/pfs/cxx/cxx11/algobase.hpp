/* 
 * File:   algorithm.hpp
 * Author: wladt
 *
 * Created on January 18, 2017, 1:18 PM
 */

#ifndef __PFS_CXX11_ALGOBASE_HPP__
#define __PFS_CXX11_ALGOBASE_HPP__

#include <algorithm>

namespace pfs {

template <typename T>
using min = std::min<T>;

template <typename T, typename Compare>
using min = std::min<T, Comapre>;

template <typename T>
using max = std::max<T>;

template <typename T, typename Compare>
using max = std::max<T, Compare>;

}

#endif /* __PFS_CXX11_ALGOBASE_HPP__ */
