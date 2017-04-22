/* 
 * File:   algorithm.hpp
 * Author: wladt
 *
 * Created on January 18, 2017, 1:18 PM
 */

#ifndef __PFS_CXX14_ALGORITHM_HPP__
#define __PFS_CXX14_ALGORITHM_HPP__

#include <algorithm>

namespace pfs {

template< class T >
constexpr T const & min (T const & a, T const & b )
{
    return std::min(a, b);
}

template< class T, class Compare >
constexpr T const & min (T const & a, T const & b, Compare comp)
{
    return std::min(a, b, comp);
}

template< class T >
constexpr T const & max (T const & a, T const & b )
{
    return std::max(a, b);
}

template< class T, class Compare >
constexpr T const & max (T const & a, T const & b, Compare comp)
{
    return std::max(a, b, comp);
}

}
#endif /* __PFS_CXX14_ALGORITHM_HPP__ */
