/* 
 * File:   multimap.hpp
 * Author: wladt
 *
 * Created on January 20, 2017, 3:54 PM
 */

// XXX DEPRECATED, use kmvcontainer.hpp

#ifndef __PFS_TRAITS_MULTIMAP_HPP__
#define __PFS_TRAITS_MULTIMAP_HPP__

#include <pfs/cxxlang.hpp>
#include <pfs/iterator.hpp>
#include <pfs/exception.hpp>
#include <pfs/utility.hpp>

namespace pfs {
namespace traits {

template <typename Key, typename T, template <typename, typename> class MultiMapT>
struct multimap_rep
{
    typedef MultiMapT<Key, T>                            native_type;
    typedef native_type const &                          const_native_reference;
    typedef typename native_type::size_type              size_type;
    typedef typename native_type::key_type               key_type;
    typedef typename native_type::mapped_type            mapped_type;
    typedef typename native_type::iterator               iterator;
    typedef typename native_type::const_iterator         const_iterator;
    typedef typename native_type::reverse_iterator       reverse_iterator;
    typedef typename native_type::const_reverse_iterator const_reverse_iterator;
    
    native_type d;
};

template <typename Key, typename T, template <typename, typename> class MultiMapT>
class multimap
{
public:
    typedef multimap_rep<Key, T, MultiMapT>           rep_type;
    typedef typename rep_type::native_type            native_type;
    typedef typename rep_type::const_native_reference const_native_reference;
    typedef typename rep_type::size_type              size_type;
    typedef typename rep_type::key_type               key_type;
    typedef typename rep_type::mapped_type            mapped_type;
    typedef typename rep_type::iterator               iterator;
    typedef typename rep_type::const_iterator         const_iterator;
    typedef typename rep_type::reverse_iterator       reverse_iterator;
    typedef typename rep_type::const_reverse_iterator const_reverse_iterator;
   
private:
    rep_type _d;
    
public:
	explicit multimap ()
		: _d()
	{}

    multimap (multimap const & rhs)
        : _d(rhs._d)
    {}

    multimap (const_native_reference rhs)
        : _d(rhs)
    {}
    
    multimap & operator = (const_native_reference rhs)
    {
        if (this != & rhs)
            _d = rhs;
        return *this;
    }
    
    operator const_native_reference () const pfs_noexcept
    {
        return _d;
    }
    
    iterator begin () pfs_noexcept
    {
        return _d.begin();
    }
		
    const_iterator begin () const pfs_noexcept
    {
        return _d.begin();
    }
		
    const_iterator cbegin () const pfs_noexcept
    {
        return _d.begin();
    }
    
    iterator end () pfs_noexcept
    {
        return _d.end();
    }
		
    const_iterator end () const pfs_noexcept
    {
        return _d.end();
    }
		
    const_iterator cend () const pfs_noexcept
    {
        return _d.end();
    }
    
    reverse_iterator rbegin () pfs_noexcept
    {
        return _d.rbegin();
    }
		
    const_reverse_iterator rbegin () const pfs_noexcept
    {
        return _d.rbegin();
    }
		
    const_reverse_iterator crbegin () const pfs_noexcept
    {
        return rbegin();
    }
    
    reverse_iterator rend () pfs_noexcept
    {
        return _d.rend();
    }
		
    const_reverse_iterator rend () const pfs_noexcept
    {
        return _d.rend();
    }
		
    const_reverse_iterator crend () const pfs_noexcept
    {
        return rend();
    }
    
    size_type size () const pfs_noexcept
    {
        return _d.size();
    }
    
    bool empty () const pfs_noexcept
    {
        return size() == 0;
    }
	
    void clear ()
    {
        _d.clear();
    }
	
    // 
    // Leave C++17-style signature only as it compatible to prior-C++11 and later
    //
    iterator erase (iterator position)
    {
        return _d.erase(position);
    }

    iterator erase (iterator first, iterator last)
    {
        return _d.erase(first, last);
    }
    
    void swap (multimap & rhs)
    {
        _d.swap(rhs._d);
    }

    /**
     * @brief Returns the number of elements associated with key @a key.
     * @param key Key.
     * @return The number of elements associated with key @a key.
     */
    size_type count (key_type const & key) const
    {
        return _d.count(key);
    }
	
    iterator find (key_type const & key)
    {
        return _d.find(key);
    }
		
    const_iterator find (key_type const & key) const
    {
        return _d.find(key);
    }
    
    pfs::pair<iterator,iterator> equal_range (key_type const & key)
    {
        return _d.equal_range(key);
    }

    std::pair<const_iterator,const_iterator> equal_range (key_type const & key ) const
    {
        return _d.equal_range(key);
    }
    
    iterator lower_bound (key_type const & key)
    {
        return _d.lower_bound(key);
    }

    const_iterator lower_bound (key_type const & key) const
    {
        return _d.lower_bound(key);
    }
    
    iterator upper_bound (key_type const & key)
    {
        return _d.upper_bound(key);
    }

    const_iterator upper_bound (key_type const & key) const
    {
        return _d.upper_bound(key);
    }
    
    iterator insert (key_type const & key, mapped_type const & value)
    {
        return _d.insert(key, value);
    }
    
//    template <typename InputIt>
//    void insert (InputIt first, InputIt last);
};

template <typename Key, typename T, template <typename, typename> class MultiMapT>
inline bool operator == (multimap<Key, T, MultiMapT> const & lhs, multimap<Key, T, MultiMapT> const & rhs)
{
    return static_cast<typename multimap<Key, T, MultiMapT>::const_native_reference>(lhs)
            == static_cast<typename multimap<Key, T, MultiMapT>::const_native_reference>(rhs);
}

template <typename Key, typename T, template <typename, typename> class MultiMapT>
inline bool operator != (multimap<Key, T, MultiMapT> const & lhs, multimap<Key, T, MultiMapT> const & rhs)
{
    return static_cast<typename multimap<Key, T, MultiMapT>::const_native_reference>(lhs)
            != static_cast<typename multimap<Key, T, MultiMapT>::const_native_reference>(rhs);
}

template <typename Key, typename T, template <typename, typename> class MultiMapT>
inline bool operator < (multimap<Key, T, MultiMapT> const & lhs, multimap<Key, T, MultiMapT> const & rhs)
{
    return static_cast<typename multimap<Key, T, MultiMapT>::const_native_reference>(lhs)
            < static_cast<typename multimap<Key, T, MultiMapT>::const_native_reference>(rhs);
}

template <typename Key, typename T, template <typename, typename> class MultiMapT>
inline bool operator <= (multimap<Key, T, MultiMapT> const & lhs, multimap<Key, T, MultiMapT> const & rhs)
{
    return static_cast<typename multimap<Key, T, MultiMapT>::const_native_reference>(lhs)
            <= static_cast<typename multimap<Key, T, MultiMapT>::const_native_reference>(rhs);
}

template <typename Key, typename T, template <typename, typename> class MultiMapT>
inline bool operator > (multimap<Key, T, MultiMapT> const & lhs, multimap<Key, T, MultiMapT> const & rhs)
{
    return static_cast<typename multimap<Key, T, MultiMapT>::const_native_reference>(lhs)
            > static_cast<typename multimap<Key, T, MultiMapT>::const_native_reference>(rhs);
}

template <typename Key, typename T, template <typename, typename> class MultiMapT>
inline bool operator >= (multimap<Key, T, MultiMapT> const & lhs, multimap<Key, T, MultiMapT> const & rhs)
{
    return static_cast<typename multimap<Key, T, MultiMapT>::const_native_reference>(lhs)
            >= static_cast<typename multimap<Key, T, MultiMapT>::const_native_reference>(rhs);
}

template <typename Key, typename T, template <typename, typename> class MultiMapT>
inline void swap (multimap<Key, T, MultiMapT> const & lhs, multimap<Key, T, MultiMapT> const & rhs)
{
    lhs.swap(rhs);
}

}} // pfs::traits

#endif /* __PFS_TRAITS_MULTIMAP_HPP__ */

