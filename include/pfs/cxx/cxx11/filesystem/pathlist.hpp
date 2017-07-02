/* 
 * File:   pathlist.hpp
 * Author: wladt
 *
 * Created on June 21, 2017, 2:53 PM
 */

#ifndef __PFS_CXX_CXX11_FILESYSTEM_PATHLIST_HPP__
#define __PFS_CXX_CXX11_FILESYSTEM_PATHLIST_HPP__

#include <list>
#include "path.hpp"

namespace pfs {
namespace filesystem {

using pathlist = std::list<filesystem::path>;

path search_path (path const & p, pathlist const & searchdirs);

}} // pfs::filesystem

#endif /* __PFS_CXX_CXX11_FILESYSTEM_PATHLIST_HPP__ */

