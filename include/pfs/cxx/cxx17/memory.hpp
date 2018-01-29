#ifndef __PFS_CXX17_MEMORY_HPP__
#define __PFS_CXX17_MEMORY_HPP__

#include <pfs/cxx/cxx11/memory.hpp>

namespace pfs {

template<typename T, typename U>
inline shared_ptr<T> reinterpret_pointer_cast (shared_ptr<U> const & r)
{
    return std::reinterpret_pointer_cast<T,U>(r);
}

} // pfs

#endif /* __PFS_CXX17_MEMORY_HPP__ */

