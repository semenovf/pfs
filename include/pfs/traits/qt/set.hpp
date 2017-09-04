/* 
 * File:   set.hpp
 * Author: wladt
 *
 * Created on May 8, 2017
 */

#ifndef __PFS_TRAITS_QT_SET_HPP__
#define __PFS_TRAITS_QT_SET_HPP__

#include <QSet>
#include <pfs/traits/qt/container.hpp>

namespace pfs {
namespace qt {

template <typename T>
struct set_wrapper
{
    typedef QSet<T> type;
};

template <typename T>
class set : public container<T, set_wrapper>
{
    typedef container<T, set_wrapper> base_class;
    
public:
    typedef typename base_class::native_type      native_type;
    typedef typename base_class::native_reference native_reference;
    typedef typename base_class::size_type        size_type;
    typedef typename native_type::value_type      value_type;
    typedef typename native_type::reference       reference;
    typedef typename native_type::const_reference const_reference;

public:
    set (native_reference rhs)
        : base_class(rhs)
    {}

    size_type max_size () const pfs_noexcept
    {
        return ((INT_MAX)/sizeof(T) - sizeof(native_type));
    }
};

}} // pfs::qt

#endif /* __PFS_TRAITS_QT_SET_HPP__ */

