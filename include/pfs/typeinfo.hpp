/*
 * typeinfo.hpp
 *
 *  Created on: Mar 24, 2015
 *      Author: wladt
 */

#ifndef __PFS_TYPEINFO_HPP__
#define __PFS_TYPEINFO_HPP__

#include <typeinfo>
#include <pfs/types.hpp>

namespace pfs {

template <typename T> inline const char * type_name () { return typeid(T).name(); }

template <> inline const char * type_name<char> () { return "char"; }
template <> inline const char * type_name<signed char> () { return "signed char"; }
template <> inline const char * type_name<unsigned char> () { return "unsigned char"; }
template <> inline const char * type_name<signed short int> () { return "signed short int"; }
template <> inline const char * type_name<unsigned short int> () { return "unsigned short int"; }
template <> inline const char * type_name<signed int> () { return "signed int"; }
template <> inline const char * type_name<unsigned int> () { return "unsigned int"; }
template <> inline const char * type_name<signed long int> () { return "signed long int"; }
template <> inline const char * type_name<unsigned long int> () { return "unsigned long int"; }

#ifdef PFS_HAVE_LONG_LONG
template <> inline const char * type_name<signed long long int> () { return "signed long long int"; }
template <> inline const char * type_name<unsigned long long int> () { return "unsigned long long int"; }
#endif

template <> inline const char * type_name<float> () { return "float"; }
template <> inline const char * type_name<double> () { return "double"; }
#ifdef PFS_HAVE_LONG_DOUBLE
template <> inline const char * type_name<long double> () { return "long double"; }
#endif

} // pfs

#endif /* __PFS_TYPEINFO_HPP__ */
