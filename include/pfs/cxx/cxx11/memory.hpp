/* 
 * File:   memory.hpp
 * Author: wladt
 *
 * Created on April 25, 2017, 8:16 AM
 */

#ifndef __PFS_CXX11_MEMORY_HPP__
#define __PFS_CXX11_MEMORY_HPP__

#include <pfs/cxxlang.hpp>

namespace pfs {

template< class T >
inline pfs_constexpr T * addressof (T & arg)
{
    return std::addressof(arg);
}

} // pfs

#endif /* __PFS_CXX11_MEMORY_HPP__ */

