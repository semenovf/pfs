
/* 
 * File:   regex.hpp
 * Author: wladt
 *
 * Created on September 13, 2017, 1:51 PM
 */

#ifndef __PFS_CXX_CXX98_REGEX_HPP__
#define __PFS_CXX_CXX98_REGEX_HPP__

#if defined(PFS_CC_MSC)
#   error "Implement for Win32"

#elif defined(HAVE_BOOST_REGEX)
#   include "regex_boost.hpp"
#else
#	include "regex_custom.hpp"
#endif

#endif /* __PFS_CXX_CXX98_REGEX_HPP__ */

