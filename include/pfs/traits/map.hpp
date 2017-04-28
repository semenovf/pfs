/* 
 * File:   map.hpp
 * Author: wladt
 *
 * Created on January 16, 2017, 2:39 PM
 */

#ifndef __PFS_MAP_HPP__
#define __PFS_MAP_HPP__

#include <pfs/cxxlang.hpp>
#include <pfs/iterator.hpp>
#include <pfs/exception.hpp>
#include <pfs/utility.hpp>

namespace pfs {
namespace traits {

template <typename Key, typename T, template <typename, typename> class MapT>
struct map_rep
{
    typedef MapT<Key, T>                                 native_type;
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

template <typename Key, typename T, template <typename, typename> class MapT>
class map
{
public:
    typedef map_rep<Key, T, MapT>                     rep_type;
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
	explicit map ()
		: _d()
	{}

    map (map const & rhs)
        : _d(rhs._d)
    {}

    map (const_native_reference rhs)
        : _d(rhs)
    {}
    
    map & operator = (const_native_reference rhs)
    {
        if (this != & rhs)
            _d = rhs;
        return *this;
    }
    
    operator const_native_reference () const pfs_noexcept
    {
        return _d;
    }

    size_type size () const
    {
        return _d.size();
    }
    
    bool empty () const
    {
        return size() == 0;
    }

    iterator begin ()
    {
        return _d.begin();
    }
		
    const_iterator begin () const
    {
        return _d.begin(_d);
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
        return _d.end(_d);
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
    
    /**
     * @brief  Access to %map data.
     * @param  kye  The key for which data should be retrieved.
     * @return  A reference to the data whose key is equivalent to @a key, if
     *          such a data is present in the %map.
     * @throw  pfs::out_of_range  If no such data is present.
     */
    mapped_type & at (Key const & key)
    {
        return _d.at(key);
    }

    mapped_type const & at (Key const & key) const
    {
        return _d.at(key);
    }
    
    /**
     * @brief  Subscript ( @c [] ) access to %map data.
     * @details Allows for easy lookup with the subscript ( @c [] )
     *          operator. Returns data associated with the key specified in
     *          subscript. If the key does not exist, a pair with that key
     *          is created using default values, which is then returned.
     *          Lookup requires logarithmic time.
     * @param key The key for which data should be retrieved.
     * @return A reference to the data of the (key,data) %pair.
     */
    mapped_type & operator [] (Key const & key)
    {
        return _d[key];
    }
    
    //size_type max_size() const;
    
    /**
     *  @details Erases all elements in a %map. Note that this function only
     *           erases the elements, and that if the elements themselves are
     *           pointers, the pointed-to memory is not touched in any way.
     *           Managing the pointer is the user's responsibility.
     */    
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
	
    size_type count (Key const & key) const
    {
        return _d.count(key);
    }
	
    iterator find (Key const & key)
    {
        return _d.find(key);
    }
		
    const_iterator find (Key const & key) const
    {
        return _d.find(key);
    }
    
    pfs::pair<iterator, bool> insert (Key const & key, T const & value)
    {
        return _d.insert(key, value);
    }

    int compare (map const & rhs) const
    {
        return _d.compare(rhs._d);
    }

    void swap (map & rhs)
    {
        _d.swap(rhs._d);
    }
};

template <typename Key, typename T, template <typename, typename> class MapT>
inline bool operator == (map<Key, T, MapT> const & lhs, map<Key, T, MapT> const & rhs)
{
    return static_cast<typename map<Key, T, MapT>::const_native_reference>(lhs)
            == static_cast<typename map<Key, T, MapT>::const_native_reference>(rhs);
}

template <typename Key, typename T, template <typename, typename> class MapT>
inline bool operator != (map<Key, T, MapT> const & lhs, map<Key, T, MapT> const & rhs)
{
    return static_cast<typename map<Key, T, MapT>::const_native_reference>(lhs) 
            != static_cast<typename map<Key, T, MapT>::const_native_reference>(rhs);
}

template <typename Key, typename T, template <typename, typename> class MapT>
inline bool operator < (map<Key, T, MapT> const & lhs, map<Key, T, MapT> const & rhs)
{
    return static_cast<typename map<Key, T, MapT>::const_native_reference>(lhs) 
            < static_cast<typename map<Key, T, MapT>::const_native_reference>(rhs);
}

template <typename Key, typename T, template <typename, typename> class MapT>
inline bool operator <= (map<Key, T, MapT> const & lhs, map<Key, T, MapT> const & rhs)
{
    return static_cast<typename map<Key, T, MapT>::const_native_reference>(lhs) 
            <= static_cast<typename map<Key, T, MapT>::const_native_reference>(rhs);
}

template <typename Key, typename T, template <typename, typename> class MapT>
inline bool operator > (map<Key, T, MapT> const & lhs, map<Key, T, MapT> const & rhs)
{
    return static_cast<typename map<Key, T, MapT>::const_native_reference>(lhs) 
            > static_cast<typename map<Key, T, MapT>::const_native_reference>(rhs);
}

template <typename Key, typename T, template <typename, typename> class MapT>
inline bool operator >= (map<Key, T, MapT> const & lhs, map<Key, T, MapT> const & rhs)
{
    return static_cast<typename map<Key, T, MapT>::const_native_reference>(lhs) 
            >= static_cast<typename map<Key, T, MapT>::const_native_reference>(rhs);
}

template <typename Key, typename T, template <typename, typename> class MapT>
inline void swap (map<Key, T, MapT> const & lhs
        , map<Key, T, MapT> const & rhs)
{
    lhs.swap(rhs);
}

}} // pfs::traits

#endif /* __PFS_MAP_HPP__ */
