/* 
 * File:   set.hpp
 * Author: wladt
 *
 * Created on May 8, 2017
 */

#ifndef __PFS_TRAITS_STDCXX_MULTISET_HPP__
#define __PFS_TRAITS_STDCXX_MULTISET_HPP__

#include <set>
#include <pfs/traits/stdcxx/container.hpp>

namespace pfs {
namespace traits {
namespace stdcxx {

template <typename T>
struct multiset_wrapper
{
    typedef std::multiset<T> type;
};

template <typename T>
class multiset : public container<T, multiset_wrapper>
{
    typedef container<T, multiset_wrapper> base_class;
    
public:
    typedef typename base_class::native_reference native_reference;

public:
    multiset (native_reference rhs)
        : base_class(rhs)
    {}
};

}}} // pfs::traits::stdcxx

#endif /* __PFS_TRAITS_STDCXX_MULTISET_HPP__ */
