#ifndef __PFS_TYPE_TRAITS_HPP__
#define __PFS_TYPE_TRAITS_HPP__

#include <pfs/cxxversion.hpp>
#include PFS_CXX_HEADER(type_traits)

namespace pfs {

template <bool, typename T1, typename>
struct if_else
{
    typedef T1 type;
};

template <typename T1, typename T2>
struct if_else<false, T1, T2>
{
    typedef T2 type;
};

}


#endif /* __PFS_TYPE_TRAITS_HPP__ */
