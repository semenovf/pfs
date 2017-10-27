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
namespace qt {

template <typename T>
struct linked_list_wrapper
{
    typedef QLinkedList<T> type;
};

template <typename T, typename ValueOrReference>
class linked_list_basic
{
    typedef ValueOrReference internal_type;

public:
    typedef linked_list_basic<T, traits::container_value<T, linked_list_wrapper> > container_value_type;
    typedef linked_list_basic<T, traits::container_reference<T, linked_list_wrapper> >   container_reference_type;
    
    typedef typename internal_type::native_type            native_type;
    typedef typename internal_type::native_reference       native_reference;
    typedef typename internal_type::const_native_reference const_native_reference;
    
    typedef typename native_type::value_type       value_type;
    typedef typename native_type::const_pointer    const_pointer;
    typedef typename native_type::reference        reference;
    typedef typename native_type::const_reference  const_reference;
    typedef typename native_type::iterator         iterator;
    typedef typename native_type::const_iterator   const_iterator;
    typedef typename native_type::reverse_iterator reverse_iterator;
    typedef typename native_type::const_reverse_iterator const_reverse_iterator;
    typedef typename native_type::difference_type  difference_type;
    typedef typename native_type::size_type        size_type;

protected:
    internal_type _p;
    
public:
    linked_list_basic ()
    {}

    linked_list_basic (native_reference rhs)
        : _p(rhs)
    {}

    linked_list_basic (const_native_reference rhs)
        : _p(rhs)
    {}
    
//    list (const_pointer s)
//        : _p(s)
//    {}
//
//    template <typename InputIt>
//    basic_string (InputIt first, InputIt last)
//        : _p(first, last)
//    {}
    
    linked_list_basic & operator = (native_reference rhs)
    {
        *_p = rhs;
        return *this;
    }

    operator const_native_reference () const
    {
        return *_p;
    }

    /**
     * Casts to native container reference
     */
    operator native_reference ()
    {
        return *_p;
    }

    // *************************************************************************
    // BEGIN Requirements for container traits
    // Based on [C++ concepts: Container](http://en.cppreference.com/w/cpp/concept/Container)
    // *************************************************************************

    // *** ITERATORS ***
    //
    iterator begin ()
    {
        return _p->begin();
    }
    
    const_iterator begin () const
    {
        return _p->begin();
    }
    
    iterator end ()
    {
        return _p->end();
    }
    
    const_iterator end () const
    {
        return _p->end();
    }
    
#if __cplusplus >= 201103L
    const_iterator cbegin () const
    {
        return _p->cbegin();
    }
    
    const_iterator cend () const
    {
        return _p->cend();
    }
#else
    const_iterator cbegin () const
    {
        return _p->begin();
    }
    
    const_iterator cend () const
    {
        return _p->end();
    }
#endif    

    // *** CAPACITY ***
    // 

    size_type size () const pfs_noexcept
    {
        return _p->size();
    }
    
    size_type max_size () const pfs_noexcept
    {
        return ((INT_MAX)/sizeof(T) - sizeof(native_type)) / 2; // TODO FIXME
    }
    
    bool empty () const pfs_noexcept
    {
        return _p->empty();
    }
    
    // *** MODIFIERS ***
    //
    // FIXME
//    void swap (container & rhs)
//    {
//        _p->swap(*rhs._p);
//    }
    
    // *** NON-MEMBER FUNCTIONS (OPERATORS) ***
    //
    
    friend inline bool operator == (linked_list_basic const & lhs
        , linked_list_basic const & rhs)
    {
        return *lhs._p == *rhs._p;
    }

    friend inline bool operator != (linked_list_basic const & lhs
        , linked_list_basic const & rhs)
    {
        return *lhs._p != *rhs._p;
    }

    // *************************************************************************
    // } END Requirements for container traits
    // *************************************************************************

    // *************************************************************************
    // BEGIN Requirements for sequence container traits {
    // *************************************************************************

    // *** ELEMENT ACCESS ***
    //
    reference back ()
    {
        return _p->back();
    }
		
    const_reference back () const
    {
        return _p->back();
    }
	
    reference front ()
    {
        return _p->front();
    }
		
    const_reference front () const
    {
        return _p->front();
    }
	
    // *** MODIFIERS ***
    //
        
    void clear () pfs_noexcept
    {
        _p->clear();
    }
    
#if __cplusplus >= 201103L
    template <typename... Args>
    iterator emplace (const_iterator pos, Args &&... args)
    {
        return _p->emplace(pos, args...);
    }

//    template <typename... Args>
//    reference emplace_back (Args &&... args)
//    {
//        return _p->emplace_back<Args>(args);
//    }
//    
//    template <typename... Args>
//    reference emplace_front (Args &&... args )
//    {
//        return _p->emplace_front<Args>(args);
//    }
#endif    

    iterator erase (const_iterator pos)
    {
        return _p->erase(pos);
    }
    
    iterator erase (const_iterator first, const_iterator last)
    {
        return _p->erase(first, last);
    }

    iterator insert (const_iterator pos, const_reference value)
    {
        return _p->insert(pos, value);
    }
    
#if __cplusplus >= 201103L
    iterator insert (const_iterator pos, T && value )
    {
        return _p->insert(pos, value);
    }
#endif

    iterator insert (const_iterator pos, size_type count, const_reference value)
    {
        return _p->insert(pos, count, value);
    }
    
    template <typename InputIt>
    iterator insert (const_iterator pos, InputIt first, InputIt last)
    {
        return _p->template insert<InputIt>(pos, first, last);
    }
    
#if __cplusplus >= 201103L
    iterator insert (const_iterator pos, std::initializer_list<T> ilist)
    {
        return _p->insert(pos, ilist);
    }
#endif
    
    void push_front (const_reference value)
    {
        _p->push_front(value);
    }
    
    void push_back (const_reference value)
    {
        _p->push_back(value);
    }

    void pop_front ()
    {
        _p->pop_front();
    }
    
    void pop_back ()
    {
        _p->pop_back();
    }
    
    // *************************************************************************
    // } END Requirements for sequence container traits
    // *************************************************************************
};

template <typename T>
class linked_list : public linked_list_basic<T, traits::container_value<T, linked_list_wrapper> >
{
    typedef linked_list_basic<T, traits::container_value<T, linked_list_wrapper> > base_class;

public:
    typedef typename base_class::native_type            native_type;
    typedef typename base_class::native_reference       native_reference;
    typedef typename base_class::const_native_reference const_native_reference;
    
public:
    linked_list ()
        : base_class()
    {}

    linked_list (native_reference rhs)
        : base_class(rhs)
    {}

    linked_list (const_native_reference rhs)
        : base_class(rhs)
    {}
};

template <typename T>
class linked_list_reference : public linked_list_basic<T, traits::container_reference<T, linked_list_wrapper> >
{
    typedef linked_list_basic<T, traits::container_reference<T, linked_list_wrapper> > base_class;

public:
    typedef typename base_class::native_type            native_type;
    typedef typename base_class::native_reference       native_reference;
    typedef typename base_class::const_native_reference const_native_reference;
    
public:
    linked_list_reference ()
        : base_class()
    {}

    linked_list_reference (native_reference rhs)
        : base_class(rhs)
    {}

    linked_list_reference (const_native_reference rhs)
        : base_class(rhs)
    {}
};

}} // pfs::qt

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

