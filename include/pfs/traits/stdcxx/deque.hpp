/* 
 * File:   deque.hpp
 * Author: wladt
 *
 * Created on May 08, 2017
 */

#ifndef __PFS_TRAITS_STDCXX_DEQUE_HPP__
#define __PFS_TRAITS_STDCXX_DEQUE_HPP__

#include <deque>
#include <pfs/traits/value_ref.hpp>

namespace pfs {
namespace traits {
namespace stdcxx {

template <typename T>
struct deque_wrapper
{
    typedef std::deque<T> type;
};

template <typename T, typename ValueOrReference>
class deque_basic
{
    typedef ValueOrReference internal_type;

public:
    typedef deque_basic<T, container_value<T, deque_wrapper> > container_value_type;
    typedef deque_basic<T, container_reference<T, deque_wrapper> >   container_reference_type;
    
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
    deque_basic ()
    {}

    deque_basic (native_reference rhs)
        : _p(rhs)
    {}

    deque_basic (const_native_reference rhs)
        : _p(rhs)
    {}
    
//    deque (const_pointer s)
//        : _p(s)
//    {}
//
//    template <typename InputIt>
//    basic_string (InputIt first, InputIt last)
//        : _p(first, last)
//    {}
    
    deque_basic & operator = (native_reference rhs)
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
    
    friend inline bool operator == (deque_basic const & lhs
        , deque_basic const & rhs)
    {
        return *lhs._p == *rhs._p;
    }

    friend inline bool operator != (deque_basic const & lhs
        , deque_basic const & rhs)
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

    template <typename... Args>
    void emplace_back (Args &&... args)
    {
        return _p->emplace_back(args...);
    }
    
//    template <typename... Args>
//    reference emplace_front (Args &&... args )
//    {
//        return _p->emplace_front(args...);
//    }
#endif    

    iterator erase (const_iterator pos)
    {
#if __cplusplus >= 201103L
        return _p->erase(pos);
#else        
        //
        // C++ prior to C++11 
        // erase() has signature `iterator erase(iterator first, iterator last)`
        //
        iterator it(this->begin());
        advance(it, distance(this->cbegin(), pos));
        return _p->erase(it);
#endif
    }
    
    iterator erase (const_iterator first, const_iterator last)
    {
#if __cplusplus >= 201103L
        return _p->erase(first, last);
#else
        //
        // C++ prior to C++11 
        // erase() has signature `iterator erase(iterator first, iterator last)`
        //
        iterator from(this->begin());
        iterator to(this->begin());
        advance(from, distance(this->cbegin(), first));
        advance(to, distance(this->cbegin(), last));
        return _p->erase(from, to);
#endif        
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
class deque : public deque_basic<T, container_value<T, deque_wrapper> >
{
    typedef deque_basic<T, container_value<T, deque_wrapper> > base_class;

public:
    typedef typename base_class::native_type            native_type;
    typedef typename base_class::native_reference       native_reference;
    typedef typename base_class::const_native_reference const_native_reference;
    
public:
    deque ()
        : base_class()
    {}

    deque (native_reference rhs)
        : base_class(rhs)
    {}

    deque (const_native_reference rhs)
        : base_class(rhs)
    {}
};

template <typename T>
class deque_reference : public deque_basic<T, container_reference<T, deque_wrapper> >
{
    typedef deque_basic<T, container_reference<T, deque_wrapper> > base_class;

public:
    typedef typename base_class::native_type            native_type;
    typedef typename base_class::native_reference       native_reference;
    typedef typename base_class::const_native_reference const_native_reference;
    
public:
    deque_reference ()
        : base_class()
    {}

    deque_reference (native_reference rhs)
        : base_class(rhs)
    {}

    deque_reference (const_native_reference rhs)
        : base_class(rhs)
    {}
};

}}} // pfs::traits::stdcxx

#endif /* __PFS_TRAITS_STDCXX_DEQUE_HPP__ */
