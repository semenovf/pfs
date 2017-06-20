/* 
 * File:   filesystem.hpp
 * Author: wladt
 *
 * Created on June 20, 2017, 11:05 AM
 */

#ifndef __PFS_FILESYSTEM_HPP__
#define __PFS_FILESYSTEM_HPP__

#include <pfs/cxxversion.hpp>
#include PFS_CXX_HEADER(filesystem)

namespace pfs {
namespace filesystem {

/** 
 * @class path
 * @brief Objects of type path represent paths on a filesystem.
 *
 * @details Only syntactic aspects of paths are handled: 
 * the pathname may represent a non-existing path or even 
 * one that is not allowed to exist on the current file system or OS...
 * Details see for [std::filesystem::path](http://en.cppreference.com/w/cpp/filesystem/path)
 */

}} // pfs::filesystem

#endif /* __PFS_FILESYSTEM_HPP__ */
