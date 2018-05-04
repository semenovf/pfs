#pragma once
#include <functional>

using namespace std::placeholders;

namespace pfs {

template <typename T>
using reference_wrapper = std::reference_wrapper<T>;

template <typename T>
inline reference_wrapper<T> ref (T & t) noexcept
{
    return std::ref<T>(t);
}

template <typename T>
reference_wrapper<const T> cref (T const & t) noexcept
{
    return std::cref<T>(t);
}

///////////////////////////////////////////////////////////////////////////////
// bind                                                                      //
///////////////////////////////////////////////////////////////////////////////

using std::bind;

} // pfs
