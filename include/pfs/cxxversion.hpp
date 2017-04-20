/* 
 * File:   cxxversion.hpp
 * Author: wladt
 *
 * Created on September 6, 2016, 9:53 AM
 */

#ifndef __PFS_CXXVERSION_HPP__
#define __PFS_CXXVERSION_HPP__

#if defined(__cplusplus)

#   define __PFS_CXX98__ 1998
#   define __PFS_CXX03__ 2003
#   define __PFS_CXX11__ 2011
#   define __PFS_CXX14__ 2014
#   define __PFS_CXX17__ 2017

#   if __cplusplus == 199711L
#       define __PFS_CXX_VERSION__  __PFS_CXX98__
#       define __PFS_CXX_HPREFIX__  cxx98
#   elif __cplusplus == 201103L
#       define __PFS_CXX_VERSION__  __PFS_CXX11__
#       define __PFS_CXX_HPREFIX__  cxx11
#   elif __cplusplus == 201402L
#       define __PFS_CXX_VERSION__  __PFS_CXX14__
#       define __PFS_CXX_HPREFIX__  cxx14
#   elif __cplusplus > 201402L
#       define __PFS_CXX_VERSION__  __PFS_CXX17__
#       define __PFS_CXX_HPREFIX__  cxx17
#   else
#       error "Unsupported C++ version"
#   endif
#else
#   error "Expected C++"
#endif

#define PFS_CXX_X(x) x
#define PFS_CXX_HEADER(x) <pfs/cxx/__PFS_CXX_HPREFIX__/PFS_CXX_X(x).hpp>

#endif /* __PFS_CXXVERSION_HPP__ */

