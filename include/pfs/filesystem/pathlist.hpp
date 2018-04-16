#pragma once
#include <pfs/list.hpp>
#include <pfs/filesystem/path.hpp>

namespace pfs {
namespace filesystem {

class pathlist : public list
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

            error_code ec;

            if (exists(p1, ec))
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
