/* 
 * File:   constants.hpp
 * Author: wladt
 *
 * Created on Sep 07, 2017
 */
#ifndef __PFS_CXX_CXX98_FILESYSTEM_CONSTANTS_HPP__
#define __PFS_CXX_CXX98_FILESYSTEM_CONSTANTS_HPP__

#if HAVE_BOOST_FILESYSTEM

#include <boost/filesystem/operations.hpp>

namespace pfs {
namespace filesystem { 
namespace details {

struct perms
{
    enum value_enum
    {
          no_perms     = boost::filesystem::no_perms
        , owner_read   = boost::filesystem::owner_read
        , owner_write  = boost::filesystem::owner_write
        , owner_exe    = boost::filesystem::owner_exe
        , owner_all    = boost::filesystem::owner_all

        , group_read   = boost::filesystem::group_read
        , group_write  = boost::filesystem::group_write
        , group_exe    = boost::filesystem::group_exe
        , group_all    = boost::filesystem::group_all

        , others_read  = boost::filesystem::others_read
        , others_write = boost::filesystem::others_write
        , others_exe   = boost::filesystem::others_exe
        , others_all   = boost::filesystem::others_all
                  
        , all_all      = boost::filesystem::all_all

        // other POSIX bits

        , set_uid_on_exe = boost::filesystem::set_uid_on_exe
        , set_gid_on_exe = boost::filesystem::set_gid_on_exe
        , sticky_bit     = boost::filesystem::sticky_bit

        , perms_mask     = boost::filesystem::perms_mask

        , perms_not_known = boost::filesystem::perms_not_known

        , add_perms       = boost::filesystem::add_perms
        , remove_perms    = boost::filesystem::remove_perms

        , symlink_perms   = boost::filesystem::symlink_perms
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

#error  "Need to implement"

#endif

#endif /* __PFS_CXX_CXX98_FILESYSTEM_CONSTANTS_HPP__ */

