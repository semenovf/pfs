#pragma once
#include <pfs/compiler.hpp>

#if 0 //  PFS_CC_GCC_VERSION >= 50300 && HAVE_STDCXX_FS_EXPERIMENTAL

#include <experimental/filesystem>

namespace pfs {
namespace filesystem {
namespace details {

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
#include <pfs/cxx/cxx98/filesystem/directory_entry.hpp>
#endif
