
/* 
 * File:   string.hpp
 * Author: wladt
 *
 * Created on July 4, 2017, 9:53 AM
 */

#ifndef __PFS_STRING_HPP__
#define __PFS_STRING_HPP__

#include <pfs/compiler.hpp>
#include <pfs/traits/string.hpp>
#include <pfs/traits/stdcxx/string.hpp>

namespace pfs {

#if PFS_CC_MSC
    typedef traits::string<traits::stdcxx::wstring>           system_string;
    typedef traits::string<traits::stdcxx::wstring_reference> system_string_reference;
#else
    typedef traits::string<traits::stdcxx::string>           system_string;
    typedef traits::string<traits::stdcxx::string_reference> system_string_reference;
#endif

} // pfs

#endif /* __PFS_STRING_HPP__ */

