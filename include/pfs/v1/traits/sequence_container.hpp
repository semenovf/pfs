#ifndef __PFS_TRAITS_SEQUENCE_CONTAINER_HPP__
#define __PFS_TRAITS_SEQUENCE_CONTAINER_HPP__

#include <pfs/traits/container.hpp>

namespace pfs {
namespace traits {

// SequenceContainer traits.
// Based on [C++ concepts: SequenceContainer](http://en.cppreference.com/w/cpp/concept/SequenceContainer)

template <typename T, template <typename> class ContainerValueRef>
class sequence_container : public container<T, ContainerValueRef>
{
    typedef container<T, ContainerValueRef> base_class;

public:
    typedef typename base_class::native_type            native_type;
    typedef typename base_class::native_reference       native_reference;
    typedef typename base_class::const_native_reference const_native_reference;

    typedef typename base_class::size_type                      size_type;
    typedef typename base_class::value_type                     value_type;
    typedef typename base_class::internal_type::reference       reference;
    typedef typename base_class::internal_type::const_reference const_reference;
    typedef typename base_class::iterator        iterator;
    typedef typename base_class::const_iterator  const_iterator;

public:
    sequence_container ()
        : base_class()
    {}

    /**
     * Initializes with reference to native container
     */
    sequence_container (native_reference rhs)
        : base_class(rhs)
    {}

    void assign (size_type count, const_reference value)
    {
        base_class::_p.assign(count, value);
    }

    template <typename InputIt>
    void assign (InputIt first, InputIt last)
    {
        base_class::_p.template assign<InputIt>(first, last);
    }

#if __cplusplus >= 201103L
    void assign (std::initializer_list<T> ilist)
    {
        base_class::_p.assign(ilist);
    }
#endif

    // *** ELEMENT ACCESS ***
    //
    reference back ()
    {
        return base_class::_p.back();
    }

    const_reference back () const
    {
        return base_class::_p.back();
    }

    reference front ()
    {
        return base_class::_p.front();
    }

    const_reference front () const
    {
        return base_class::_p.front();
    }

    // *** MODIFIERS ***
    //

    void clear () noexcept
    {
        base_class::_p.clear();
    }

#if __cplusplus >= 201103L
    template <typename... Args>
    iterator emplace (const_iterator pos, Args &&... args)
    {
        return base_class::_p.emplace(pos, args...);
    }

    template <typename... Args>
    void emplace_back (Args &&... args)
    {
        base_class::_p.emplace_back(args...);
    }

//    template <typename... Args>
//    reference emplace_front (Args &&... args )
//    {
//        return base_class::_p.emplace_front<Args>(args);
//    }
#endif

    iterator erase (const_iterator pos)
    {
        return base_class::_p.erase(pos);
    }

    iterator erase (const_iterator first, const_iterator last)
    {
        return base_class::_p.erase(first, last);
    }

    iterator insert (const_iterator pos, const_reference value)
    {
        return base_class::_p.insert(pos, value);
    }

#if __cplusplus >= 201103L
    iterator insert (const_iterator pos, T && value )
    {
        return base_class::_p.insert(pos, value);
    }
#endif

    iterator insert (const_iterator pos, size_type count, const_reference value)
    {
        return base_class::_p.insert(pos, count, value);
    }

    template <typename InputIt>
    iterator insert (const_iterator pos, InputIt first, InputIt last)
    {
        return base_class::_p.template insert<InputIt>(pos, first, last);
    }

#if __cplusplus >= 201103L
    iterator insert (const_iterator pos, std::initializer_list<T> ilist)
    {
        return base_class::_p.insert(pos, ilist);
    }
#endif

    void push_front (const_reference value)
    {
        base_class::_p.push_front(value);
    }

//#if __cplusplus >= 201103L
//    void push_front (T && value)
//    {
//        _p.push_front(value);
//    }
//#endif

    void push_back (const_reference value)
    {
        base_class::_p.push_back(value);
    }

    void pop_front ()
    {
        base_class::_p.pop_front();
    }

    void pop_back ()
    {
        base_class::_p.pop_back();
    }
    
    void resize (size_type count)
    {
        base_class::_p.resize(count);
    }

    void resize (size_type count, value_type const & value)
    {
        base_class::_p.resize(count, value);
    }

    // *** NON-MEMBER FUNCTIONS (OPERATORS)***
    //
};

}} // pfs::traits

#endif /* __PFS_TRAITS_SEQUENCE_CONTAINER_HPP__ */

