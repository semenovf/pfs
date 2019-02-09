#pragma once
#include <pfs/type_traits.hpp>
#include <pfs/string.hpp>
#include <pfs/exception.hpp>

namespace pfs {

#if __cplusplus >= 201103L

template <typename EnumT, bool = pfs::is_enum<EnumT>::value>
struct enum_helper
{
    using underlying_type = typename std::underlying_type<EnumT>::type;
    using type = EnumT;
};

template <typename EnumT>
struct enum_helper<EnumT, false>
{
    using underlying_type = typename std::underlying_type<typename EnumT::value_enum>::type;
    using type = typename EnumT::value_enum;
};

#else

// Rough approximation to enum using integer type
template <typename EnumT, bool = pfs::is_enum<EnumT>::value>
struct enum_helper
{
    typedef typename size_type_n<sizeof(EnumT)>::signed_type underlying_type;
    typedef EnumT type;
};

template <typename EnumT>
struct enum_helper<EnumT, false>
{
    typedef typename size_type_n<sizeof(typename EnumT::value_enum)>::signed_type underlying_type;
    typedef typename EnumT::value_enum type;
};

#endif

template <typename EnumT>
typename enum_helper<EnumT>::type make_enum (typename enum_helper<EnumT>::underlying_type value
        , bool & ok);

template <typename EnumT>
inline typename enum_helper<EnumT>::type make_enum (typename enum_helper<EnumT>::underlying_type value)
{
    bool ok = true;
    typename enum_helper<EnumT>::type result = make_enum<EnumT>(value, ok);

    if (!ok) PFS_THROW(pfs::out_of_range("make_enum()"));

    return result;
}

template <typename EnumT>
typename enum_helper<EnumT>::type make_enum (string const & s, bool & ok);

template <typename EnumT>
inline typename enum_helper<EnumT>::type make_enum (string const & s)
{
    bool ok = true;
    typename enum_helper<EnumT>::type result = make_enum<EnumT>(s, ok);

    if (!ok) PFS_THROW(pfs::out_of_range("make_enum()"));

    return result;
}

template <typename EnumT>
string to_string (typename enum_helper<EnumT>::type value);

} // namespace pfs

