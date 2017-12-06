#ifndef __PFS_CXX_CXX98_FILESYSTEM_DIRECTORY_ENTRY_HPP__
#define __PFS_CXX_CXX98_FILESYSTEM_DIRECTORY_ENTRY_HPP__

#include <pfs/config.h>
#include <pfs/cxxlang.hpp>

#if HAVE_BOOST_FILESYSTEM

#include <boost/filesystem/operations.hpp>
//#include "../system_error.hpp"

namespace pfs {
namespace filesystem {
namespace details {

class directory_entry : public ::boost::filesystem::directory_entry
{
    typedef ::boost::filesystem::directory_entry base_class;
    
public:
    directory_entry () pfs_noexcept
        : base_class()
    {}
        
    directory_entry (directory_entry const & other)
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

#   error "Need to implement"

#endif

#endif /* __PFS_CXX_CXX98_FILESYSTEM_DIRECTORY_ENTRY_HPP__ */

