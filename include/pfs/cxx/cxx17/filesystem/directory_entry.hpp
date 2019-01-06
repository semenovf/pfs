#pragma once
#include <pfs/config.h>

#if HAVE_STDCXX_FILESYSTEM

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
