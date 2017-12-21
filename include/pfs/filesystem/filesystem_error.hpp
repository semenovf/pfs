#ifndef __PFS_FILESYSTEM_ERROR_HPP__
#define __PFS_FILESYSTEM_ERROR_HPP__

#include <string>
#include <pfs/filesystem/path.hpp>
#include <pfs/cxxversion.hpp>
#include PFS_CXX_HEADER(filesystem/filesystem_error)
#include PFS_CXX_HEADER(system_error)

namespace pfs {
namespace filesystem {

template <typename PathImpl>
class basic_path;

template <typename Impl>
class basic_filesystem_error 
{
    Impl _d;
    
protected:
    basic_filesystem_error (Impl const & d)
        : _d(d)
    {}
    
public:
    basic_filesystem_error (std::string const & what_arg
            , pfs::error_code ec)
        : _d(what_arg, ec)
    {}
    
    template <typename PathImpl>
    basic_filesystem_error (std::string const & what_arg
            , basic_path<PathImpl> const & p1
            , pfs::error_code ec)
        : _d(what_arg, p1.native_path(), ec)
    {}
    
    template <typename PathImpl>
    basic_filesystem_error (std::string const & what_arg
            , basic_path<PathImpl> const & p1
            , basic_path<PathImpl> const & p2
            , pfs::error_code ec)
        : _d(what_arg, p1.native_path(), p2.native_path(), ec)
    {}
};

typedef basic_filesystem_error<details::filesystem_error> filesystem_error;

}} // pfs::filesystem

#endif /* __PFS_FILESYSTEM_ERROR_HPP__ */

