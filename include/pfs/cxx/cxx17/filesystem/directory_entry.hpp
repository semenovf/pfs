#ifndef __PFS_CXX_CXX17_FILESYSTEM_DIRECTORY_ENTRY_HPP__
#define __PFS_CXX_CXX17_FILESYSTEM_DIRECTORY_ENTRY_HPP__

#include <pfs/compiler.hpp>

#if PFS_CC_GCC_VERSION >= 80000

#include <filesystem>

namespace pfs {
namespace filesystem {
namespace details {

class directory_entry : public std::filesystem::directory_entry
{
    typedef std::filesystem::directory_entry base_class;
    
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
    
//    explicit directory_entry (pfs::filesystem::path const & p)
//        : base_class(p)
//    {}
    
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

}}} // pfs::filesystem::details

#else

#include <pfs/cxx/cxx14/filesystem/directory_entry.hpp>

#endif

#endif /* __PFS_CXX_CXX17_FILESYSTEM_DIRECTORY_ENTRY_HPP__ */

