/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   string_view.hpp
 * Author: wladt
 *
 * Created on January 26, 2017, 9:10 AM
 */

#ifndef __PFS_STRING_VIEW_HPP__
#define __PFS_STRING_VIEW_HPP__

#include <pfs/cxxfoundation.hpp>
#include PFS_CXX_FOUNDATION(string_view)

namespace pfs {

#if PFS_CXX_USE_FOUNDATION == PFS_CXX_FOUNDATION_STDCXX

    typedef traits::string_view<char>    string_view;
    typedef traits::string_view<wchar_t> wstring_view;
   
#elif PFS_CXX_USE_FOUNDATION == PFS_CXX_FOUNDATION_QT
    
    typedef traits::string_view<QChar>  string_view;
    
#elif PFS_CXX_USE_FOUNDATION == PFS_CXX_FOUNDATION_BOOST
#   error "Need to implement BOOST-specific traits for string_view"
#elif PFS_CXX_USE_FOUNDATION == PFS_CXX_FOUNDATION_GTK
#   error "Need to implement GTK-specific traits for string_view"
#endif

} // pfs

#endif /* __PFS_STRING_VIEW_HPP__ */

