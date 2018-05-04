#pragma once
#include <pfs/cxxlang.hpp>
#include <pfs/type_traits.hpp>
#include <pfs/memory.hpp>

namespace boost
{

template <typename T>
T const * get_pointer (pfs::shared_ptr<T> const & p)
{
    return p.get();
}

template <typename T>
T * get_pointer (pfs::shared_ptr<T> & p)
{
    return p.get();
}

template <typename T>
T const * get_pointer (pfs::unique_ptr<T> const & p)
{
    return p.get();
}

template <typename T>
T * get_pointer (pfs::unique_ptr<T> & p)
{
    return p.get();
}

} // namespace boost

#include <boost/bind.hpp>

namespace pfs {

namespace placeholders {

//#include <boost/bind/placeholders.hpp>

} // placeholders

using boost::bind;

} // namespace pfs


