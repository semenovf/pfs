#include "pfs/filesystem.hpp"

namespace pfs {
namespace filesystem {

path search_path (path const & p, pathlist const & searchdirs)
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

}} // pfs::filesystem