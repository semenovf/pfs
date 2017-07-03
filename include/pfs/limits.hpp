/* 
 * File:   limits.hpp
 * Author: wladt
 *
 * Created on February 18, 2017, 4:09 PM
 */

#ifndef __PFS_LIMITS_HPP__
#define __PFS_LIMITS_HPP__

#include <pfs/assert.hpp>

#include <pfs/cxxversion.hpp>
#include PFS_CXX_HEADER(limits)

namespace pfs {

template <typename T1, typename T2>
inline T1 integral_cast_check (T2 x)
{
    PFS_ASSERT(static_cast<T1>(x) >= numeric_limits<T1>::min()
            && static_cast<T1>(x) <= numeric_limits<T1>::max());
    return static_cast<T1>(x);
}

} // pfs

#endif /* __PFS_LIMITS_HPP__ */

