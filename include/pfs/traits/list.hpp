/* 
 * File:   list.hpp
 * Author: wladt
 *
 * Created on January 16, 2017, 12:08 PM
 */

#ifndef __PFS_TRAITS_LIST_HPP__
#define __PFS_TRAITS_LIST_HPP__

#if __OBSOLETE__

#include <pfs/exception.hpp>
#include <pfs/cxxlang.hpp>

namespace pfs {
namespace traits {

template <typename T, template <typename> class ListT>
struct list_rep
{
    typedef ListT<T>                                     native_type;
    typedef native_type const &                          const_native_reference;
    typedef typename native_type::size_type              size_type;
    typedef typename native_type::value_type             value_type;
    typedef typename native_type::reference              reference;
    typedef typename native_type::const_reference        const_reference;
    typedef typename native_type::iterator               iterator;
    typedef typename native_type::const_iterator         const_iterator;
    typedef typename native_type::reverse_iterator       reverse_iterator;
    typedef typename native_type::const_reverse_iterator const_reverse_iterator;
//    
//    native_type d;
};

template <typename T, template <typename> class ListT>
class list
{
public:
    typedef list_rep<T, ListT>                        rep_type;
    typedef typename rep_type::native_type            native_type;
    typedef typename rep_type::const_native_reference const_native_reference;
    typedef typename rep_type::size_type              size_type;
    typedef typename rep_type::value_type             value_type;
    typedef typename rep_type::reference              reference;
    typedef typename rep_type::const_reference        const_reference;
    typedef typename rep_type::iterator               iterator;
    typedef typename rep_type::const_iterator         const_iterator;
    typedef typename rep_type::reverse_iterator       reverse_iterator;
    typedef typename rep_type::const_reverse_iterator const_reverse_iterator;
    
private:
    rep_type _d;
   
public:
	list ()
		: _d()
	{}

    list (size_type count, T const & value)
        : _d(count, value)
    {}
    
    explicit list (size_type count)
        : _d(count)
    {}
    
    template <typename InputIt>
    list (InputIt first, InputIt last)
    {
        for (; first != last; ++first)
            push_back(*first);
    }
    
    explicit list (const_native_reference rhs)
        : _d(rhs)
    {}

    list & operator = (list const & rhs)
    {
        if (this != & rhs)
            _d = rhs._d;
        return *this;
    }

    list & operator = (const_native_reference rhs)
    {
        _d = rhs;
        return *this;
    }

    operator const_native_reference () const pfs_noexcept
    {
        return _d;
    }
    
    iterator begin ()
    {
        return _d.begin();
    }
		
    const_iterator begin () const
    {
        return _d.begin();
    }
		
    const_iterator cbegin () const
    {
        return _d.begin();
    }
    
    iterator end ()
    {
        return _d.end();
    }
		
    const_iterator end () const
    {
        return _d.end();
    }
		
    const_iterator cend () const
    {
        return _d.end();
    }
    
    reverse_iterator rbegin ()
    {
        return _d.rbegin();
    }
		
    const_reverse_iterator rbegin () const
    {
        return _d.rbegin();
    }
		
    const_reverse_iterator crbegin () const
    {
        return _d.rbegin();
    }
    
    reverse_iterator rend ()
    {
        return _d.rend();
    }
		
    const_reverse_iterator rend () const
    {
        return _d.rend();
    }
		
    const_reverse_iterator crend () const
    {
        return _d.rend();
    }
    
    reference front ()
    {
        return *begin();
    }
		
    const_reference front () const
    {
        return *begin();
    }
    
    reference back ()
    {
        iterator it = this->end();
        --it; 
        return *it;
    }
		
    const_reference back () const
    {
        const_iterator it = this->end();
        --it; 
        return *it;
    }
    
    size_type size () const pfs_noexcept
    {
        return _d.size();
    }
	
    bool empty () const pfs_noexcept
    {
        return this->size() == 0;
    }
	
    void clear ()
    {
        _d.clear();
    }
    
    iterator erase (iterator pos)
    {
        iterator last(pos);
        ++last;
        return erase(pos, last);
    }

    iterator erase (iterator first, iterator last)
    {
        return _d.erase(first, last);
    }

    void push_back (const_reference value)
    {
        _d.push_back(value);
    }
    
    void pop_back ()
    {
        _d.pop_back();
    }
    
    void push_front (T const & value)
    {
        _d.push_front(value);
    }
		
    void pop_front ()
    {
        _d.pop_front();
    }
	
    void resize (size_type count)
    {
        resize(count, value_type());
    }
    
    void resize (size_type count, value_type const & value)
    {
        _d.resize(count, value);
    }

    void splice (iterator pos, list & rhs)
    {
        _d.splice(pos, rhs._d);
    }
    
    iterator insert (iterator pos, value_type const & value)
    {
        return _d.insert(pos, value);
    }
    
    iterator insert (iterator pos, size_type count, value_type const & value);

    template <typename InputIt>
    iterator insert (iterator pos, InputIt first, InputIt last);

    void swap (list & rhs)
    {
        _d.swap(rhs._d);
    }
};

template <typename T, template <typename> class ListT>
typename list<T, ListT>::iterator 
list<T, ListT>::insert (iterator pos, size_type count, value_type const & value)
{
    if (count) {
        list tmp(count, value);
        this->splice(pos, tmp);
        ++pos;
    }
    return pos;
}

template <typename T, template <typename> class ListT>
template <typename InputIt>
typename list<T, ListT>::iterator
list<T, ListT>::insert (iterator pos, InputIt first, InputIt last)
{
    if (first != last) {
        list tmp(first, last);
        this->splice(pos, tmp);
        ++pos;
    }
    return pos;
}

template < typename T, template <typename> class ListT>
inline bool operator == (list<T, ListT> const & lhs,  list<T, ListT> const & rhs)
{
    return static_cast<typename  list<T, ListT>::const_native_reference>(lhs)
            == static_cast<typename  list<T, ListT>::const_native_reference>(rhs);
}

template < typename T, template <typename> class ListT>
inline bool operator != ( list<T, ListT> const & lhs,  list<T, ListT> const & rhs)
{
    return static_cast<typename  list<T, ListT>::const_native_reference>(lhs) 
            != static_cast<typename  list<T, ListT>::const_native_reference>(rhs);
}

template < typename T, template <typename> class ListT>
inline bool operator < ( list<T, ListT> const & lhs,  list<T, ListT> const & rhs)
{
    return static_cast<typename  list<T, ListT>::const_native_reference>(lhs) 
            < static_cast<typename  list<T, ListT>::const_native_reference>(rhs);
}

template < typename T, template <typename> class ListT>
inline bool operator <= ( list<T, ListT> const & lhs,  list<T, ListT> const & rhs)
{
    return static_cast<typename  list<T, ListT>::const_native_reference>(lhs) 
            <= static_cast<typename  list<T, ListT>::const_native_reference>(rhs);
}

template < typename T, template <typename> class ListT>
inline bool operator > ( list<T, ListT> const & lhs,  list<T, ListT> const & rhs)
{
    return static_cast<typename  list<T, ListT>::const_native_reference>(lhs) 
            > static_cast<typename  list<T, ListT>::const_native_reference>(rhs);
}

template < typename T, template <typename> class ListT>
inline bool operator >= ( list<T, ListT> const & lhs,  list<T, ListT> const & rhs)
{
    return static_cast<typename  list<T, ListT>::const_native_reference>(lhs) 
            >= static_cast<typename  list<T, ListT>::const_native_reference>(rhs);
}

template <typename T, template <typename> class ListT>
inline void swap (list<T, ListT> const & lhs
        , list<T, ListT> const & rhs)
{
    lhs.swap(rhs);
}

}} // pfs::traits

#endif

#endif /* __PFS_TRAITS_LIST_HPP__ */
