#ifndef __PFS_TRAITS_CONTAINER_HPP__
#define __PFS_TRAITS_CONTAINER_HPP__

#include <pfs/cxxlang.hpp>

namespace pfs {
namespace traits {

// Container traits.
// Based on [C++ concepts: Container](http://en.cppreference.com/w/cpp/concept/Container)

template <typename T, template <typename> class ContainerValueRef>
class container
{
protected:
    typedef ContainerValueRef<T> internal_type;

public:
    typedef typename internal_type::native_type            native_type;
    typedef typename internal_type::native_reference       native_reference;
    typedef typename internal_type::const_native_reference const_native_reference;

    typedef T                                              value_type;
    //typedef typename internal_type::reference              reference;
    //typedef typename internal_type::const_reference        const_reference;
    typedef typename internal_type::iterator               iterator;
    typedef typename internal_type::const_iterator         const_iterator;
    typedef typename internal_type::difference_type        difference_type;
    typedef typename internal_type::size_type              size_type;

protected:
    internal_type _p;

public:
    // Acceptable for container_value only (as opposite to container_reference)
    container ()
    {}

    /**
     * Initializes with reference to native container
     */
    container (native_reference rhs)
        : _p(rhs)
    {}

    /**
     * Casts to const native container reference
     */
    operator const_native_reference () const
    {
        return _p;
    }

    /**
     * Casts to native container reference
     */
    operator native_reference ()
    {
        return _p;
    }

    // *** ITERATORS ***
    //

    iterator begin ()
    {
        return _p.begin();
    }

    const_iterator begin () const
    {
        return _p.begin();
    }

    iterator end ()
    {
        return _p.end();
    }

    const_iterator end () const
    {
        return _p.end();
    }

    const_iterator cbegin () const
    {
        return _p.cbegin();
    }

    const_iterator cend () const
    {
        return _p.cend();
    }

    // *** CAPACITY ***
    //

    size_type size () const noexcept
    {
        return _p.size();
    }

    size_type max_size () const noexcept
    {
        return _p.max_size();
    }

    bool empty () const noexcept
    {
        return _p.empty();
    }

    // *** MODIFIERS ***
    //
    void clear ()
    {
        _p.clear();
    }

    iterator erase (const_iterator pos)
    {
        _p.erase(pos);
    }

    // FIXME
//    void swap (container & rhs)
//    {
//        _p->swap(rhs._p);
//    }

    // *** NON-MEMBER FUNCTIONS (OPERATORS)***
    //

    friend inline bool operator == (container const & lhs
        , container const & rhs)
    {
        return lhs._p == rhs._p;
    }

    friend inline bool operator != (container const & lhs
        , container const & rhs)
    {
        return lhs._p != rhs._p;
    }
};

}} // pfs::traits

#endif /* __PFS_TRAITS_CONTAINER_HPP__ */

