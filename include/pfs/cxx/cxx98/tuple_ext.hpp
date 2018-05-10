#pragma once

namespace pfs {

template <typename T>
inline size_t tuple_size_f (T const & t)
{
    return static_cast<size_t>(tuple_size<T>::value);
}

}
