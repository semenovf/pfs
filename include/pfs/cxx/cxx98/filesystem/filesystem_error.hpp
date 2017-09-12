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
#include <boost/filesystem/path.hpp>

namespace pfs {
namespace filesystem { 
namespace details {

typedef ::boost::filesystem::filesystem_error filesystem_error;
    
//struct filesystem_error : public ::boost::filesystem::filesystem_error
//{
//    typedef ::boost::filesystem::filesystem_error base_class;
//    
//    filesystem_error (std::string const & what_arg
//            , ::boost::system::error_code ec)
//        : base_class(what_arg, ec)
//    {}
//            
//    filesystem_error (std::string const & what_arg
//            , ::boost::filesystem::path const & p1
//            , ::boost::system::error_code ec)
//       : base_class(what_arg, p1, ec)
//    {}
//            
//    filesystem_error (std::string const & what_arg
//            , ::boost::filesystem::path const & p1
//            , ::boost::filesystem::path const & p2
//            , ::boost::system::error_code ec)
//        : base_class(what_arg, p1, p2, ec)
//    {}
//};

}}} // pfs::filesystem

#else

#   error "Need to implement"

#endif

#endif /* __PFS_CXX_CXX98_FILESYSTEM_FILESYSTEM_ERROR_HPP__ */

