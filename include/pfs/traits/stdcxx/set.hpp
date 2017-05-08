/* 
 * File:   set.hpp
 * Author: wladt
 *
 * Created on May 8, 2017
 */

#ifndef __PFS_TRAITS_STDCXX_SET_HPP__
#define __PFS_TRAITS_STDCXX_SET_HPP__

#include <set>
#include <pfs/traits/stdcxx/container.hpp>

namespace pfs {
namespace traits {
namespace stdcxx {

template <typename T>
struct set_wrapper
{
    typedef std::set<T> type;
};

template <typename T>
class set : public container<T, set_wrapper>
{
    typedef container<T, set_wrapper> base_class;
    
public:
    typedef typename base_class::native_reference native_reference;

public:
    set (native_reference rhs)
        : base_class(rhs)
    {}
};

}}} // pfs::traits::stdcxx

#endif /* __PFS_TRAITS_STDCXX_SET_HPP__ */
