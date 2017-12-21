#ifndef __PFS_TRAITS_QT_MULTIHASH_HPP__
#define __PFS_TRAITS_QT_MULTIHASH_HPP__

#include <QHash>
#include <pfs/traits/qt/container.hpp>

namespace pfs {
namespace qt {

//template <typename T>
//struct multihash_wrapper
//{
//    typedef QMultiHash<typename T::first_type, typename T::second_type> type;
//};
//
//template <typename T>
//class multihash : public container<T, multihash_wrapper>
//{
//    typedef container<T, multihash_wrapper> base_class;
//    
//public:
//    typedef typename base_class::native_type      native_type;
//    typedef typename base_class::native_reference native_reference;
//    typedef typename base_class::size_type        size_type;
//    
//    typedef typename pfs::pair<typename T::first_type
//            , typename T::second_type>                value_type;
//    typedef typename native_type::mapped_type &       reference;
//    typedef typename native_type::mapped_type const & const_reference;
//
//public:
//    multihash (native_reference rhs)
//        : base_class(rhs)
//    {}
//        
//    size_type max_size () const pfs_noexcept
//    {
//        return ((INT_MAX)/sizeof(T) - sizeof(native_type)) / 2; // FIXME
//    }
//};

}} // pfs::qt

#if __COMMENT__
#include <pfs/traits/multimap.hpp>

namespace pfs {
namespace traits {

namespace qt {

template <typename Key, typename T>
struct multimap_iterator : public QMultiMap<Key, T>::iterator
{
    typedef typename QMultiMap<Key, T>::iterator base_type;

    multimap_iterator (base_type lhs) pfs_noexcept
        : base_type(lhs)
    {}
};

template <typename Key, typename T>
struct multimap_const_iterator : public QMultiMap<Key, T>::const_iterator
{
    typedef typename QMultiMap<Key, T>::const_iterator base_type;

    multimap_const_iterator (base_type lhs) pfs_noexcept
        : base_type(lhs)
    {}
};

} // qt

template <typename Key, typename T>
struct multimap_rep<Key, T, QMultiMap> : public QMultiMap<Key, T>
{
    typedef QMultiMap<Key, T> base_class;
    typedef QMultiMap<Key, T> native_type;

    typedef native_type const &                            const_native_reference;
    typedef typename native_type::size_type                size_type;
    typedef typename native_type::key_type                 key_type;
    typedef typename native_type::mapped_type              mapped_type;
    typedef typename qt::map_iterator<Key, T>              iterator;
    typedef typename qt::map_const_iterator<Key, T>        const_iterator;    
    typedef typename std::reverse_iterator<iterator>       reverse_iterator;
    typedef typename std::reverse_iterator<const_iterator> const_reverse_iterator;
    
	explicit multimap_rep ()
		: base_class()
	{}

    multimap_rep (const_native_reference rhs)
        : base_class(rhs)
    {}
};

//template <typename Key, typename T>
//class basic_multimap<Key, T, ::qt::multimap>
//    : public details::basic_multimap<Key, T, ::qt::multimap>
//{
//    typedef details::basic_multimap<Key, T, ::qt::multimap> base_type;
//    typedef basic_multimap<Key, T, ::qt::multimap>          self_type;
//    
//protected:
//    typedef typename base_type::size_type              size_type;
//    typedef typename base_type::key_type               key_type;
//    typedef typename base_type::mapped_type            mapped_type;
//    typedef typename base_type::native_type            native_type;
//    typedef typename base_type::iterator               iterator;
//    typedef typename base_type::const_iterator         const_iterator;
//    typedef typename base_type::reverse_iterator       reverse_iterator;
//    typedef typename base_type::const_reverse_iterator const_reverse_iterator;
//    
//protected:

//};

}} // pfs::traits

#endif

#endif /* __PFS_TRAITS_QT_MULTIHASH_HPP__ */

