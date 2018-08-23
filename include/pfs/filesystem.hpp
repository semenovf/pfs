#pragma once
#include <pfs/filesystem/constants.hpp>
#include <pfs/filesystem/path.hpp>
#include <pfs/filesystem/pathlist.hpp>
#include <pfs/filesystem/filesystem_error.hpp>
#include <pfs/filesystem/directory_entry.hpp>

namespace pfs {
namespace filesystem {

inline bool exists (path const & p)
{
    error_code ec;

    if (!exists(p, ec)) {
        if (ec) throw filesystem_error("exists", p, ec);
        return false;
    }

    return true;
}

inline bool remove (path const & p)
{
    error_code ec;

    if (!remove(p, ec)) {
        if (ec) throw filesystem_error("remove", p, ec);
        return false;
    }

    return true;
}

inline void rename (path const & old_p, path const & new_p)
{
    error_code ec;
    rename(old_p, new_p, ec);

    if (ec) throw filesystem_error("rename", old_p, new_p, ec);
}

}} // namespace pfs::filesystem
