#ifndef __PFS_CXX_CXX98_NUMERIC_HPP__
#define __PFS_CXX_CXX98_NUMERIC_HPP__

#include <numeric>

namespace pfs {

template <typename ForwardIterator, typename T>
void iota (ForwardIterator first, ForwardIterator last, T value)
{
    while (first != last)
        *first++ = value++;
}

} // pfs

#endif /* __PFS_CXX_CXX98_NUMERIC_HPP__ */

