/* 
 * File:   filesystem_error.hpp
 * Author: wladt
 *
 * Created on Sep 07, 2017
 */

#ifndef __PFS_CXX_CXX98_FILESYSTEM_FILESYSTEM_ERROR_HPP__
#define __PFS_CXX_CXX98_FILESYSTEM_FILESYSTEM_ERROR_HPP__

#if HAVE_BOOST_FILESYSTEM

#include <boost/filesystem/operations.hpp>
#include <pfs/system_error.hpp>

namespace pfs {
namespace filesystem { 
namespace details {

struct filesystem_error : public ::boost::filesystem::filesystem_error
{
    typedef ::boost::filesystem::filesystem_error base_class;
    
    filesystem_error (std::string const & what, pfs::error_code ec)
        : base_class(what, boost::convert(ec))
    {}
            
    filesystem_error (std::string const & what
            , pfs::filesystem::path const & p1
            , pfs::error_code ec)
       : base_class(what, p1, boost::convert(ec))
    {}
            
    filesystem_error (std::string const & what
            , pfs::filesystem::path const & p1
            , pfs::filesystem::path const & p2
            , pfs::error_code ec)
        : base_class(what, p1, p2, boost::convert(ec))
    {}
}

}}} // pfs::filesystem

#else

#   error "Need to implement"

#endif

#endif /* __PFS_CXX_CXX98_FILESYSTEM_FILESYSTEM_ERROR_HPP__ */

