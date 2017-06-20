/*
 * unique_ptr.hpp
 *
 *  Created on: Oct 23, 2016
 *      Author: wladt
 */

#ifndef __PFS_CXX14_UNIQUE_PTR_HPP__
#define __PFS_CXX14_UNIQUE_PTR_HPP__

#include <memory>

namespace pfs {

template <typename T, typename Deleter = std::default_delete<T> >
using unique_ptr = std::unique_ptr<T, Deleter>;

} // pfs

#endif /* __PFS_CXX14_UNIQUE_PTR_HPP__ */
