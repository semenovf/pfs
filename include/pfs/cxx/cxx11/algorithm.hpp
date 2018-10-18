#pragma once
#include <algorithm>
#include <pfs/cxx/cxx98/algorithm.hpp>

// TODO Implement using STL (>= C++11) instead of C++98 implementation

namespace pfs
{

template <typename ForwardIt>
inline pfs::pair<ForwardIt,ForwardIt> minmax_element (ForwardIt first, ForwardIt last)
{
    std::pair<ForwardIt,ForwardIt> result = std::minmax_element(first, last);
    return pfs::make_pair(result.first, result.second);
}

template <typename ForwardIt, typename Compare>
inline pfs::pair<ForwardIt,ForwardIt> minmax_element (ForwardIt first, ForwardIt last, Compare comp)
{
    std::pair<ForwardIt,ForwardIt> result = std::minmax_element(first, last, comp);
    return pfs::make_pair(result.first, result.second);
}

} // namespace pfs
