#pragma once

namespace pfs {
namespace filesystem { 
namespace details {

struct perms
{
    // See std::filesystem::perms (C++17)
    enum value_enum
    {
          none         = 0
        , owner_read   = 0400
        , owner_write  = 0200
        , owner_exec   = 0100
        , owner_all    = 0700

        , group_read   = 040
        , group_write  = 020
        , group_exec   = 010
        , group_all    = 070

        , others_read  = 04
        , others_write = 02
        , others_exec  = 01
        , others_all   = 07
                  
        , all          = 0777

        , set_uid      = 04000
        , set_gid      = 02000
        , sticky_bit   = 01000

        , mask         = 07777

        , unknown      = 0xFFFF
//
//        , add_perms       = ::boost::filesystem::add_perms
//        , remove_perms    = ::boost::filesystem::remove_perms
//
//        , symlink_perms   = ::boost::filesystem::symlink_perms
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

