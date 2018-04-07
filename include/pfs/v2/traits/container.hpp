#pragma once
#include <pfs/cxxlang.hpp>

namespace pfs {
namespace traits {

// Container traits.
// Based on [C++ concepts: Container](http://en.cppreference.com/w/cpp/concept/Container)

template <typename ContainerImpl>
class container
{
public:
    typedef ContainerImpl native_type;
    typedef native_type & native_reference;

    typedef typename native_type::value_type      value_type;
    typedef typename native_type::key_type        key_type;
    typedef typename native_type::mapped_type     mapped_type;
    typedef typename native_type::iterator        iterator;
    typedef typename native_type::const_iterator  const_iterator;
    typedef typename native_type::difference_type difference_type;
    typedef typename native_type::size_type       size_type;

protected:
    ContainerImpl * _p;

public:
    container (native_reference rhs)
        : _p(& rhs)
    {}

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

    const_iterator cbegin () const
    {
        return _p->cbegin();
    }

    const_iterator cend () const
    {
        return _p->cend();
    }

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
    void clear ()
    {
        _p->clear();
    }

    iterator erase (const_iterator pos)
    {
        _p->erase(pos);
    }

    // FIXME
//    void swap (container & rhs)
//    {
//        _p->swap(rhs._p);
//    }

    // *** NON-MEMBER FUNCTIONS (OPERATORS)***
    //

//     friend inline bool operator == (container const & lhs
//         , container const & rhs)
//     {
//         return lhs._p == rhs._p;
//     }
//
//     friend inline bool operator != (container const & lhs
//         , container const & rhs)
//     {
//         return lhs._p != rhs._p;
//     }
};

}} // pfs::traits

