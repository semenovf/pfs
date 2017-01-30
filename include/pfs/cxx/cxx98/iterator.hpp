/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   iterator.hpp
 * Author: wladt
 *
 * Created on December 30, 2016, 11:00 AM
 */

#ifndef __PFS_CXX98_ITERATOR_HPP__
#define __PFS_CXX98_ITERATOR_HPP__

#include <iterator>
#include <pfs/types.hpp>

namespace pfs {

typedef std::input_iterator_tag         input_iterator_tag;
typedef std::output_iterator_tag        output_iterator_tag;
typedef std::forward_iterator_tag       forward_iterator_tag;
typedef std::bidirectional_iterator_tag bidirectional_iterator_tag;
typedef std::random_access_iterator_tag random_access_iterator_tag;

template <typename Category, typename T, typename Distance = ptrdiff_t,
    typename Pointer = T *, typename Reference = T &>
struct iterator
{
    typedef Category  iterator_category;
    typedef T         value_type;
    typedef Distance  difference_type;
    typedef Pointer   pointer;
    typedef Reference reference;
};

template <typename Iterator>
struct iterator_traits
{
    typedef typename Iterator::iterator_category iterator_category;
    typedef typename Iterator::value_type        value_type;
    typedef typename Iterator::difference_type   difference_type;
    typedef typename Iterator::pointer           pointer;
    typedef typename Iterator::reference         reference;
};

template <typename T>
struct iterator_traits <T *>
{
    typedef std::random_access_iterator_tag iterator_category;
    typedef T         value_type;
    typedef ptrdiff_t difference_type;
    typedef T *       pointer;
    typedef T &       reference;
};

template <typename T>
struct iterator_traits<const T *>
{
    typedef std::random_access_iterator_tag iterator_category;
    typedef T         value_type;
    typedef ptrdiff_t difference_type;
    typedef const T * pointer;
    typedef const T & reference;
};

template <typename Category, typename Derived, typename T, typename Pointer, typename Reference, typename Distance = ptrdiff_t>
struct iterator_facade; 

template <typename Derived, typename T, typename Pointer, typename Reference, typename Distance>
struct iterator_facade<input_iterator_tag, Derived, T, Pointer, Reference, Distance> 
{
    typedef input_iterator_tag iterator_category;
    typedef T                  value_type;
    typedef Distance           difference_type;
    typedef Pointer            pointer;
    typedef Reference          reference;

    static reference ref (Derived &);
    static pointer   ptr (Derived &);
    static void      increment (Derived &);
    static bool      equals (Derived const &, Derived const &);

    value_type operator * () const
    {
    	return Derived::ref(*const_cast<Derived *>(static_cast<Derived const *>(this)));
    }

    pointer operator -> () const
    {
    	return Derived::ptr(*const_cast<Derived *>(static_cast<Derived const *>(this)));
    }

    iterator_facade & operator ++ () // prefix increment
	{
    	Derived::increment(static_cast<Derived &>(*this));
    	return *this;
	}

    Derived operator ++ (int) // postfix increment
	{
        Derived r(static_cast<Derived &>(*this));
        Derived::increment(static_cast<Derived &>(*this));
		return r;
	}
    
    friend bool operator == (Derived const & lhs, Derived const & rhs)
    {
        return Derived::equals(lhs, rhs);
    }

    friend bool operator != (Derived const & lhs, Derived const & rhs)
    {
        return ! Derived::equals(lhs, rhs);
    }
};

template <typename Derived, typename T, typename Pointer, typename Reference, typename Distance>
struct iterator_facade<output_iterator_tag, Derived, T, Pointer, Reference, Distance> 
{
    typedef output_iterator_tag iterator_category;
    typedef T                   value_type;
    typedef Distance            difference_type;
    typedef Pointer             pointer;
    typedef Reference           reference;

    static reference ref (Derived &);
    static void increment (Derived &) {}

    reference operator * () const
    {
    	return Derived::ref(*const_cast<Derived *>(static_cast<Derived const *>(this)));
    }
    
    Derived & operator ++ () // prefix increment
	{
    	Derived::increment(static_cast<Derived &>(*this));
    	return *this;
	}

    Derived operator ++ (int) // postfix increment
	{
        Derived r(static_cast<Derived &>(*this));
        Derived::increment(static_cast<Derived &>(*this));
		return r;
	}
};

//
// ForwardIterator:
//      * satisfies InputIterator
//      * satisfies DefaultConstructible
//      * Objects provide multipass guarantee
//      * 
template <typename Derived, typename T, typename Pointer, typename Reference, typename Distance>
struct iterator_facade<forward_iterator_tag, Derived, T, Pointer, Reference, Distance>
        : public iterator_facade<input_iterator_tag, Derived, T, Pointer, Reference, Distance>
{
    typedef forward_iterator_tag iterator_category;
    typedef T                    value_type;
    typedef Distance             difference_type;
    typedef Pointer              pointer;
    typedef Reference            reference;
    typedef const Reference      const_reference;

    iterator_facade () {}
    
    reference operator * ()
    {
    	return Derived::ref(*static_cast<Derived *>(this));
    }

    const_reference operator * () const
    {
    	return Derived::ref(*static_cast<Derived const *>(this));
    }
};

//template <typename Iterator>
//struct iterator_facade<bidirectional_iterator_tag, Iterator> 
//        : public iterator_facade<forward_iterator_tag, Iterator>
//{
//    typedef Iterator iterator_type;
//    
//    iterator_facade & operator -- () // prefix decrement
//	{
//    	iterator_type::decrement(*this);
//    	return *this;
//	}
//
//    iterator_facade operator -- (int) // postfix decrement
//	{
//        iterator_facade r(*this);
//        iterator_type::decrement(*this);
//		return r;
//	}
//};

//template <typename Iterator>
//struct iterator_facade<random_access_iterator_tag, Iterator> 
//        : public iterator_facade<bidirectional_iterator_tag, Iterator>
//{
//    typedef Iterator iterator_type;
//    typedef typename iterator_traits<Iterator>::reference       reference;
//    typedef typename iterator_traits<Iterator>::difference_type difference_type;
//    
//  	iterator_facade & operator += (difference_type n)
//	{
//    	iterator_type::increment(*this, n);
//    	return *this;
//	}
//
//  	iterator_facade & operator -= (difference_type n)
//	{
//    	iterator_type::decrement(*this, n);
//    	return *this;
//	}
//
//   	reference operator [] (difference_type index)
//	{
//		return iterator_type::subscript(*this, index);
//	}
//
//    friend iterator_type operator + (iterator_type const & i, difference_type n)
//    {
//        iterator_type r(i);
//        iterator_type::increment(r, n);
//        return r;
//    }
//
//    friend iterator_type operator + (difference_type n, iterator_type const & i)
//    {
//        return operator + (i, n);
//    }
//
//    friend iterator_type operator - (iterator_type const & i, difference_type n)
//    {
//        iterator_type r(i);
//        iterator_type::decrement(r, n);
//        return r;
//    }
//
//    friend difference_type operator - (iterator_type const & i1, iterator_type const & i2)
//    {
//        return i1.base() - i2.base();
//    }
//    
//    friend bool operator < (iterator_type const & i1, iterator_type const & i2)
//    {
//        return iterator_type::compare(i1, i2) < 0;
//    }
//
//    friend bool operator > (iterator_type const & i1, iterator_type const & i2)
//    {
//        return iterator_type::compare(i1, i2) > 0;
//    }
//
//    friend bool operator <= (iterator_type const & i1, iterator_type const & i2)
//    {
//        return iterator_type::compare(i1, i2) <= 0;
//    }
//
//    friend bool operator >= (iterator_type const & i1, iterator_type const & i2)
//    {
//        return iterator_type::compare(i1, i2) >= 0;
//    }
//};

template <typename InputIt>
inline typename pfs::iterator_traits<InputIt>::difference_type
distance (InputIt begin, InputIt end)
{
    return std::distance(begin, end);
}

template <typename InputIterator, typename Distance>
inline void
advance (InputIterator & it, Distance n)
{
    return std::advance(it, n);
}

} // pfs

#endif /* __PFS_CXX98_ITERATOR_HPP__ */

