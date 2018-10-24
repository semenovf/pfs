#pragma once
#include <limits>

namespace pfs {

template <typename T>
struct numeric_limits : public std::numeric_limits<T>
{};

}
