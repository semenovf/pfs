/* 
 * File:   traits.hpp
 * Author: wladt
 *
 * Created on January 23, 2017, 3:27 PM
 */

#ifndef __PFS_FS_TRAITS_HPP__
#define __PFS_FS_TRAITS_HPP__

#include <pfs/traits/list.hpp>
#include <pfs/traits/string.hpp>
#include <pfs/string_builder.hpp>

namespace pfs {
namespace fs {

template <typename StringT
    , template <typename> class ListT>
struct traits
{
    typedef pfs::traits::string<StringT>           string_type;
    typedef typename string_type::value_type       char_type;
    typedef pfs::traits::list<string_type, ListT>  stringlist_type;
    typedef pfs::string_builder<char_type>         string_builder_type;
    
    template <typename T>
    struct list
    {
        typedef pfs::traits::list<T, ListT> type;
    };
    
    static bool is_separator (char_type c)
    {
        return c == char_type('/'); // POSIX API
        // return c == char_type('/') || c == char_type('\\'); // Windows API
    }
    
    // Preferred separator
    static char_type separator ()
    {
        return char_type('/');    // POSIX API
        //return char_type('\\'); // Windows API
    }
    
    class filesystem_error
    {
        string_type _msg;

    public:
        explicit filesystem_error (string_type const & arg);

        string_type const & what () const pfs_noexcept
        {
            return _msg;
        }
    };
};

}} // pfs::cli

#endif /* __PFS_FS_TRAITS_HPP__ */

