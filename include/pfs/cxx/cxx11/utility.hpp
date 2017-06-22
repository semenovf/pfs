/* 
 * File:   utility.hpp
 * Author: wladt
 *
 * Created on January 17, 2017, 2:20 PM
 */

#ifndef __PFS_CXX_CXX11_UTILITY_HPP__
#define __PFS_CXX_CXX11_UTILITY_HPP__

#include <utility>

namespace pfs {

template <typename T>
inline void swap (T & a, T & b)
{
    std::swap(a, b);
}

template <typename T, size_t N>
inline void swap (T (& a)[N], T (&b)[N])
{
    std::swap(a, b);
}

template <typename T1, typename T2>
using pair = std::pair<T1, T2>;

}

#endif /* __PFS_CXX_CXX11_UTILITY_HPP__ */

