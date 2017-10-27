/* 
 * File:   vector.hpp
 * Author: wladt
 *
 * Created on February 26, 2017, 10:58 AM
 */

#ifndef __PFS_TRAITS_QT_VECTOR_HPP__
#define __PFS_TRAITS_QT_VECTOR_HPP__

#include <QVector>
#include <pfs/iterator.hpp>
#include <pfs/traits/value_ref.hpp>

namespace pfs {
namespace qt {

template <typename T>
struct vector_wrapper
{
    typedef QVector<T> type;
};

template <typename T, typename ValueOrReference>
class vector_basic
{
    typedef ValueOrReference internal_type;

public:
    typedef vector_basic<T, traits::container_value<T, vector_wrapper> > container_value_type;
    typedef vector_basic<T, traits::container_reference<T, vector_wrapper> >   container_reference_type;

    typedef typename internal_type::native_type            native_type;
    typedef typename internal_type::native_reference       native_reference;
    typedef typename internal_type::const_native_reference const_native_reference;

    typedef typename native_type::value_type       value_type;
    typedef typename native_type::pointer          pointer;
    typedef typename native_type::const_pointer    const_pointer;
    typedef typename native_type::reference        reference;
    typedef typename native_type::const_reference  const_reference;
    typedef typename native_type::iterator         iterator;
    typedef typename native_type::const_iterator   const_iterator;
    typedef typename pfs::reverse_iterator<iterator>       reverse_iterator;
    typedef typename pfs::reverse_iterator<const_iterator> const_reverse_iterator;
    typedef typename native_type::difference_type  difference_type;
    typedef typename native_type::size_type        size_type;

protected:
    internal_type _p;
    
public:
    vector_basic ()
    {}

    vector_basic (native_reference rhs)
        : _p(rhs)
    {}

    vector_basic (const_native_reference rhs)
        : _p(rhs)
    {}
    
//    vector_basic (size_type count, T const & value)
//        : _p(count, value)
//    {}

//    template <typename InputIt>
//    basic_string (InputIt first, InputIt last)
//        : _p(first, last)
//    {}
    
    vector_basic & operator = (native_reference rhs)
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
    
    friend inline bool operator == (vector_basic const & lhs
        , vector_basic const & rhs)
    {
        return *lhs._p == *rhs._p;
    }

    friend inline bool operator != (vector_basic const & lhs
        , vector_basic const & rhs)
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
        return _p->template emplace_back<Args...>(args...);
    }
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
        pfs::advance(it, distance(this->cbegin(), pos));
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
        pfs::advance(from, distance(this->cbegin(), first));
        pfs::advance(to, distance(this->cbegin(), last));
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
        return _p->template insert<InputIt>(pos, first, last);
    }

#if __cplusplus >= 201103L
    iterator insert (const_iterator pos, std::initializer_list<T> ilist)
    {
        return _p->insert(pos, ilist);
    }
#endif

    void push_back (const_reference value)
    {
        _p->push_back(value);
    }

    void pop_back ()
    {
        _p->pop_back();
    }

    // *************************************************************************
    // } END Requirements for sequence container traits
    // *************************************************************************

    // *************************************************************************
    // BEGIN Requirements for contigous container traits {
    // *************************************************************************

    pointer data ()
    {
        return _p->data();
    }

    const_pointer data () const
    {
        return _p->data();
    }

    void resize (size_type count)
    {
        _p->resize(count);
    }

    void resize (size_type count, value_type const & value)
    {
        _p->resize(count, value);
    }

    void reserve (size_type new_cap)
    {
        _p->reserve(new_cap);
    }

    void shrink_to_fit ()
    {
        _p->shrink_to_fit();
    }

    //
    // *** ELEMENT ACCESS
    //

    reference at (size_type pos)
    {
        return _p->at(pos);
    }

    const_reference at (size_type pos) const
    {
        return _p->at(pos);
    }

    reference operator [] (size_type pos)
    {
        return _p->operator[](pos);
    }

    const_reference operator [] (size_type pos) const
    {
        return _p->operator[](pos);
    }

    // *************************************************************************
    // } END Requirements for contigous container traits
    // *************************************************************************
};

template <typename T>
class vector : public vector_basic<T, traits::container_value<T, vector_wrapper> >
{
    typedef vector_basic<T, traits::container_value<T, vector_wrapper> > base_class;

public:
    typedef typename base_class::native_type            native_type;
    typedef typename base_class::native_reference       native_reference;
    typedef typename base_class::const_native_reference const_native_reference;
    typedef typename base_class::size_type              size_type;

public:
    vector ()
        : base_class()
    {}

    vector (native_reference rhs)
        : base_class(rhs)
    {}

    vector (const_native_reference rhs)
        : base_class(rhs)
    {}

//    vector (size_type count, T const & value)
//        : base_class(count, value)
//    {}
};

template <typename T>
class vector_reference : public vector_basic<T, traits::container_reference<T, vector_wrapper> >
{
    typedef vector_basic<T, traits::container_reference<T, vector_wrapper> > base_class;

public:
    typedef typename base_class::native_type            native_type;
    typedef typename base_class::native_reference       native_reference;
    typedef typename base_class::const_native_reference const_native_reference;

public:
    vector_reference ()
        : base_class()
    {}

    vector_reference (native_reference rhs)
        : base_class(rhs)
    {}

    vector_reference (const_native_reference rhs)
        : base_class(rhs)
    {}
};











//template <typename T>
//struct vector_wrapper
//{
//    typedef QVector<T> type;
//};
//
//template <typename T>
//class vector : public container<T, vector_wrapper>
//{
//    typedef container<T, vector_wrapper> base_class;
//    
//public:
//    typedef typename base_class::native_type      native_type;
//    typedef typename base_class::native_reference native_reference;
//    typedef typename base_class::size_type        size_type;
//    typedef typename native_type::value_type      value_type;
//    typedef typename native_type::reference       reference;
//    typedef typename native_type::const_reference const_reference;
//
//public:
//    vector (native_reference rhs)
//        : base_class(rhs)
//    {}
//
//    size_type max_size () const pfs_noexcept
//    {
//        return (INT_MAX)/sizeof(T) - sizeof(native_type);
//    }
//};

}} // pfs::qt

#endif /* __PFS_TRAITS_QT_VECTOR_HPP__ */
