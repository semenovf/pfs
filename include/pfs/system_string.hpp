
/* 
 * File:   string.hpp
 * Author: wladt
 *
 * Created on July 4, 2017, 9:53 AM
 */

#ifndef __PFS_SYSTEM_STRING_HPP__
#define __PFS_SYSTEM_STRING_HPP__

#include <pfs/compiler.hpp>
#include <pfs/string.hpp>

#define PFS_TRAITS_STDCXX_STRING  1
#define PFS_TRAITS_STDCXX_WSTRING 2
#define PFS_TRAITS_QT_STRING      3

#ifndef PFS_SYSTEM_STRING

#   ifdef QT_CORE_LIB
#       define PFS_SYSTEM_STRING PFS_TRAITS_QT_STRING
#   else // ! QT_CORE_LIB
#       if PFS_CC_MSC
#           define PFS_SYSTEM_STRING PFS_TRAITS_STDCXX_WSTRING
#       else
#           define PFS_SYSTEM_STRING PFS_TRAITS_STDCXX_STRING
#       endif
#   endif // QT_CORE_LIB

#endif // PFS_SYSTEM_STRING

#if PFS_SYSTEM_STRING == PFS_TRAITS_STDCXX_STRING

#   include <pfs/traits/stdcxx/string.hpp>
    namespace pfs {

        typedef string<traits::stdcxx::string>                system_string;
        typedef string<traits::stdcxx::string_reference>      system_string_reference;

    } // pfs        

#   elif PFS_SYSTEM_STRING == PFS_TRAITS_STDCXX_WSTRING
#   include <pfs/traits/stdcxx/string.hpp>
    namespace pfs {

    typedef string<traits::stdcxx::wstring>           system_string;
    typedef string<traits::stdcxx::wstring_reference> system_string_reference;

    } // pfs        
#   elif PFS_SYSTEM_STRING == PFS_TRAITS_QT_STRING

#   include <pfs/traits/qt/string.hpp>
    namespace pfs {

        typedef string<traits::qt::string>                system_string;
        typedef string<traits::qt::string_reference>      system_string_reference;

    } // pfs        

#   else
#       error "Unknown string implementation for system_string, see valid values in pfs/system_string.hpp"
#   endif

#endif /* __PFS_SYSTEM_STRING_HPP__ */

