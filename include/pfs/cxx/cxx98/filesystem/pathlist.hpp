/* 
 * File:   pathlist.hpp
 * Author: wladt
 *
 * Created on June 30, 2017, 7:14 PM
 */

#ifndef __PFS_CXX_CXX98_FILESYSTEM_PATHLIST_HPP__
#define __PFS_CXX_CXX98_FILESYSTEM_PATHLIST_HPP__

#include <list>
#include "path.hpp"

namespace pfs {
namespace filesystem {

typedef std::list<filesystem::path> pathlist;

path search_path (path const & p, pathlist const & searchdirs);

}} // pfs::filesystem

#endif /* __PFS_CXX_CXX98_FILESYSTEM_PATHLIST_HPP__ */

