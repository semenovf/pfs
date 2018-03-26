#ifndef __PFS_CXX_CXX98_FILESYSTEM_DIRECTORY_ENTRY_HPP__
#define __PFS_CXX_CXX98_FILESYSTEM_DIRECTORY_ENTRY_HPP__

#include <pfs/config.h>

#if HAVE_BOOST_FILESYSTEM

#include <boost/filesystem/operations.hpp>

namespace pfs {
namespace filesystem {

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
};

}} // pfs::filesystem

#else
#   error "Filesystem implementation not found"
#endif

#endif /* __PFS_CXX_CXX98_FILESYSTEM_DIRECTORY_ENTRY_HPP__ */

