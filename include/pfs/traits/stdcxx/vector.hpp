/* 
 * File:   vector.hpp
 * Author: wladt
 *
 * Created on April 27, 2017, 7:17 PM
 */

#ifndef __PFS_TRAITS_STDCXX_VECTOR_HPP__
#define __PFS_TRAITS_STDCXX_VECTOR_HPP__

#include <vector>
#include <pfs/traits/stdcxx/container.hpp>

namespace pfs {
namespace traits {
namespace stdcxx {

template <typename T>
struct vector_wrapper
{
    typedef std::vector<T> type;
};

template <typename T>
class vector : public container<T, vector_wrapper>
{
    typedef container<T, vector_wrapper> base_class;
    
public:
    typedef typename base_class::native_reference native_reference;

public:
    vector (native_reference rhs)
        : base_class(rhs)
    {}
};

}}} // pfs::traits::stdcxx

#endif /* __PFS_TRAITS_STDCXX_VECTOR_HPP__ */

