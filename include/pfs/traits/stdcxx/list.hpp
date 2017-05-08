/* 
 * File:   list.hpp
 * Author: wladt
 *
 * Created on January 16, 2017, 12:58 PM
 */

#ifndef __PFS_TRAITS_STDCXX_LIST_HPP__
#define __PFS_TRAITS_STDCXX_LIST_HPP__

#include <list>
#include <pfs/traits/stdcxx/container.hpp>

namespace pfs {
namespace traits {
namespace stdcxx {

template <typename T>
struct list_wrapper
{
    typedef std::list<T> type;
};

template <typename T>
class list : public container<T, list_wrapper>
{
    typedef container<T, list_wrapper> base_class;
    
public:
    typedef typename base_class::native_reference native_reference;

public:
    list (native_reference rhs)
        : base_class(rhs)
    {}
};

}}} // pfs::traits::stdcxx

#if __OBSOLETE__

namespace stdcxx {

template <typename T>
struct list
{
    typedef std::list<T> type;
};

} // stdcxx

namespace pfs {
namespace traits {

template <typename T>
struct list_rep<T, ::stdcxx::list> : public ::stdcxx::list<T>::type
{
    typedef typename ::stdcxx::list<T>::type           base_class;
    typedef typename ::stdcxx::list<T>::type           native_type;
    typedef native_type const &                          const_native_reference;
    typedef typename native_type::size_type              size_type;
    typedef typename native_type::value_type             value_type;
    typedef typename native_type::reference              reference;
    typedef typename native_type::const_reference        const_reference;
    typedef typename native_type::iterator               iterator;
    typedef typename native_type::const_iterator         const_iterator;
    typedef typename native_type::reverse_iterator       reverse_iterator;
    typedef typename native_type::const_reverse_iterator const_reverse_iterator;

    explicit list_rep ()
        : base_class()
    {}

    list_rep (size_type count, T const & value)
        : base_class(count, value)
    {}
    
    explicit list_rep (size_type count)
        : base_class(count)
    {}

    iterator erase (iterator first
            , iterator last)
    {
#if __cplusplus >= 201103L
        return base_class::erase(first, last);
#else
        base_class::erase(first, last);
        return last; // TODO check in manual if iterator is valid after eraser.
#endif        
    }

    iterator insert (iterator pos, value_type const & value)
    {
#if __cplusplus >= 201103L
        return base_class::insert(pos, value);
#else
        base_class::insert(pos++, value);
        return pos;
#endif
    }
};

}} // pfs::traits

#endif

#endif /* __PFS_TRAITS_STDCXX_LIST_HPP__ */
