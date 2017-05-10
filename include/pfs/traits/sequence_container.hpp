/* 
 * File:   sequence_container.hpp
 * Author: wladt
 *
 * Created on May 8, 2017, 1:40 PM
 */

#ifndef __PFS_TRAITS_SEQUENCE_CONTAINER_HPP__
#define __PFS_TRAITS_SEQUENCE_CONTAINER_HPP__

#include <pfs/traits/container.hpp>

namespace pfs {
namespace traits {

// SequenceContainer traits.
// Based on [C++ concepts: SequenceContainer](http://en.cppreference.com/w/cpp/concept/SequenceContainer)

template <typename T, template <typename> class ContainerRef>
class sequence_container : public container<T, ContainerRef>
{
    typedef container<T, ContainerRef> base_class;
    
public:
    typedef ContainerRef<T>                           ref_type;
//    typedef typename ref_type::native_type            native_type;
    typedef typename ref_type::native_reference       native_reference;
//    typedef typename ref_type::const_native_reference const_native_reference;
//
//    typedef typename ref_type::value_type             value_type;
    typedef typename ref_type::reference              reference;
    typedef typename ref_type::const_reference        const_reference;
    typedef typename ref_type::iterator               iterator;
    typedef typename ref_type::const_iterator         const_iterator;
//    typedef typename ref_type::difference_type        difference_type;
    typedef typename ref_type::size_type              size_type;
    
public:
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
        base_class::_p.assign<InputIt>(first, last);
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
        
    void clear () pfs_noexcept
    {
        base_class::_p.clear();
    }
    
#if __cplusplus >= 201103L
    template <typename... Args>
    iterator emplace (const_iterator pos, Args &&... args)
    {
        return base_class::_p.emplace(pos, args);
    }

//    template <typename... Args>
//    reference emplace_back (Args &&... args)
//    {
//        return _p.emplace_back<Args>(args);
//    }
//    
//    template <typename... Args>
//    reference emplace_front (Args &&... args )
//    {
//        return _p.emplace_front<Args>(args);
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
        return base_class::_p.insert<InputIt>(pos, first, last);
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

//    void pop_back ()
//    {
//        _p.pop_back();
//    }
	
    // *** NON-MEMBER FUNCTIONS (OPERATORS)***
    //
};

}} // pfs::traits

#endif /* __PFS_TRAITS_SEQUENCE_CONTAINER_HPP__ */

