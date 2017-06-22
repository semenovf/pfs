/* 
 * File:   list.hpp
 * Author: wladt
 *
 * Created on January 16, 2017, 12:58 PM
 */

#ifndef __PFS_TRAITS_STDCXX_LIST_HPP__
#define __PFS_TRAITS_STDCXX_LIST_HPP__

#include <list>
#include <pfs/traits/value_ref.hpp>

namespace pfs {
namespace traits {
namespace stdcxx {

template <typename T>
struct list_wrapper
{
    typedef std::list<T> type;
};

template <typename T, typename ValueOrReference>
class list_basic
{
    typedef ValueOrReference internal_type;

public:
    typedef list_basic<T, container_value<T, list_wrapper> > container_value_type;
    typedef list_basic<T, container_reference<T, list_wrapper> >   container_reference_type;
    
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
    list_basic ()
    {}

    list_basic (native_reference rhs)
        : _p(rhs)
    {}

    list_basic (const_native_reference rhs)
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
    
    list_basic & operator = (native_reference rhs)
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
        return _p->cbegin();
    }
#else
    const_iterator cbegin () const
    {
        return _p->begin();
    }
    
    const_iterator cend () const
    {
        return _p->begin();
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
        return _p->max_size();
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
    
    friend inline bool operator == (list_basic const & lhs
        , list_basic const & rhs)
    {
        return *lhs._p == *rhs._p;
    }

    friend inline bool operator != (list_basic const & lhs
        , list_basic const & rhs)
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
        return _p->insert<InputIt>(pos, first, last);
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
class list : public list_basic<T, container_value<T, list_wrapper> >
{
    typedef list_basic<T, container_value<T, list_wrapper> > base_class;

public:
    typedef typename base_class::native_type            native_type;
    typedef typename base_class::native_reference       native_reference;
    typedef typename base_class::const_native_reference const_native_reference;
    
public:
    list ()
        : base_class()
    {}

    list (native_reference rhs)
        : base_class(rhs)
    {}

    list (const_native_reference rhs)
        : base_class(rhs)
    {}
};

template <typename T>
class list_reference : public list_basic<T, container_reference<T, list_wrapper> >
{
    typedef list_basic<T, container_reference<T, list_wrapper> > base_class;

public:
    typedef typename base_class::native_type            native_type;
    typedef typename base_class::native_reference       native_reference;
    typedef typename base_class::const_native_reference const_native_reference;
    
public:
    list_reference ()
        : base_class()
    {}

    list_reference (native_reference rhs)
        : base_class(rhs)
    {}

    list_reference (const_native_reference rhs)
        : base_class(rhs)
    {}
};

}}} // pfs::traits::stdcxx

#if __OBSOLETE__


template <typename T>
struct list_rep<T, ::stdcxx::list> : public ::stdcxx::list<T>::type
{

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
