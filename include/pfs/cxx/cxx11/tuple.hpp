#pragma once
#include <tuple>

namespace pfs {

template <typename... Types>
using tuple = std::tuple<Types...>;

template <typename T>
using tuple_size = std::tuple_size<T>;

template <std::size_t I, typename... Types>
using tuple_element = std::tuple_element<I, Types...>;

template <std::size_t I, typename... Types>
inline typename tuple_element<I, pfs::tuple<Types...>>::type & get (pfs::tuple<Types...> & t)
{
    return std::get<I, Types...>(t);
}

template <std::size_t I, typename... Types>
inline typename tuple_element<I, pfs::tuple<Types...>>::type const & get (pfs::tuple<Types...> const & t)
{
    return std::get<I, Types...>(t);
}

template <std::size_t I, typename... Types>
inline typename tuple_element<I, pfs::tuple<Types...>>::type && get (pfs::tuple<Types...> && t)
{
    return std::get<I, Types...>(std::forward<pfs::tuple<Types...>>(t));
}

} // namespace pfs

#include <pfs/cxx/cxx98/tuple_ext.hpp>
