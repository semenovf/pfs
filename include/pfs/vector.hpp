/* 
 * File:   vector.hpp
 * Author: wladt
 *
 * Created on February 1, 2017, 2:23 PM
 */

#ifndef __PFS_VECTOR_HPP__
#define __PFS_VECTOR_HPP__

#include <pfs/cxxfoundation.hpp>
#include PFS_CXX_FOUNDATION(vector)

namespace pfs {

#if defined(__PFS_FOUNDATION_STDCXX_VECTOR_HPP__)

#if __cplusplus >= 201103L
    template <typename T>
    using vector = traits::vector<foundation::stdcxx, T>;
#endif    
   
#elif defined(__PFS_FOUNDATION_QT_VECTOR_HPP__)

#if __cplusplus >= 201103L
    template <typename T>
    using vector = traits::vector<foundation::qt, T>;
#endif
    
#elif defined(__PFS_FOUNDATION_BOOST_VECTOR_HPP__)
#   error "Need to implement BOOST-specific traits for vector"
#elif defined(__PFS_FOUNDATION_GTK_VECTOR_HPP__)
#   error "Need to implement GTK-specific traits for vector"
#endif

} // pfs


#endif /* __PFS_VECTOR_HPP__ */

