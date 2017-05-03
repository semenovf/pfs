/* 
 * File:   kmvcontainer.hpp
 * Author: wladt
 *
 * Created on May 3, 2017, 8:22 PM
 */

#ifndef __PFS_TRAITS_KMVCONTAINER_HPP__
#define __PFS_TRAITS_KMVCONTAINER_HPP__

#include <pfs/cxxlang.hpp>
#include <pfs/utility.hpp>

namespace pfs {
namespace traits {

template <typename KmvContainerTraits>
class key_multivalue_container_base
{
public:
    typedef KmvContainerTraits                           traits_type;
    typedef typename traits_type::rep_type               rep_type;
    typedef typename traits_type::native_type            native_type;
    typedef typename traits_type::const_native_reference const_native_reference;
    typedef typename traits_type::size_type              size_type;
    typedef typename traits_type::key_type               key_type;
    typedef typename traits_type::mapped_type            mapped_type;
    typedef typename traits_type::iterator               iterator;
    typedef typename traits_type::const_iterator         const_iterator;
    typedef typename traits_type::reverse_iterator       reverse_iterator;
    typedef typename traits_type::const_reverse_iterator const_reverse_iterator;
   
private:
    rep_type _d;
    
public:
	explicit key_multivalue_container_base ()
		: _d()
	{}

    key_multivalue_container_base (key_multivalue_container_base const & rhs)
        : _d(rhs._d)
    {}

    key_multivalue_container_base (const_native_reference rhs)
        : _d(rhs)
    {}
    
    key_multivalue_container_base & operator = (const_native_reference rhs)
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
    
//    iterator erase (iterator first, iterator last)
//    {
//        if (first == begin() && last == end())
//            clear();
//        else
//            while (first != last)
//                erase(first++);
//    }
//	
    void swap (key_multivalue_container_base & rhs)
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

//    pfs::pair<iterator,iterator> equal_range (Key const & key) 
//    {
//        return pfs::pair<iterator,iterator>(lower_bound(key)
//                , upper_bound(key));
//    }
//    
//    pfs::pair<const_iterator,const_iterator> equal_range (Key const & key) const
//    {
//        return pfs::pair<iterator,iterator>(lower_bound(key)
//                , upper_bound(key));
//    }
    
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

template <typename KmvContainerTraits>
inline bool operator == (key_multivalue_container_base<KmvContainerTraits> const & lhs
        , key_multivalue_container_base<KmvContainerTraits> const & rhs)
{
    return static_cast<typename key_multivalue_container_base<KmvContainerTraits>::const_native_reference>(lhs)
            == static_cast<typename key_multivalue_container_base<KmvContainerTraits>::const_native_reference>(rhs);
}

template <typename KmvContainerTraits>
inline bool operator != (key_multivalue_container_base<KmvContainerTraits> const & lhs
        , key_multivalue_container_base<KmvContainerTraits> const & rhs)
{
    return static_cast<typename key_multivalue_container_base<KmvContainerTraits>::const_native_reference>(lhs)
            != static_cast<typename key_multivalue_container_base<KmvContainerTraits>::const_native_reference>(rhs);
}

template <typename KmvContainerTraits>
inline bool operator < (key_multivalue_container_base<KmvContainerTraits> const & lhs
        , key_multivalue_container_base<KmvContainerTraits> const & rhs)
{
    return static_cast<typename key_multivalue_container_base<KmvContainerTraits>::const_native_reference>(lhs)
            < static_cast<typename key_multivalue_container_base<KmvContainerTraits>::const_native_reference>(rhs);
}

template <typename KmvContainerTraits>
inline bool operator <= (key_multivalue_container_base<KmvContainerTraits> const & lhs
        , key_multivalue_container_base<KmvContainerTraits> const & rhs)
{
    return static_cast<typename key_multivalue_container_base<KmvContainerTraits>::const_native_reference>(lhs)
            <= static_cast<typename key_multivalue_container_base<KmvContainerTraits>::const_native_reference>(rhs);
}

template <typename KmvContainerTraits>
inline bool operator > (key_multivalue_container_base<KmvContainerTraits> const & lhs
        , key_multivalue_container_base<KmvContainerTraits> const & rhs)
{
    return static_cast<typename key_multivalue_container_base<KmvContainerTraits>::const_native_reference>(lhs)
            > static_cast<typename key_multivalue_container_base<KmvContainerTraits>::const_native_reference>(rhs);
}

template <typename KmvContainerTraits>
inline bool operator >= (key_multivalue_container_base<KmvContainerTraits> const & lhs
        , key_multivalue_container_base<KmvContainerTraits> const & rhs)
{
    return static_cast<typename key_multivalue_container_base<KmvContainerTraits>::const_native_reference>(lhs)
            >= static_cast<typename key_multivalue_container_base<KmvContainerTraits>::const_native_reference>(rhs);
}

template <typename KmvContainerTraits>
inline void swap (key_multivalue_container_base<KmvContainerTraits> const & lhs
        , key_multivalue_container_base<KmvContainerTraits> const & rhs)
{
    lhs.swap(rhs);
}

template <typename Key
        , typename T
        , template <typename, typename, typename> class KmvContainerT>
struct key_multivalue_container_traits
{
    typedef KmvContainerT<Key, T>                        native_type;
    typedef typename traits_type::rep_type               rep_type;
    typedef typename traits_type::native_type            native_type;
    typedef typename traits_type::const_native_reference const_native_reference;
    typedef typename traits_type::size_type              size_type;
    typedef typename traits_type::key_type               key_type;
    typedef typename traits_type::mapped_type            mapped_type;
    typedef typename traits_type::iterator               iterator;
    typedef typename traits_type::const_iterator         const_iterator;
    typedef typename traits_type::reverse_iterator       reverse_iterator;
    typedef typename traits_type::const_reverse_iterator const_reverse_iterator;
};

template <typename Key
        , typename T
        , template <typename, typename, typename> class KmvContainerT>
class key_multivalue_container 
    : public key_multivalue_container_base<key_multivalue_container_traits<Key, T, KmvContainerT> >
{
    typedef key_multivalue_container_base<key_multivalue_container_traits<Key, T, KmvContainerT> > base_class;

public:
	explicit key_multivalue_container ()
		: base_class()
	{}

    key_multivalue_container (key_multivalue_container const & rhs)
        : base_class(rhs)
    {}

    key_multivalue_container (const_native_reference rhs)
        : base_class(rhs)
    {}
};

}} // pfs::traits

#endif /* __PFS_TRAITS_KMVCONTAINER_HPP__ */

