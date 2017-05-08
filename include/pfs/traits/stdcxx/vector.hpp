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

#if __COMMENT__

#include <vector>
#include <pfs/traits/vector.hpp>

namespace stdcxx {

template <typename T>
struct vector
{
    typedef std::vector<T> type;
};

} // stdcxx

namespace pfs {
namespace traits {

template <typename T>
struct vector_rep<T, ::stdcxx::vector> : public ::stdcxx::vector<T>::type
{
    typedef typename ::stdcxx::vector<T>::type           base_class;
    typedef typename ::stdcxx::vector<T>::type           native_type;
    typedef native_type const &                          const_native_reference;
    typedef typename native_type::size_type              size_type;
    typedef typename native_type::value_type             value_type;
    typedef typename native_type::reference              reference;
    typedef typename native_type::const_reference        const_reference;
    typedef typename native_type::pointer                pointer;
    typedef typename native_type::const_pointer          const_pointer;
    typedef typename native_type::iterator               iterator;
    typedef typename native_type::const_iterator         const_iterator;
    typedef typename native_type::reverse_iterator       reverse_iterator;
    typedef typename native_type::const_reverse_iterator const_reverse_iterator;

    explicit vector_rep ()
        : base_class()
    {}

    vector_rep (size_type count, T const & value)
        : base_class(count, value)
    {}
    
    explicit vector_rep (size_type count)
        : base_class(count)
    {}
};

}} // pfs::traits

#endif

#endif /* __PFS_TRAITS_STDCXX_VECTOR_HPP__ */

