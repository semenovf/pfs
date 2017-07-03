/* 
 * File:   memory.hpp
 * Author: wladt
 *
 * Created on April 25, 2017, 8:13 AM
 */

#ifndef __PFS_CXX98_MEMORY_HPP__
#define __PFS_CXX98_MEMORY_HPP__

#include <pfs/cxxlang.hpp>
#include "shared_ptr.hpp"
#include "unique_ptr.hpp"

namespace pfs {

template< class T >
pfs_constexpr T * addressof (T & arg)
{
    return reinterpret_cast<T *>(
        & const_cast<char &>(reinterpret_cast<const volatile char &>(arg)));
}

} // pfs

#endif /* __PFS_CXX98_MEMORY_HPP__ */

