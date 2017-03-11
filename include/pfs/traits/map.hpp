/* 
 * File:   map.hpp
 * Author: wladt
 *
 * Created on January 16, 2017, 2:39 PM
 */

#ifndef __PFS_MAP_HPP__
#define __PFS_MAP_HPP__

#include <pfs/iterator.hpp>
#include <pfs/exception.hpp>
#include <pfs/utility.hpp>

// FIXME Reimplement `map` like `vector`

namespace pfs {
namespace traits {

template <typename Foundation, typename Key, typename T>
struct map_traits
{
    typedef struct __Use_Specialized_Traits__ {} native_type;
    typedef native_type const & const_native_reference;
    typedef size_t              size_type;
    typedef Key                 key_type;
    typedef T                   mapped_type;
//    typedef T &                 reference;
//    typedef T const &           const_reference;
    typedef T *                 iterator;
    typedef T const *           const_iterator;
    typedef T *                 reverse_iterator;
    typedef T const *           const_reverse_iterator;
    typedef native_type         data_type;

    static void xassign (data_type & d, const_native_reference lhs);
    static size_type xsize (data_type const & d);
    static iterator xbegin (data_type & d);
    static const_iterator xbegin (data_type const & d);
    static iterator xend (data_type & d);
    static const_iterator xend (data_type const & d);
    static reverse_iterator xrbegin (data_type & d);
    static const_reverse_iterator xrbegin (data_type const & d);
    static reverse_iterator xrend (data_type & d);
    static const_reverse_iterator xrend (data_type const & d);
    static mapped_type & xat (data_type & d, Key const & key);
    static mapped_type const & xat (data_type const & d, Key const & key);
    static mapped_type & xsubscript (data_type & d, Key const & key);
    static void xclear (data_type & d);
    static iterator xerase (data_type & d, iterator position);
    static iterator xerase (data_type & d, iterator first, iterator last);
    static size_type xcount (data_type const & d, Key const & key);
    static iterator xfind (data_type & d, Key const & key);
    static const_iterator xfind (data_type const & d, Key const & key);
    static pfs::pair<iterator, bool> xinsert (data_type & d
            , Key const & key
            , T const & value);
    static void xswap (data_type & lhs, data_type & rhs);
    static const_native_reference xcast (data_type const & d);
};

template <typename Foundation, typename Key, typename T>
class map
{
public:
    typedef map_traits<Foundation, Key, T>               traits_type;
    typedef typename traits_type::native_type            native_type;
    typedef typename traits_type::const_native_reference const_native_reference;
    typedef typename traits_type::size_type              size_type;
    typedef typename traits_type::key_type               key_type;
    typedef typename traits_type::mapped_type            mapped_type;
//    typedef typename traits_type::reference              reference;
//    typedef typename traits_type::const_reference        const_reference;
    typedef typename traits_type::iterator               iterator;
    typedef typename traits_type::const_iterator         const_iterator;
    typedef typename traits_type::reverse_iterator       reverse_iterator;
    typedef typename traits_type::const_reverse_iterator const_reverse_iterator;
    typedef typename traits_type::data_type              data_type;
    
private:
    data_type _d;
   
public:
	explicit map ()
		: _d()
	{}

	map (map const & rhs)
        : _d(rhs._d)
	{}
    
    explicit map (const_native_reference rhs)
        : _d(_d)
    {}
    
//    template <typename InputIt>
//    map (InputIt first, InputIt last);
    
    map & operator = (map const & rhs)
    {
        if (this != & rhs)
            traits_type::xassign(_d, traits_type::xcast(rhs._d));
        return *this;
    }

    map & operator = (const_native_reference rhs)
    {
        traits_type::xassign(_d, rhs);
        return *this;
    }

    size_type size () const
    {
        return traits_type::xsize(_d);
    }
    
    bool empty () const
    {
        return size() == 0;
    }

    iterator begin ()
    {
        return traits_type::xbegin(_d);
    }
		
    const_iterator begin () const
    {
        return traits_type::xbegin(_d);
    }
		
    const_iterator cbegin () const
    {
        return traits_type::xbegin(_d);
    }
    
    iterator end ()
    {
        return traits_type::xend(_d);
    }
		
    const_iterator end () const
    {
        return traits_type::xend(_d);
    }
		
    const_iterator cend () const
    {
        return traits_type::xend(_d);
    }
    
    reverse_iterator rbegin ()
    {
        return traits_type::xrbegin(_d);
    }
		
    const_reverse_iterator rbegin () const
    {
        return traits_type::xrbegin(_d);
    }
		
    const_reverse_iterator crbegin () const
    {
        return traits_type::xrbegin(_d);
    }
    
    reverse_iterator rend ()
    {
        return traits_type::xrend(_d);
    }
		
    const_reverse_iterator rend () const
    {
        return traits_type::xrend(_d);
    }
		
    const_reverse_iterator crend () const
    {
        return traits_type::xrend(_d);
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
        return traits_type::xat(_d, key);
    }

    mapped_type const & at (Key const & key) const
    {
        return traits_type::xat(_d, key);
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
        return traits_type::xsubscript(_d, key);
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
        traits_type::xclear(_d);
    }
	
    // 
    // Leave C++17-style signature only as it compatible to prior-C++11 and later
    //
    iterator erase (iterator position)
    {
        return traits_type::xerase(_d, position);
    }

    iterator erase (iterator first, iterator last)
    {
        return traits_type::xerase(_d, first, last);
    }
	
    size_type count (Key const & key) const
    {
        return traits_type::xcount(_d, key);
    }
	
    iterator find (Key const & key)
    {
        return traits_type::xfind(_d, key);
    }
		
    const_iterator find (Key const & key) const
    {
        return traits_type::xfind(_d, key);
    }
    
    pfs::pair<iterator, bool> insert (Key const & key, T const & value)
    {
        return traits_type::xinsert(_d, key, value);
    }

    void swap (map & rhs)
    {
        traits_type::xswap(_d, rhs._d);
    }
    
    const_native_reference cast () const
    {
        return traits_type::xcast(_d);
    }
};

template <typename Foundation, typename Key, typename T>
inline bool operator == (map<Foundation, Key, T> const & lhs
        , map<Foundation, Key, T> const & rhs)
{
    return lhs.cast() == rhs.cast();
}

template <typename Foundation, typename Key, typename T>
inline bool operator != (map<Foundation, Key, T> const & lhs
        , map<Foundation, Key, T> const & rhs)
{
    return ! operator == (lhs, rhs);
}

template <typename Foundation, typename Key, typename T>
inline void swap (map<Foundation, Key, T> const & lhs
        , map<Foundation, Key, T> const & rhs)
{
    lhs.swap(rhs);
}

}} // pfs::traits

#endif /* __PFS_MAP_HPP__ */
