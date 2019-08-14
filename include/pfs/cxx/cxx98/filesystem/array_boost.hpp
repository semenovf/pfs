#pragma once
#include <boost/array.hpp>

namespace pfs {

template <typename T, size_t N>
class array : public ::boost::array<T, N>
{};

// TODO Complete implementation

} // namespace pfs
