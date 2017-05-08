/* 
 * File:   map.hpp
 * Author: wladt
 *
 * Created on May 8, 2017
 */

#ifndef __PFS_TRAITS_QT_MAP_HPP__
#define __PFS_TRAITS_QT_MAP_HPP__

#include <QMap>
#include <pfs/traits/qt/container.hpp>

namespace pfs {
namespace traits {
namespace qt {

template <typename T>
struct map_wrapper
{
    typedef QMap<typename T::first_type, typename T::second_type> type;
};

template <typename T>
class map : public container<T, map_wrapper>
{
    typedef container<T, map_wrapper> base_class;
    
public:
    typedef typename base_class::native_type      native_type;
    typedef typename base_class::native_reference native_reference;
    typedef typename base_class::size_type        size_type;
    
    typedef typename pfs::pair<typename T::first_type
            , typename T::second_type>                value_type;
    typedef typename native_type::mapped_type &       reference;
    typedef typename native_type::mapped_type const & const_reference;

public:
    map (native_reference rhs)
        : base_class(rhs)
    {}
        
    size_type max_size () const pfs_noexcept
    {
        return ((INT_MAX)/sizeof(T) - sizeof(native_type)) / 2; // FIXME
    }
};

}}} // pfs::traits::qt


#if __OBSOLETE__

#include <pfs/cxxlang.hpp>
#include <pfs/traits/map.hpp>

namespace pfs {
namespace traits {

namespace qt {

template <typename Key, typename T>
struct map_iterator : public QMap<Key, T>::iterator
{
    typedef typename QMap<Key, T>::iterator base_type;

    map_iterator () pfs_noexcept
        : base_type()
    {}

    map_iterator (base_type lhs) pfs_noexcept
        : base_type(lhs)
    {}
};

template <typename Key, typename T>
struct map_const_iterator : public QMap<Key, T>::const_iterator
{
    typedef typename QMap<Key, T>::const_iterator base_type;

    map_const_iterator () pfs_noexcept
        : base_type()
    {}
    
    map_const_iterator (base_type lhs) pfs_noexcept
        : base_type(lhs)
    {}
};

} // qt

template <typename Key, typename T>
struct map_rep<Key, T, QMap> : public QMap<Key, T>
{
    typedef QMap<Key, T> base_class;
    typedef QMap<Key, T> native_type;

    typedef native_type const &                            const_native_reference;
    typedef typename native_type::size_type                size_type;
    typedef typename native_type::key_type                 key_type;
    typedef typename native_type::mapped_type              mapped_type;
    typedef typename qt::map_iterator<Key, T>              iterator;
    typedef typename qt::map_const_iterator<Key, T>        const_iterator;
    typedef typename std::reverse_iterator<iterator>       reverse_iterator;
    typedef typename std::reverse_iterator<const_iterator> const_reverse_iterator;
    
	explicit map_rep ()
		: base_class()
	{}

    map_rep (const_native_reference rhs)
        : base_class(rhs)
    {}

    iterator erase (iterator position)
    {
#if __cplusplus >= 201103L
        return base_class::erase(position);
#else
        base_class::erase(position++);
        return position;
#endif        
    }

    iterator erase (iterator first, iterator last)
    {
#if __cplusplus >= 201103L
        return base_class::erase(first, last);
#else
        base_class::erase(first, last);
        return last;
#endif
    }
    
    mapped_type & at (Key const & key)
    {
        iterator it = this->find(key);
        if (it == this->end())
            throw out_of_range("map::at");
        return it.value();
    }

    mapped_type const & at (Key const & key) const
    {
        const_iterator it = this->find(key);
        if (it == this->end())
            throw out_of_range("map::at");
        return it.value();
    }
};
//    
//    virtual pfs::pair<iterator, bool> xinsert (Key const & key, T const & value)
//    {
//        iterator it = iterator(this->_d.insert(key, value));
//        return pfs::pair<iterator, bool>(it, true);
//    }
//};

}} // pfs::traits

#endif

#endif /* __PFS_TRAITS_QT_MAP_HPP__ */
