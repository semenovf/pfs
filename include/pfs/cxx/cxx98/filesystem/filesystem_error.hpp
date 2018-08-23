#pragma once
#include <pfs/config.h>

#if HAVE_BOOST_FILESYSTEM

#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>

namespace pfs {
namespace filesystem {

class filesystem_error : public ::boost::filesystem::filesystem_error
{
    std::string _enhanced_msg;

public:
    typedef ::boost::filesystem::filesystem_error base_class;

    filesystem_error (std::string const & what_arg
            , error_code ec)
        : base_class(what_arg
                , error_code_cast<error_code, ::boost::system::error_code>(ec))
    {}

    filesystem_error (std::string const & what_arg
            , path const & p1
            , error_code ec)
        : base_class(what_arg
            , p1
            , error_code_cast<error_code, ::boost::system::error_code>(ec))
     {}

    filesystem_error (std::string const & what_arg
            , path const & p1
            , path const & p2
            , error_code ec)
        : base_class(what_arg
            , p1
            , p2
            , error_code_cast<error_code, ::boost::system::error_code>(ec))
    {}
};

}} // pfs::filesystem

#else
#   error "Filesystem implementation not found"
#endif
