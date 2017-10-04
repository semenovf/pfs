
/* 
 * File:   traits.hpp
 * Author: wladt
 *
 * Created on October 4, 2017, 2:47 PM
 */

#ifndef __PFS_UNICODE_TRAITS_HPP__
#define __PFS_UNICODE_TRAITS_HPP__

#include <pfs/exception.hpp>

namespace pfs {
namespace unicode {

struct except_broken_sequence 
{
    void operator () () const { throw pfs::runtime_error("broken utf-8 sequence"); }
};

struct ignore_broken_sequence 
{
    void operator () () const {}
};

}} // pfs::unicode

#endif /* __PFS_UNICODE_TRAITS_HPP__ */

