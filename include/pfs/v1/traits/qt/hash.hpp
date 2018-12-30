#ifndef __PFS_TRAITS_QT_HASH_HPP__
#define __PFS_TRAITS_QT_HASH_HPP__

#include <QHash>
#include <pfs/traits/qt/container.hpp>

namespace pfs {
namespace qt {

template <typename T>
struct hash_wrapper
{
    typedef QHash<typename T::first_type, typename T::second_type> type;
};

template <typename T>
class hash : public container<T, hash_wrapper>
{
    typedef container<T, hash_wrapper> base_class;
    
public:
    typedef typename base_class::native_type      native_type;
    typedef typename base_class::native_reference native_reference;
    typedef typename base_class::size_type        size_type;
    
    typedef typename pfs::pair<typename T::first_type
            , typename T::second_type>                value_type;
    typedef typename native_type::mapped_type &       reference;
    typedef typename native_type::mapped_type const & const_reference;

public:
    hash (native_reference rhs)
        : base_class(rhs)
    {}
        
    size_type max_size () const noexcept
    {
        return ((INT_MAX)/sizeof(T) - sizeof(native_type)) / 2; // FIXME
    }
};

}} // pfs::qt

#endif /* __PFS_TRAITS_QT_HASH_HPP__ */
