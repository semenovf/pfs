/* 
 * File:   functional.hpp
 * Author: wladt
 *
 * Created on May 2, 2017, 4:16 PM
 */

#ifndef __PFS_CXX_CXX11_FUNCTIONAL_HPP__
#define __PFS_CXX_CXX11_FUNCTIONAL_HPP__

#include <functional>

namespace pfs {

template <typename T>
using reference_wrapper = std::reference_wrapper<T>;

template <typename T>
using ref = std::ref<T>;

template <typename T>
using cref = std::cref<T>;

} // pfs

#endif /* __PFS_CXX_CXX11_FUNCTIONAL_HPP__ */

