#ifndef __PFS_TRAITS_STDCXX_MULTISET_HPP__
#define __PFS_TRAITS_STDCXX_MULTISET_HPP__

#include <set>
#include <pfs/traits/value_ref.hpp>

namespace pfs {
namespace stdcxx {

template <typename T>
struct multiset_wrapper
{
    typedef std::multiset<T> type;
};

template <typename T, typename ValueOrReference>
class multiset_basic
{
    typedef ValueOrReference internal_type;

public:
    typedef multiset_basic<T, traits::container_value<T, multiset_wrapper> > container_value_type;
    typedef multiset_basic<T, traits::container_reference<T, multiset_wrapper> >   container_reference_type;
    
    typedef typename internal_type::native_type            native_type;
    typedef typename internal_type::native_reference       native_reference;
    typedef typename internal_type::const_native_reference const_native_reference;
    
    typedef typename native_type::value_type       value_type;
    //typedef typename native_type::const_pointer    const_pointer;
    //typedef typename native_type::reference        reference;
    //typedef typename native_type::const_reference  const_reference;
    typedef typename native_type::iterator         iterator;
    typedef typename native_type::const_iterator   const_iterator;
    typedef typename native_type::reverse_iterator reverse_iterator;
    typedef typename native_type::const_reverse_iterator const_reverse_iterator;
    typedef typename native_type::difference_type  difference_type;
    typedef typename native_type::size_type        size_type;

    typedef typename native_type::key_type         key_type;
    
    typedef typename native_type::key_compare      key_compare;
    typedef typename native_type::value_compare    value_compare;
    

protected:
    internal_type _p;
    
public:
    multiset_basic ()
    {}

    multiset_basic (native_reference rhs)
        : _p(rhs)
    {}

    multiset_basic (const_native_reference rhs)
        : _p(rhs)
    {}
    
//    multiset (const_pointer s)
//        : _p(s)
//    {}
//
//    template <typename InputIt>
//    basic_string (InputIt first, InputIt last)
//        : _p(first, last)
//    {}
    
    multiset_basic & operator = (native_reference rhs)
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
    
    iterator erase (const_iterator pos)
    {
        return _p->erase(pos);
    }

    // FIXME
//    void swap (container & rhs)
//    {
//        _p->swap(*rhs._p);
//    }
    
    // *** NON-MEMBER FUNCTIONS (OPERATORS) ***
    //
    
    friend inline bool operator == (multiset_basic const & lhs
        , multiset_basic const & rhs)
    {
        return *lhs._p == *rhs._p;
    }

    friend inline bool operator != (multiset_basic const & lhs
        , multiset_basic const & rhs)
    {
        return *lhs._p != *rhs._p;
    }

    // *************************************************************************
    // } END Requirements for container traits
    // *************************************************************************
    
    // *************************************************************************
    // BEGIN Requirements for associative container traits {
    // *************************************************************************

    // *** LOOKUP ***
    //
    
    iterator find (key_type const & key)
    {
        return _p->find(key);
    }
    
    const_iterator find (key_type const & key) const
    {
        return _p->find(key);
    }
    
    // *************************************************************************
    // } END Requirements for associative container traits
    // *************************************************************************
};

template <typename T>
class multiset : public multiset_basic<T, traits::container_value<T, multiset_wrapper> >
{
    typedef multiset_basic<T, traits::container_value<T, multiset_wrapper> > base_class;

public:
    typedef typename base_class::native_type            native_type;
    typedef typename base_class::native_reference       native_reference;
    typedef typename base_class::const_native_reference const_native_reference;
    
public:
    multiset ()
        : base_class()
    {}

    multiset (native_reference rhs)
        : base_class(rhs)
    {}

    multiset (const_native_reference rhs)
        : base_class(rhs)
    {}
};

template <typename T>
class multiset_reference : public multiset_basic<T, traits::container_reference<T, multiset_wrapper> >
{
    typedef multiset_basic<T, traits::container_reference<T, multiset_wrapper> > base_class;

public:
    typedef typename base_class::native_type            native_type;
    typedef typename base_class::native_reference       native_reference;
    typedef typename base_class::const_native_reference const_native_reference;
    
public:
    multiset_reference ()
        : base_class()
    {}

    multiset_reference (native_reference rhs)
        : base_class(rhs)
    {}

    multiset_reference (const_native_reference rhs)
        : base_class(rhs)
    {}
};

}} // pfs::stdcxx

#endif /* __PFS_TRAITS_STDCXX_MULTISET_HPP__ */
