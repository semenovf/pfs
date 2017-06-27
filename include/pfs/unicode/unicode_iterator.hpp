/* 
 * File:   unicode_iterator.hpp
 * Author: wladt
 *
 * Created on June 27, 2017, 11:13 AM
 */

#ifndef __PFS_UNICODE_UNICODE_ITERATOR_HPP__
#define __PFS_UNICODE_UNICODE_ITERATOR_HPP__

#include <string>
#include <pfs/unicode/utf8_iterator.hpp>
//#include <pfs/unicode/utf16_iterator.hpp>
//#include <pfs/unicode/utf32_iterator.hpp>

namespace pfs {
namespace unicode {

template <typename CodePointIter>
struct unicode_iterator_traits;

template <>
struct unicode_iterator_traits<unsigned char *>
{
    typedef utf8_iterator<unsigned char *> iterator;
};

template <>
struct unicode_iterator_traits<unsigned char const *>
{
    typedef utf8_iterator<unsigned char const *> iterator;
};

template <>
struct unicode_iterator_traits<char *>
{
    typedef utf8_iterator<char *> iterator;
};

template <>
struct unicode_iterator_traits<char const *>
{
    typedef utf8_iterator<char const *> iterator;
};

template <>
struct unicode_iterator_traits<std::string::iterator>
{
    typedef utf8_iterator<std::string::iterator> iterator;
};

template <>
struct unicode_iterator_traits<std::string::const_iterator>
{
    typedef utf8_iterator<std::string::const_iterator> iterator;
};

}}

#endif /* __PFS_UNICODE_UNICODE_ITERATOR_HPP__ */

