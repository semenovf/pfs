/* 
 * File:   traits.hpp
 * Author: wladt
 *
 * Created on January 23, 2017, 3:27 PM
 */

#ifndef __PFS_CLI_TRAITS_HPP__
#define __PFS_CLI_TRAITS_HPP__

#include <pfs/traits/list.hpp>
#include <pfs/traits/multimap.hpp>
#include <pfs/traits/string.hpp>

namespace pfs {
namespace cli {

template <typename StringT
    , template <typename> class ListT
    , template <typename, typename> class MultiMapT>
struct traits
{
    typedef traits::string<StringT>          string_type;
    typedef typename string_type::value_type char_type;
    typedef traits::list<string_type, ListT> domain_list_type;
};

}}} // pfs::cli

#endif /* __PFS_CLI_TRAITS_HPP__ */

