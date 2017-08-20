/* 
 * File:   directory_entry.hpp
 * Author: wladt
 *
 * Created on August 20, 2017, 11:20 AM
 */

#ifndef __PFS_FILESYSTEM_DIRECTORY_ENTRY_HPP__
#define __PFS_FILESYSTEM_DIRECTORY_ENTRY_HPP__

#include <pfs/cxxversion.hpp>
#include PFS_CXX_HEADER(filesystem/directory_entry)
#include PFS_CXX_HEADER(system_error)

namespace pfs {
namespace filesystem {

template <typename Impl>
class basic_directory_entry
{
    Impl _d;
    
public:
    basic_directory_entry (Impl const & d)
        : _d(d)
    {}

    basic_directory_entry () pfs_noexcept
        : _d()
    {}

#if __cplusplus >= 201103L    
    basic_directory_entry (basic_directory_entry const &) = default;
    basic_directory_entry (basic_directory_entry && ) noexcept = default;
#else
    basic_directory_entry (basic_directory_entry const & d)
        : _d(d._d)
    {}
#endif    

    explicit basic_directory_entry (pfs::filesystem::path const & p)
        : _d(p.impl())
    {}
    
    basic_directory_entry (pfs::filesystem::path const & p, pfs::error_code & ec)
        : _d(p.impl(), ec)
    {}
};

typedef basic_directory_entry<details::directory_entry> directory_entry;

}} // pfs::filesystem

#endif /* __PFS_FILESYSTEM_DIRECTORY_ENTRY_HPP__ */

