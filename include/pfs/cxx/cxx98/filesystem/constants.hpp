#ifndef __PFS_CXX_CXX98_FILESYSTEM_CONSTANTS_HPP__
#define __PFS_CXX_CXX98_FILESYSTEM_CONSTANTS_HPP__

#include <pfs/config.h>

#if HAVE_BOOST_FILESYSTEM

#include <boost/filesystem/operations.hpp>

namespace pfs {
namespace filesystem { 
namespace details {

struct perms
{
    enum value_enum
    {
          none         = boost::filesystem::no_perms
        , owner_read   = boost::filesystem::owner_read
        , owner_write  = boost::filesystem::owner_write
        , owner_exec   = boost::filesystem::owner_exe
        , owner_all    = boost::filesystem::owner_all

        , group_read   = boost::filesystem::group_read
        , group_write  = boost::filesystem::group_write
        , group_exec   = boost::filesystem::group_exe
        , group_all    = boost::filesystem::group_all

        , others_read  = boost::filesystem::others_read
        , others_write = boost::filesystem::others_write
        , others_exec  = boost::filesystem::others_exe
        , others_all   = boost::filesystem::others_all
                  
        , all          = boost::filesystem::all_all

        , set_uid      = boost::filesystem::set_uid_on_exe
        , set_gid      = boost::filesystem::set_gid_on_exe
        , sticky_bit   = boost::filesystem::sticky_bit

        , mask         = boost::filesystem::perms_mask

        , unknown      = boost::filesystem::perms_not_known
//
//        , add_perms       = boost::filesystem::add_perms
//        , remove_perms    = boost::filesystem::remove_perms
//
//        , symlink_perms   = boost::filesystem::symlink_perms
    };
    
    int v;
    
    perms (int x)
        : v(x)
    {}

    perms & operator = (int x)
    {
        v = x;
        return *this;
    }
    
    operator int () const
    {
        return v;
    }
};

}}} // pfs::filesystem::details

#else
#   error "Filesystem implementation not found"
#endif

#endif /* __PFS_CXX_CXX98_FILESYSTEM_CONSTANTS_HPP__ */

