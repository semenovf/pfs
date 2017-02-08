/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   cxxfoundation.hpp
 * Author: wladt
 *
 * Created on February 1, 2017, 1:48 PM
 */

#ifndef __PFS_CXXFOUNDATION_HPP__
#define __PFS_CXXFOUNDATION_HPP__

#define PFS_CXX_FOUNDATION_STDCXX 1
#define PFS_CXX_FOUNDATION_QT     2  
#define PFS_CXX_FOUNDATION_BOOST  3
#define PFS_CXX_FOUNDATION_GTK    4

#ifndef PFS_CXX_USE_FOUNDATION
#   define PFS_CXX_USE_FOUNDATION PFS_CXX_FOUNDATION_STDCXX
#endif

#if PFS_CXX_USE_FOUNDATION == PFS_CXX_FOUNDATION_STDCXX
#   define __PFS_CXX_FOUNDATION_HPREFIX__  stdcxx
#elif PFS_CXX_USE_FOUNDATION == PFS_CXX_FOUNDATION_QT
#   define __PFS_CXX_FOUNDATION_HPREFIX__  qt
#elif PFS_CXX_USE_FOUNDATION == PFS_CXX_FOUNDATION_BOOST
#   define __PFS_CXX_FOUNDATION_HPREFIX__  boost
#elif PFS_CXX_USE_FOUNDATION == PFS_CXX_FOUNDATION_GTK
#   define __PFS_CXX_FOUNDATION_HPREFIX__  gtk
#else
#   error "Unsupported C++ preferred foundation (library, framework)"
#endif

#define PFS_CXX_FOUNDATION_X(x) x
#define PFS_CXX_FOUNDATION(x) <pfs/foundation/cxx/__PFS_CXX_FOUNDATION_HPREFIX__/PFS_CXX_FOUNDATION_X(x).hpp>

#endif /* __PFS_CXXFOUNDATION_HPP__ */

