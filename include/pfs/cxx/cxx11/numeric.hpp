#ifndef __PFS_CXX_CXX11_NUMERIC_HPP__
#define __PFS_CXX_CXX11_NUMERIC_HPP__

#include <numeric>

namespace pfs {

template <typename ForwardIterator, typename T>
using iota = std::iota<ForwardIterator, T>;

} // pfs


#endif /* __PFS_CXX_CXX11_NUMERIC_HPP__ */

