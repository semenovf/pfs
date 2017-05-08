/* 
 * File:   linked_list.hpp
 * Author: wladt
 *
 * Created on May 8, 2017
 */

#ifndef __PFS_TRAITS_QT_LINKED_LIST_HPP__
#define __PFS_TRAITS_QT_LINKED_LIST_HPP__

#include <QLinkedList>
#include <pfs/traits/qt/container.hpp>

namespace pfs {
namespace traits {
namespace qt {

template <typename T>
struct linked_list_wrapper
{
    typedef QLinkedList<T> type;
};

template <typename T>
class linked_list : public container<T, linked_list_wrapper>
{
    typedef container<T, linked_list_wrapper> base_class;
    
public:
    typedef typename base_class::native_type      native_type;
    typedef typename base_class::native_reference native_reference;
    typedef typename base_class::size_type        size_type;
    typedef typename native_type::value_type      value_type;
    typedef typename native_type::reference       reference;
    typedef typename native_type::const_reference const_reference;

public:
    linked_list (native_reference rhs)
        : base_class(rhs)
    {}

    size_type max_size () const pfs_noexcept
    {
        return ((INT_MAX)/sizeof(T) - sizeof(native_type)) / 2; // TODO FIXME
    }
};

}}} // pfs::traits::qt

#if __COMMENT__


#include <pfs/iterator.hpp>
#include <pfs/traits/list.hpp>

namespace pfs {
namespace traits {

template <typename T>
struct list_rep<T, QLinkedList> : public QLinkedList<T>
{
    typedef QLinkedList<T>                                 base_class;
    typedef QLinkedList<T>                                 native_type;
    typedef native_type const &                            const_native_reference;
    typedef typename native_type::size_type                size_type;
    typedef typename native_type::value_type               value_type;
    typedef typename native_type::reference                reference;
    typedef typename native_type::const_reference          const_reference;
    typedef typename native_type::iterator                 iterator;
    typedef typename native_type::const_iterator           const_iterator;
    typedef typename std::reverse_iterator<iterator>       reverse_iterator;
    typedef typename std::reverse_iterator<const_iterator> const_reverse_iterator;

    explicit list_rep ()
        : base_class()
    {}

    list_rep (size_type count, T const & value)
        : base_class(count, value)
    {}
    
    explicit list_rep (size_type count)
        : base_class(count)
    {}

    reverse_iterator rbegin ()
    {
        return reverse_iterator(base_class::end());
    }
    
    const_reverse_iterator rbegin () const
    {
        return const_reverse_iterator(base_class::end());
    }
    
    reverse_iterator rend ()
    {
        return reverse_iterator(base_class::begin());
    }
    
    const_reverse_iterator rend () const
    {
        return const_reverse_iterator(base_class::begin());
    }

    void resize (size_type count, const_reference value);

    void splice (iterator pos, native_type & rhs)
    {
        // Emulate splice operation through move elements
        // TODO Optimize it
        for (iterator it = rhs.begin(); it != rhs.end(); ++it)
            pos = base_class::insert(pos, *it);
        rhs.clear();
    }

};

template <typename T>
void list_rep<T, QLinkedList>::resize (size_type count
        , const_reference value)
{
    if (count < base_class::size()) {
        // Reduce container to first count elements
        //
        iterator first = base_class::begin();
        pfs::advance(first, count);
        base_class::erase(first, base_class::end());
    } else {
        for (size_type i = base_class::size(); i < count; ++i)
            base_class::append(value);
    }
}

}} // pfs::traits

#endif

#endif /* __PFS_TRAITS_QT_LINKED_LIST_HPP__ */

