#ifndef __PFS_CXX11_ARRAY_HPP__
#define __PFS_CXX11_ARRAY_HPP__

#include <array>

namespace pfs {

template <typename T, size_t N>
using array = std::array<T, N>;

} // pfs

#endif /* __PFS_CXX11_ARRAY_HPP__ */

