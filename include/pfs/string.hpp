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

namespace pfs {

#if defined(__PFS_FOUNDATION_STDCXX_STRING_HPP__)

    typedef traits::string<foundation::stdcxx, char>    string;
    typedef traits::c_str<foundation::stdcxx, char>     c_str;
    typedef traits::string<foundation::stdcxx, wchar_t> wstring;
    typedef traits::c_str<foundation::stdcxx, wchar_t>  c_wstr;
   
#elif defined(__PFS_FOUNDATION_QT_STRING_HPP__)

    typedef traits::string<foundation::qt, QChar>  string;
    typedef traits::c_str<foundation::qt, QChar>   c_str;
   
#elif defined(__PFS_FOUNDATION_BOOST_STRING_HPP__)
#   error "Need to implement BOOST-specific traits for string"
#elif defined(__PFS_FOUNDATION_GTK_STRING_HPP__)
#   error "Need to implement GTK-specific traits for string"
#endif

} // pfs

#endif /* __PFS_STRING_HPP__ */
