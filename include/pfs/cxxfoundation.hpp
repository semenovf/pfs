/* 
 * File:   cxxfoundation.hpp
 * Author: wladt
 *
 * Created on February 1, 2017, 1:48 PM
 */

#ifndef __PFS_CXXFOUNDATION_HPP__
#define __PFS_CXXFOUNDATION_HPP__

#ifndef PFS_CXX_USE_FOUNDATION
// Valid foundations: stdcxx, qt, boost, gtk
#   define PFS_CXX_USE_FOUNDATION stdcxx
#endif

#define PFS_CXX_FOUNDATION_X(x) x
#define PFS_CXX_FOUNDATION(x) <pfs/foundation/cxx/PFS_CXX_USE_FOUNDATION/PFS_CXX_FOUNDATION_X(x).hpp>

#endif /* __PFS_CXXFOUNDATION_HPP__ */

