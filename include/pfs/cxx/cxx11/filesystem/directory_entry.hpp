/* 
 * File:   directory_entry.hpp
 * Author: wladt
 *
 * Created on June 21, 2017, 11:58 AM
 */

#ifndef __PFS_CXX_CXX11_FILESYSTEM_DIRECTORY_ENTRY_HPP__
#define __PFS_CXX_CXX11_FILESYSTEM_DIRECTORY_ENTRY_HPP__

#if PFS_CC_GCC_VERSION >= 50300

#include <experimental/filesystem>

namespace pfs {
namespace filesystem { 

class directory_entry : public std::experimental::filesystem::directory_entry
{
    typedef std::experimental::filesystem::directory_entry base_class;
    
public:
    directory_entry () noexcept
        : base_class()
    {}
        
    directory_entry (directory_entry const & other)
        : base_class(other)
    {}

    directory_entry (directory_entry && other) noexcept
        : base_class(other)
    {}
	
    explicit directory_entry (pfs::filesystem::path const & p)
        : base_class(p)
    {}
    
    // TODO Implement
    //directory_entry (path const & p, std::error_code & ec);
    
    //==========================================================================
    // Modifiers
    //==========================================================================
    directory_entry & operator = (directory_entry const & other)
    {
        base_class::operator = (other);
        return *this;
    }

    directory_entry & operator = (directory_entry && other) noexcept
    {
        base_class::operator = (other);
        return *this;
    }
    
    /**
     * @fn void directory_entry::assign (path const & p)
     * 
     * @param p Path to the file system object to which the 
     *          directory entry will refer.
     */
    
    
    // TODO Implement
    //void assign (path const & p, std::error_code & ec);
};

}} // pfs::filesystem

#else

#   include <pfs/cxx/cxx98/filesystem/directory_entry.hpp>

#endif

#endif /* __PFS_CXX_CXX11_FILESYSTEM_DIRECTORY_ENTRY_HPP__ */

