/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   string.hpp
 * Author: wladt
 *
 * Created on February 1, 2017, 2:19 PM
 */

#ifndef __PFS_STRING_HPP__
#define __PFS_STRING_HPP__

#include <pfs/cxxfoundation.hpp>
#include PFS_CXX_FOUNDATION(string)

#ifdef Tr
#   error "Tr(x) macro aleady defined"
#endif

#define Tr(x) x

namespace pfs {

#if PFS_CXX_USE_FOUNDATION == PFS_CXX_FOUNDATION_STDCXX

    typedef traits::string<foundation::stdcxx, char>    string;
    typedef traits::c_str<foundation::stdcxx, char>     c_str;
    typedef traits::string<foundation::stdcxx, wchar_t> wstring;
    typedef traits::c_str<foundation::stdcxx, wchar_t>  c_wstr;
   
#elif PFS_CXX_USE_FOUNDATION == PFS_CXX_FOUNDATION_QT

    typedef traits::string<foundation::qt, QChar>  string;
    typedef traits::c_str<foundation::qt, QChar>   c_str;
    
#elif PFS_CXX_USE_FOUNDATION == PFS_CXX_FOUNDATION_BOOST
#   error "Need to implement BOOST-specific traits for string"
#elif PFS_CXX_USE_FOUNDATION == PFS_CXX_FOUNDATION_GTK
#   error "Need to implement GTK-specific traits for string"
#endif

} // pfs

#endif /* __PFS_STRING_HPP__ */
