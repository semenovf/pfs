/* 
 * File:   path.hpp
 * Author: wladt
 *
 * Created on Sep 07, 2017
 */

#ifndef __PFS_CXX_CXX98_FILESYSTEM_PATH_HPP__
#define __PFS_CXX_CXX98_FILESYSTEM_PATH_HPP__

#include <pfs/cxxlang.hpp>

#if HAVE_BOOST_FILESYSTEM

#include <boost/filesystem/path.hpp>
#include <pfs/system_error.hpp>

namespace pfs {
namespace filesystem {
namespace details {

class path : public ::boost::filesystem::path
{
    typedef ::boost::filesystem::path base_class;
    
public:
    typedef base_class native_path_type;
    typedef typename base_class::string_type string_type;

public:
    path ()
        : base_class()
    {}

    path (path const & other)
        : base_class(other)
    {}

    path const & operator = (path const & other)
    {
        base_class::operator = (other);
        return *this;
    }

    template <typename Source>
    explicit path (Source const & source)
        : base_class(source)
    {}

    template <typename InputIt>
    path (InputIt first, InputIt last)
        : base_class(first, last)
    {}

//    template <typename Source>
//    path (Source const & source, std::locale const & loc)
//        : base_class(source, loc)
//    {}

//    // FIXME fmt unused yet
//    template <typename InputIt>
//    path (InputIt first, InputIt last, std::locale const & loc)
//        : base_class(first, last, loc)
//    {}

    template <typename Source>
    path & operator = (Source const & source)
    {
        base_class::operator = (source);
        return *this;
    }
    
    operator string_type () const
    {
        return base_class::native();
    }


    // TODO Implement file_status
    //bool exists (filesystem::file_status s) noexcept;

    bool exists (::boost::system::error_code & ec) const pfs_noexcept
    {
        return ::boost::filesystem::exists(*this, ec);
    }

    bool remove (::boost::system::error_code & ec) const pfs_noexcept
    {
        return ::boost::filesystem::remove(*this, ec);
    }
    
    path & replace_filename (path const & replacement)
    {
        remove_filename();
        base_class::operator /= (replacement);
        return *this;
    }
};

}}} // pfs::filesystem::details

#else

#   error "Need to implement"

#endif

#endif /* __PFS_CXX_CXX98_FILESYSTEM_PATH_HPP__ */
