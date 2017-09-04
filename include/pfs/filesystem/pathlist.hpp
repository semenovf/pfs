/* 
 * File:   pathlist.hpp
 * Author: wladt
 *
 * Created on August 20, 2017, 10:36 AM
 */

#ifndef __PFS_FILESYSTEM_PATHLIST_HPP__
#define __PFS_FILESYSTEM_PATHLIST_HPP__

#include <pfs/traits/stdcxx/list.hpp>
#include <pfs/filesystem/path.hpp>

namespace pfs {
namespace filesystem {

class pathlist : public stdcxx::list<path>
{
public:
    static path search_path (path const & p, pathlist const & searchdirs)
    {
        if (p.empty())
            return path();

        if (p.is_absolute())
            return p;

        for (pathlist::const_iterator i = searchdirs.cbegin()
                ; i != searchdirs.cend(); ++i) {

            path p1(*i);
            p1 /= p;

            if (exists(p1))
                return p1;
        }

        return path();
    }
};

inline path search_path (path const & p, pathlist const & searchdirs)
{
    return pathlist::search_path(p, searchdirs);
}

}} // pfs::filesystem

#endif /* __PFS_FILESYSTEM_PATHLIST_HPP__ */

