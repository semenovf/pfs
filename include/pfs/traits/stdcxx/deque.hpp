/* 
 * File:   deque.hpp
 * Author: wladt
 *
 * Created on May 08, 2017
 */

#ifndef __PFS_TRAITS_STDCXX_DEQUE_HPP__
#define __PFS_TRAITS_STDCXX_DEQUE_HPP__

#include <deque>
#include <pfs/traits/stdcxx/container.hpp>

namespace pfs {
namespace traits {
namespace stdcxx {

template <typename T>
struct deque_wrapper
{
    typedef std::deque<T> type;
};

template <typename T>
class deque : public container<T, deque_wrapper>
{
    typedef container<T, deque_wrapper> base_class;
    
public:
    typedef typename base_class::native_reference native_reference;

public:
    deque (native_reference rhs)
        : base_class(rhs)
    {}
};

}}} // pfs::traits::stdcxx


#endif /* __PFS_TRAITS_STDCXX_DEQUE_HPP__ */

