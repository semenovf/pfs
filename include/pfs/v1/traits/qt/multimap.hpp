#ifndef __PFS_TRAITS_QT_MULTIMAP_HPP__
#define __PFS_TRAITS_QT_MULTIMAP_HPP__

#include <QMultiMap>

namespace pfs {
namespace qt {

//template <typename T>
//struct multimap_wrapper
//{
//    typedef QMultiMap<typename T::first_type, typename T::second_type> type;
//};
//
//template <typename T>
//class multimap : public container<T, multimap_wrapper>
//{
//    typedef container<T, multimap_wrapper> base_class;
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
//    multimap (native_reference rhs)
//        : base_class(rhs)
//    {}
//        
//    size_type max_size () const pfs_noexcept
//    {
//        return ((INT_MAX)/sizeof(T) - sizeof(native_type)) / 2; // FIXME
//    }
//};

}} // pfs::qt

#endif /* __PFS_TRAITS_QT_MULTIMAP_HPP__ */

